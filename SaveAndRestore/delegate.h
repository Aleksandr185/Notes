#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

class Delegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit Delegate(QObject* parent = Q_NULLPTR);

  void paint(QPainter *painter,
             const QStyleOptionViewItem &option,
             const QModelIndex &index) const Q_DECL_OVERRIDE;

  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // DELEGATE_H
