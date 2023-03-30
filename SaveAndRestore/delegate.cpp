/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "delegate.h"

#include <QPainter>
#include <QApplication>

Delegate::Delegate(QObject* parent)
  :QStyledItemDelegate(parent)
{

}

void Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                     const QModelIndex& index) const
{
  painter->save();

  QPen pen = painter->pen();
  pen.setColor(option.palette.color(QPalette::Active, QPalette::Button));
  painter->setPen(pen);

  // Vertical Lines
  switch (option.viewItemPosition) {
    case QStyleOptionViewItem::Beginning:
    case QStyleOptionViewItem::Middle:
      painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
      break;
    case QStyleOptionViewItem::OnlyOne:
    case QStyleOptionViewItem::End:{
      painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
      painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
    }
      break;
    default:
      break;
  }

  // Horizontal Lines
  painter->drawLine(option.rect.topLeft(), option.rect.topRight());

  // Last line
  if (index.isValid() && index.row() == index.model()->rowCount(index.parent()) - 1)
    painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());


  painter->restore();

  QStyledItemDelegate::paint(painter, option, index);
}

QSize Delegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  // src QStyledItemDelegate::sizeHint
//  QVariant value = index.data(Qt::SizeHintRole);
//  if (value.isValid())
//      return qvariant_cast<QSize>(value);

//  QStyleOptionViewItem opt = option;
//  initStyleOption(&opt, index);
//  const QWidget *widget = QStyledItemDelegatePrivate::widget(option);
//  QStyle *style = widget ? widget->style() : QApplication::style();
//  return style->sizeFromContents(QStyle::CT_ItemViewItem, &opt, QSize(), widget);
  // end src

  Q_UNUSED(index)

  const QWidget* widget = option.widget;
  QStyle* style = widget ? widget->style() : QApplication::style();
  QSize size = style->sizeFromContents(QStyle::CT_ItemViewItem, &option, QSize(), widget);
  size.setHeight(size.height() + 2);

  return size;
}
