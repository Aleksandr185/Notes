#include "popupwidget.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QFile>

#include <QDebug>

PopupWidget::PopupWidget(QWidget *parent)
    : QWidget{parent}
{
    _edit = new QLineEdit();
    _completer = new QCompleter(this);
    _model = new QStringListModel(this);
    setFocusProxy(_edit);

    QVBoxLayout* lay = new QVBoxLayout();
    lay->addWidget(_edit);
    lay->setContentsMargins(0, 0, 0, 0);
    setLayout(lay);

    initModel();

    _completer->setModel(_model);
    _edit->setCompleter(_completer);
    QMetaObject::invokeMethod(_completer, "complete", Qt::QueuedConnection);
}

void PopupWidget::initModel()
{
    QFile file(":/files/appointments.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "can't read file!" << file.errorString();
        return;
    }

    QStringList list;
    while (!file.atEnd()) {
        list.append( QString::fromUtf8(file.readLine()) );
    }

    _model->setStringList(list);
}
