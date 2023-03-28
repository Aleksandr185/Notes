/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "abstract_tree_model.h"

#include "abstract_tree_node.h"

// --------------------------- RootTreeNode ------------------------------------
class RootTreeNode : public AbstractTreeNode
{
public:
  RootTreeNode() : AbstractTreeNode(Q_NULLPTR, -1) {}
  ~RootTreeNode() {}

  QVariant data(int /*column*/, int /* role*/) const override {return QVariant();}
};

// -----------------------------------------------------------------------------


// ------------------------- AbstractTreeModel ---------------------------------
AbstractTreeModel::AbstractTreeModel(QObject *parent)
  :QAbstractItemModel(parent), m_root(new RootTreeNode()), m_flag_started_reset(false)
{

}

AbstractTreeModel::~AbstractTreeModel()
{
  deleteRoot();
}

// Override

QVariant AbstractTreeModel::data(const QModelIndex &index, int role) const
{
  AbstractTreeNode* index_node = node(index, Q_NULLPTR);
  if (index_node)
    return index_node->data(index.column(), role);
  return QVariant();
}

QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  AbstractTreeNode* parentNode = node(parent, m_root);
  if (!parentNode)
    return QModelIndex();

  AbstractTreeNode* childNode = parentNode->child(row);
  if (!childNode)
    return QModelIndex();

  return createIndex(row, column, childNode);
}

QModelIndex AbstractTreeModel::parent(const QModelIndex &child) const
{
  if (!child.isValid())
    return QModelIndex();

  AbstractTreeNode* childNode = static_cast<AbstractTreeNode*>(child.internalPointer());
  if (!childNode)
    return QModelIndex();

  AbstractTreeNode* parentNode = childNode->parent();
  if (!parentNode || (parentNode ==m_root))
    return QModelIndex();

  return createIndex(parentNode->index(), 0, parentNode);
}

int AbstractTreeModel::rowCount(const QModelIndex& parent) const
{
  AbstractTreeNode* parentNode = node(parent, m_root);
  if (parentNode)
    return parentNode->childCount();

  return 0;
}

Qt::ItemFlags AbstractTreeModel::flags(const QModelIndex& index) const
{
  AbstractTreeNode* index_node = node(index, Q_NULLPTR);
  if (index_node)
    return index_node->flags(index.column());

  return QAbstractItemModel::flags(index);
}

bool AbstractTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  AbstractTreeNode* index_node = node(index, Q_NULLPTR);
  if (index_node) {
    const bool result = index_node->setData(index.column(), value, role);
    if (result)
      emit dataChanged(index, index, QVector<int>{role});
    return result;
  }

  return false;
}

// Protected

AbstractTreeNode* AbstractTreeModel::node(const QModelIndex& index,
                                          AbstractTreeNode* default_node) const
{
  if (index.isValid())
    return static_cast<AbstractTreeNode*>(index.internalPointer());

  return default_node;
}

void AbstractTreeModel::deleteRoot()
{
  delete m_root;
  m_root = Q_NULLPTR;
}

void AbstractTreeModel::createNewRoot()
{
  deleteRoot();
  m_root = new RootTreeNode();
}

void AbstractTreeModel::startReset()
{
  if ( !m_flag_started_reset ) {
    m_flag_started_reset = true;
    beginResetModel();

    if (m_root->childCount() > 0)
      createNewRoot();
  }
}

void AbstractTreeModel::endReset()
{
  if ( m_flag_started_reset ) {
    m_flag_started_reset = false;
    endResetModel();
  }
}

QModelIndex AbstractTreeModel::index(AbstractTreeNode* node, int column) const
{
  if (!node || (node == m_root))
    return QModelIndex();

  const int row = node->index();
  QModelIndex parent_index = index(node->parent());

  return index(row, column, parent_index);
}
