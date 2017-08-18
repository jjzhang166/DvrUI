#ifndef MAIN_DESKTOP_H
#define MAIN_DESKTOP_H

#include <QWidget>
#include <QDebug>
#include <QPixmap>
#include <QDateTime>
#include <QString>
#include <QTimer>
#include <QPalette>
#include <QPixmap>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
//#include <settings.h>
#include "setfirst.h"
#include "dashboard.h"
#include "moviedesk.h"
namespace Ui {
class main_desktop;
}

class main_desktop : public QWidget
{
    Q_OBJECT

public:
    explicit main_desktop(QWidget *parent = 0);
    ~main_desktop();
private://用于检测是否有动作
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event) ;

    void on_mouse_active();
private:
    Ui::main_desktop *ui;
    bool cameraState;//摄像头前后状态：true=前置；false=后置
    bool isLocked;//是否锁定屏幕
    QTimer *mouseMoveTime;//检测鼠标离开的时间
    QPixmap screenshot_pic;
    void accept();
public slots:
    void timerUpdate(void);
    void cameraChange();//前后摄像头切换
private slots://有动作时显示图标
    void on_mouse_no_active_10_second();
    void on_recordButton_clicked();

public slots://其它界面调出
    void show_settingDesk();
    void show_movieDesk();
    void show_photoDesk();
    void show_dashboard();
    void on_cameraButton_clicked();//截图
    void on_lockButton_clicked();//锁屏
private://界面类
    SetFirst *setting_desktop;
    dashBoard *dashboards;
    movieDesk *moviedesk;
//public slots://返回值
//    void recieve_setting_data(results);

};

#endif // MAIN_DESKTOP_H

