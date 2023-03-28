#ifndef ORGANIZATIONS_MODEL_H
#define ORGANIZATIONS_MODEL_H

#include "models/abstract_tree_model.h"

class Organization;
class OrganizationStorage;

class OrganizationsModelPrivate;

class OrganizationsModel : public AbstractTreeModel
{
  Q_OBJECT
public:
  enum ColumnIndex{
    ColumnIndex,
    ColumnText,
    ColumnCount
  };

  explicit OrganizationsModel(QObject* parent = Q_NULLPTR);
  ~OrganizationsModel() override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& /*parent*/) const override
  {return ColumnCount;}
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  OrganizationStorage* storage() const;

public slots:
  void refresh();
  void setStorage(OrganizationStorage* storage);

protected:
  const QScopedPointer<OrganizationsModelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(OrganizationsModel)

private slots:
  void _q_storage_refreshed();
  void _q_storage_destroyed(QObject*);

  void _q_organization_added(Organization* o);
  void _q_organization_removed(Organization* o);
  void _q_organization_changed();
};

#endif // ORGANIZATIONS_MODEL_H
