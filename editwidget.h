#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
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
    QFileDialog* fileDialog;
    void open_file();
    QString fileName;
};

#endif // EDITWIDGET_H
