/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "organizations_model.h"

#include <QFont>
#include <QPalette>

#include "abstract_tree_node.h"

#include "organization.h"
#include "organization_storage.h"

#include "utils/utils.h"
#include "model_utils.h"
#include "globals/garbage_controller.h"

namespace {

// --------------------------- OrganizationNode ---------------------------------
class OrganizationNode : public AbstractTreeNode
{
public:
  OrganizationNode(OrganizationPtr o, AbstractTreeNode* parent = Q_NULLPTR);
  ~OrganizationNode();

  QVariant data(int column, int role) const override;

  inline OrganizationPtr organization() const { return static_cast<OrganizationPtr>(pointer()); }

  QVariant getDataDisplayRole(int column) const;

  static int type();
};

OrganizationNode::OrganizationNode(OrganizationPtr o, AbstractTreeNode *parent)
  :AbstractTreeNode(o, OrganizationNode::type(), parent)
{
  Q_ASSERT_X(o, "create OrganizationNode", "Organization ptr is nullptr!");
  OBJECT_CREATED(OrganizationNode);
}

OrganizationNode::~OrganizationNode()
{
  OBJECT_DELETED(OrganizationNode);
}

int OrganizationNode::type()
{
  static int NODE_TYPE = AbstractTreeNode::None;

  if (NODE_TYPE == AbstractTreeNode::None)
    NODE_TYPE = AbstractTreeNode::registerType();

  return NODE_TYPE;
}

QVariant OrganizationNode::data(int column, int role) const
{
  Organization* o = organization();

  switch (role) {
  case Qt::DisplayRole:
  case Qt::EditRole:
    return getDataDisplayRole(column);
  case Qt::TextAlignmentRole:
    return column == OrganizationsModel::ColumnText ? QVariant(Qt::AlignLeft | Qt::AlignVCenter)
                                                    : QVariant(Qt::AlignCenter);
  case Qt::FontRole:
    return ModelUtils::getDataFontRole(*o);
  case Qt::ForegroundRole:
    return ModelUtils::getDataForegroundRole(*o);
  case Qt::WhatsThisRole:
    return QVariant(o->id().toInt());
  case Qt::UserRole:
    return QVariant::fromValue(o);
  default:
    return QVariant();
  }
}

QVariant OrganizationNode::getDataDisplayRole(int column) const
{
  Organization* o = organization();
  switch (column) {
    case OrganizationsModel::ColumnIndex:
      return QVariant(index() + 1);
    case OrganizationsModel::ColumnText:
      return QVariant(o->text());
    default:
      return QVariant();
  }
}
// -----------------------------------------------------------------------------

} // namespace unnamed

// ----------------------- OrganizationsModelPrivate ----------------------------
class OrganizationsModelPrivate
{
  Q_DECLARE_PUBLIC(OrganizationsModel)
public:
  OrganizationsModelPrivate();
  ~OrganizationsModelPrivate();

  OrganizationsModel* q_ptr;

  OrganizationStorage* storage;
  QVector<QMetaObject::Connection> storage_connections;
  QVector<QMetaObject::Connection> objects_connections;

  QVariant getHeaderDisplayRole(int section) const;
  QVariant getHeaderTextAlignmentRole(int section) const;

  void addOrganization(Organization* o);
};

OrganizationsModelPrivate::OrganizationsModelPrivate()
  :q_ptr(Q_NULLPTR), storage(Q_NULLPTR)
{
  OBJECT_CREATED(OrganizationsModelPrivate)
}

OrganizationsModelPrivate::~OrganizationsModelPrivate()
{
  OBJECT_DELETED(OrganizationsModelPrivate)
}

QVariant OrganizationsModelPrivate::getHeaderDisplayRole(int section) const
{
  switch (section) {
    case OrganizationsModel::ColumnIndex:
      return QStringLiteral("№ п/п");
    case OrganizationsModel::ColumnText:
      return QStringLiteral("Текст");
    default:
      return QVariant();
  }
}

QVariant OrganizationsModelPrivate::getHeaderTextAlignmentRole(int section) const
{
  if (section == OrganizationsModel::ColumnText)
    return QVariant(Qt::AlignLeft | Qt::AlignVCenter);

  return QVariant(Qt::AlignCenter);
}

void OrganizationsModelPrivate::addOrganization(Organization* o)
{
  Q_Q(OrganizationsModel);

  AbstractTreeNode* root = q->rootNode();
  if (!o || !root)
    return;

  // дважды организацию не добавляем
  if (root->find(o))
    return;

  // Connect Organization
  objects_connections.append(QObject::connect(o, &Organization::changed,
                                              q_ptr, &OrganizationsModel::_q_organization_changed));

  root->appendChild(new OrganizationNode(o, root));
}

// -----------------------------------------------------------------------------

// --------------------------- OrganizationsModel -------------------------------
OrganizationsModel::OrganizationsModel(QObject *parent)
  :AbstractTreeModel(parent), d_ptr(new OrganizationsModelPrivate())
{
  Q_D(OrganizationsModel);
  d->q_ptr = this;
  OBJECT_CREATED(OrganizationsModel)
}

OrganizationsModel::~OrganizationsModel()
{
  OBJECT_DELETED(OrganizationsModel)
}

int OrganizationsModel::rowCount(const QModelIndex& parent) const
{
  Q_D(const OrganizationsModel);

  if (!parent.isValid() && d->storage && !d->storage->isLoaded())
    d->storage->loadLater();

  return AbstractTreeModel::rowCount(parent);

}

// GET

QVariant OrganizationsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation != Qt::Horizontal)
    return QVariant();

  Q_D(const OrganizationsModel);

  switch (role) {
    case Qt::DisplayRole:
      return d->getHeaderDisplayRole(section);
    case Qt::TextAlignmentRole:
      return d->getHeaderTextAlignmentRole(section);
    default:
      return QVariant();
  }
}

OrganizationStorage* OrganizationsModel::storage() const
{
  Q_D(const OrganizationsModel);
  return d->storage;
}

void OrganizationsModel::refresh()
{
  Q_D(OrganizationsModel);

  if (d->storage){
    if (d->storage->isLoaded())
      d->storage->reloadLater();
    else
      d->storage->loadLater();
  }
}

// SET

void OrganizationsModel::setStorage(OrganizationStorage* storage)
{
  Q_D(OrganizationsModel);

  if (d->storage == storage)
    return;

  startReset();

  // 1. Disconnect old storage
  utils::disconnectAll(d->storage_connections);

  // 2. Disconnect old Organizations
  utils::disconnectAll(d->objects_connections);

  // 3. Set new storage
  d->storage = storage;

  // 4. Connect new storage
  if (d->storage){
    d->storage_connections.append(connect(d->storage, &OrganizationStorage::loaded,
                                          this, &OrganizationsModel::_q_storage_refreshed));
    d->storage_connections.append(connect(d->storage, &OrganizationStorage::destroyed,
                                  this, &OrganizationsModel::_q_storage_destroyed));
    d->storage_connections.append(connect(d->storage, &OrganizationStorage::added,
                                  this, &OrganizationsModel::_q_organization_added));
    d->storage_connections.append(connect(d->storage, &OrganizationStorage::removed,
                                  this, &OrganizationsModel::_q_organization_removed));

    if (d->storage->isLoaded())
      _q_storage_refreshed();
    else
      d->storage->loadLater();
  }

  endReset();
}

// private slots:

void OrganizationsModel::_q_storage_refreshed()
{
  Q_D(OrganizationsModel);

  if (!d->storage)
    return;

  startReset();

  // 1. Disconnect old Organizations
  utils::disconnectAll(d->objects_connections);

  // 2. Add Organizations
  const OrganizationList& list = d->storage->organizations();
  for (int i = 0; i < list.size(); ++i)
    d->addOrganization(list.at(i));

  endReset();
}

void OrganizationsModel::_q_storage_destroyed(QObject*)
{
  setStorage(Q_NULLPTR);
}

void OrganizationsModel::_q_organization_added(Organization* o)
{
  Q_D(OrganizationsModel);

  AbstractTreeNode* r = rootNode();

  beginInsertRows(QModelIndex(), r->childCount(), r->childCount());
  d->addOrganization(o);
  endInsertRows();
}

void OrganizationsModel::_q_organization_removed(Organization* o)
{
  AbstractTreeNode* n = rootNode()->find(o);

  if (n){
    QModelIndex node_index = index(n);
    beginRemoveRows(node_index.parent(), node_index.row(), node_index.row());
    n->parent()->removeChild(n->index());
    endRemoveRows();
  }
}

void OrganizationsModel::_q_organization_changed()
{
  AbstractTreeNode* n = rootNode()->find(sender());
  if (n)
    emit dataChanged(index(n), index(n, ColumnCount - 1));
}

// -----------------------------------------------------------------------------
