#ifndef ABSTRACT_TREE_NODE_H
#define ABSTRACT_TREE_NODE_H

#include <QVariant>
#include "utils/fast_pimpl.h"


class AbstractTreeNode
{
public:

  enum Type{
    None = 0,
    User = 1,
    MaxUser = 65535
  };

  AbstractTreeNode(void* p, int t, AbstractTreeNode* parent = Q_NULLPTR);
  virtual ~AbstractTreeNode();

  void*             pointer() const;
  AbstractTreeNode* parent() const;
  AbstractTreeNode* child(int index) const;
  AbstractTreeNode* find(void* p);
  QList<AbstractTreeNode*> findAll(void *p);

  int childCount() const;
  int index()      const;
  int type()       const;

  void appendChild(AbstractTreeNode* child);
  void removeChild(int index);
  void insertChild(int index, AbstractTreeNode* child);

  virtual QVariant  data(int column, int role) const = 0;
  virtual Qt::ItemFlags flags(int column) const;
  virtual bool setData(int column, const QVariant& value, int role);


  static int registerType(int hint = -1) Q_DECL_NOTHROW;

private:
  class Private;

#ifdef Q_PROCESSOR_X86_64
  static constexpr std::size_t PrivateSize  = 40;
  static constexpr std::size_t PrivateAlign =  8;
#elif defined(Q_PROCESSOR_X86_32)
  static constexpr std::size_t PrivateSize  = 20;
  static constexpr std::size_t PrivateAlign =  4;
#endif

  utils::FastPimpl<Private, PrivateSize, PrivateAlign> d;

};

#endif // ABSTRACT_TREE_NODE_H
