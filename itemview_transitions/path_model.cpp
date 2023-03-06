#include "path_model.h"

#include <QDebug>

PathModel::PathModel()
{
  {
    QGeoPath path;
    path.addCoordinate( QGeoCoordinate(47.3964167854329, 39.39240708140207) );
    path.addCoordinate( QGeoCoordinate(47.44635050187707, 39.63323080205191) );
    path.addCoordinate( QGeoCoordinate(47.42533155161557, 39.92454981894957) );
    path.addCoordinate( QGeoCoordinate(47.362224321381994, 40.02165615792131) );
    path.addCoordinate( QGeoCoordinate(47.354330603673986, 39.949797467090974) );
    path.addCoordinate( QGeoCoordinate(47.399046056531354, 39.8449226210015) );
    path.addCoordinate( QGeoCoordinate(47.4003606428801, 39.642941435940344) );
    path.addCoordinate( QGeoCoordinate(47.362224321381994, 39.421538983084815) );
    _data.append(path);
  }

  {
    QGeoPath path;
    path.addCoordinate( QGeoCoordinate(47.19884745706887, 39.40600196884935) );
    path.addCoordinate( QGeoCoordinate(47.13943268458777, 39.652652069846255) );
    path.addCoordinate( QGeoCoordinate(47.190929327427924, 39.88959153691977) );
    path.addCoordinate( QGeoCoordinate(47.23842038365426, 40.091572721980924) );
    path.addCoordinate( QGeoCoordinate(47.03099484461636, 40.07797783451616) );
    path.addCoordinate( QGeoCoordinate(47.074661005215276, 39.875996649472484) );
    _data.append(path);
  }


}

int PathModel::rowCount(const QModelIndex &parent) const
{
  return !parent.isValid() ? _data.size() : 0;
}

QVariant PathModel::data(const QModelIndex& index, int role) const
{
  qDebug() << "get role: " << roleNames().value(role);

  if (!index.isValid())
    return QVariant();

  switch (role) {
  case Qt::UserRole: {
      return QVariant::fromValue(_data.at(index.row())/*.path()*/);
    break;
    }
  case Qt::UserRole + 1:
  case Qt::DisplayRole : {
      return _data.at(index.row()).toString();
      //return QString("Text from model: %1").arg(index.row());
    }
  default:
    break;
  }

  //qDebug() << "get role: " << role;
  return QVariant();
}

QHash<int, QByteArray> PathModel::roleNames() const
{
  QHash<int, QByteArray> result = QAbstractListModel::roleNames();
  result.insert(Qt::UserRole, "path");
  result.insert(Qt::UserRole + 1, "text");
  return result;
}

QGeoPath PathModel::get(int index) const
{
  return _data.at(index);
}
