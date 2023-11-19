#ifndef POPUPWIDGET_H
#define POPUPWIDGET_H

#include <QWidget>

class QLineEdit;
class QCompleter;
class QStringListModel;

class PopupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PopupWidget(QWidget *parent = nullptr);

signals:

private:
    QLineEdit* _edit;
    QCompleter* _completer;
    QStringListModel* _model;

    void initModel();
};

#endif // POPUPWIDGET_H
