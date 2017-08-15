#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QTimer>
#include <QDebug>
#include <QTime>
namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();
public slots:
    void on_formatButton_clicked();
    void set_pro_value();
private:
    Ui::About *ui;
    int test;//用于测试
    QTimer *timer;
};

#endif // ABOUT_H
