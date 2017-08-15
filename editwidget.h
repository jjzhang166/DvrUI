#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QDialog>

namespace Ui {
class editWidget;
}

class editWidget : public QDialog
{
    Q_OBJECT

public:
    explicit editWidget(QWidget *parent = 0);
    ~editWidget();

private:
    Ui::editWidget *ui;
};

#endif // EDITWIDGET_H
