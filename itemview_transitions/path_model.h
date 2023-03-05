#ifndef PATHMODEL_H
#define PATHMODEL_H

#include <QAbstractListModel>
#include <QtPositioning/QGeoPath>
#include <QHash>

class PathModel : public QAbstractListModel
{
  Q_OBJECT
public:
  PathModel();

  // QAbstractItemModel interface
  int rowCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

private:
  QVector<QGeoPath> _data;
};


#endif // PATHMODEL_H
