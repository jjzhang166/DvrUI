#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QTimer>
#include <QDebug>
#include <QTime>
#if defined(Q_OS_LINUX)
#include "Fat.h"
#define PATH_SDCARD  "/mnt/sdcard/mmcblk1p1/"
#define PATH_SD_DEV  "/dev/mmcblk1p1"
#endif
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
    #if defined(Q_OS_LINUX)
        Fat *pfat;
    #endif
};

#endif // ABOUT_H
