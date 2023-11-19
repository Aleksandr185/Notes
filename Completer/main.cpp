#include <QApplication>

#include "widget.h"
#include "popupwidget.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  PopupWidget w;
  w.show();
  w.setFocus();
  return a.exec();
}
