/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "abstract_tree_node.h"

#include <QList>
#include <QMultiHash>
#include <qbasicatomic.h>

#include "globals/garbage_controller.h"

// --------------------------------- AbstractTreeNode::Private -------------------------------------
class AbstractTreeNode::Private
{
public:
  Private(void* _ptr, int _type, AbstractTreeNode* _parent);
  ~Private();

  AbstractTreeNode*        parent;
  QList<AbstractTreeNode*> childs;
  QMultiHash<void*, AbstractTreeNode*> childs_hash;

  int    type;
  void*  ptr;
};

AbstractTreeNode::Private::Private(void *_ptr, int _type, AbstractTreeNode *_parent)
  :parent(_parent), type(_type), ptr(_ptr)
{
  OBJECT_CREATED(AbstractTreeNode::Private)
}

AbstractTreeNode::Private::~Private()
{
  qDeleteAll(childs);
  OBJECT_DELETED(AbstractTreeNode::Private)
}
// -------------------------------------------------------------------------------------------------


// ------------------------------------- AbstractTreeNode ------------------------------------------
AbstractTreeNode::AbstractTreeNode(void* p, int t, AbstractTreeNode* parent)
  : d(p, t, parent)
{
  OBJECT_CREATED(AbstractTreeNode)
}

AbstractTreeNode::~AbstractTreeNode()
{
  OBJECT_DELETED(AbstractTreeNode)
}

void* AbstractTreeNode::pointer() const
{
  return d->ptr;
}

AbstractTreeNode* AbstractTreeNode::parent() const
{
  return d->parent;
}

AbstractTreeNode* AbstractTreeNode::child(int index) const
{
  return d->childs.value(index, Q_NULLPTR);
}

AbstractTreeNode* AbstractTreeNode::find(void* p)
{
  if (d->ptr == p)
    return this;

  if (d->childs_hash.contains(p))
    return d->childs_hash.value(p);

  for (int i = 0; i < d->childs.size(); ++i) {
    AbstractTreeNode* result = d->childs.at(i)->find(p);
    if (result)
      return result;
  }

  return Q_NULLPTR;
}

QList<AbstractTreeNode*> AbstractTreeNode::findAll(void* p)
{
  QList<AbstractTreeNode*> result;

  if (d->ptr == p)
    result << this;

  for (int i = 0; i < d->childs.size(); ++i)
    result << d->childs.at(i)->findAll(p);

  return result;
}

int AbstractTreeNode::childCount() const
{
  return d->childs.count();
}

int AbstractTreeNode::index() const
{
  if (d->parent)
    return d->parent->d->childs.indexOf(const_cast<AbstractTreeNode*>(this));

  return 0;
}

int AbstractTreeNode::type() const
{
  return d->type;
}

void AbstractTreeNode::appendChild(AbstractTreeNode* child)
{
  if (!child || (child->parent() != this) || d->childs.contains(child))
    return;

  d->childs.append(child);
  d->childs_hash.insert(child->pointer(), child);
}

void AbstractTreeNode::removeChild(int index)
{
  if ((index >= 0) && (index < d->childs.size())){
    AbstractTreeNode* n = d->childs.takeAt(index);
    d->childs_hash.remove(n->pointer(), n);
    delete n;
  }
}

void AbstractTreeNode::insertChild(int index, AbstractTreeNode* child)
{
  if (!child || (child->parent() != this) || d->childs.contains(child))
    return;

  d->childs.insert(index, child);
  d->childs_hash.insert(child->pointer(), child);
}

Qt::ItemFlags AbstractTreeNode::flags(int column) const
{
  Q_UNUSED(column)
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool AbstractTreeNode::setData(int column, const QVariant& value, int role)
{
  Q_UNUSED(column)
  Q_UNUSED(value)
  Q_UNUSED(role)
  return false;
}
// -------------------------------------------------------------------------------------------------

namespace {
template <size_t N>
struct BasicAtomicBitField {
    enum {
        BitsPerInt = std::numeric_limits<uint>::digits,
        NumInts = (N + BitsPerInt - 1) / BitsPerInt,
        NumBits = N
    };
    // This atomic int points to the next (possibly) free ID saving
    // the otherwise necessary scan through 'data':
    QBasicAtomicInteger<uint> next;
    QBasicAtomicInteger<uint> data[NumInts];

    bool allocateSpecific(int which) Q_DECL_NOTHROW
    {
        QBasicAtomicInteger<uint> &entry = data[which / BitsPerInt];
        const uint old = entry.loadRelaxed();
        const uint bit = 1U << (which % BitsPerInt);
        return !(old & bit) // wasn't taken
            && entry.testAndSetRelaxed(old, old | bit); // still wasn't taken
        // don't update 'next' here - it's unlikely that it will need
        // to be updated, in the general case, and having 'next'
        // trailing a bit is not a problem, as it is just a starting
        // hint for allocateNext(), which, when wrong, will just
        // result in a few more rounds through the allocateNext()
        // loop.
    }

    int allocateNext() Q_DECL_NOTHROW
    {
        // Unroll loop to iterate over ints, then bits? Would save
        // potentially a lot of cmpxchgs, because we can scan the
        // whole int before having to load it again.
        // Then again, this should never execute many iterations, so
        // leave like this for now:
        for (uint i = next.loadRelaxed(); i < NumBits; ++i) {
            if (allocateSpecific(i)) {
                // remember next (possibly) free id:
                const uint oldNext = next.loadRelaxed();
                next.testAndSetRelaxed(oldNext, qMax(i + 1, oldNext));
                return i;
            }
        }
        return -1;
    }
};
} // unnamed namespace

typedef BasicAtomicBitField<AbstractTreeNode::MaxUser - AbstractTreeNode::User + 1> UserTypeRegistry;

static UserTypeRegistry userTypeRegistry;

static inline int registerTypeZeroBased(int id) Q_DECL_NOTHROW
{
    // if the type hint hasn't been registered yet, take it:
    if (id < UserTypeRegistry::NumBits && id >= 0 && userTypeRegistry.allocateSpecific(id))
        return id;
    // otherwise, ignore hint:
    return userTypeRegistry.allocateNext();
}
/*!
    \since 4.4
    \threadsafe
    Registers and returns a custom event type. The \a hint provided
    will be used if it is available, otherwise it will return a value
    between QEvent::User and QEvent::MaxUser that has not yet been
    registered. The \a hint is ignored if its value is not between
    QEvent::User and QEvent::MaxUser.
    Returns -1 if all available values are already taken or the
    program is shutting down.
*/
int AbstractTreeNode::registerType(int hint) Q_DECL_NOTHROW
{
    const int result = registerTypeZeroBased(MaxUser - hint);
    return result < 0 ? -1 : MaxUser - result ;
}
// -------------------------------------------------------------------------------------------------
