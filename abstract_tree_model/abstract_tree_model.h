#ifndef ABSTRACT_TREE_MODEL_H
#define ABSTRACT_TREE_MODEL_H

#include <QAbstractItemModel>

class AbstractTreeNode;

class AbstractTreeModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  explicit AbstractTreeModel(QObject *parent = Q_NULLPTR);
  ~AbstractTreeModel();

  QVariant data(const QModelIndex &index, int role) const override;

  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &child) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;

protected:
  inline AbstractTreeNode* rootNode() const {return m_root;} // rooteNode всегда != NULL
  AbstractTreeNode* node(const QModelIndex& index, AbstractTreeNode* default_node) const;

  void createNewRoot();
  void startReset();
  void endReset();

  QModelIndex index(AbstractTreeNode* node, int column = 0) const;

private:
  AbstractTreeNode* m_root;
  bool m_flag_started_reset;

  void deleteRoot();
};

#endif // ABSTRACT_TREE_MODEL_H
