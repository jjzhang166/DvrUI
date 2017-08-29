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

#if defined(Q_OS_LINUX)//行车记录模块视频显示
//文件在sdk文件夹下
#include "CameraDebug.h"
#include <sdklog.h>
#include "V4L2CameraDevice2.h"
#include "CallbackNotifier.h"
#include "PreviewWindow.h"
#include "CameraHardware2.h"
#include "HALCameraFactory.h"
#include "CameraHardwareInterface.h"
#include "DvrFactory.h"
#include "CameraFileCfg.h"
using namespace android;
#endif

namespace Ui {
class main_desktop;
}
#if defined(Q_OS_LINUX)
#define SUPPORT_CAMERA_NUM 2
//char *cameraname[SUPPORT_CAMERA_NUM]={"0","1"};
class DvrCamera
{
public:
    dvr_factory *dvr;

    int setRecord(bool flag ){isRecord=flag;return 0;}
    int setPaused(bool flag){isPaused=flag;return 0;}
    int setPreview(bool flag){isPreview=flag;return 0;}

    bool getRecord(){return isRecord;}
    bool getPaused(){return isPaused;}
    bool getPreview(){return isPreview;}
    DvrCamera()
    {
        isRecord=false;
        isPaused=false;
        isPreview=false;
    }
 private:
    bool isRecord;
    bool isPaused;
    bool isPreview;
};
#endif
class main_desktop : public QWidget
{
    Q_OBJECT

public:
    explicit main_desktop(QWidget *parent = 0);
    ~main_desktop();
protected:
    void paintEvent(QPaintEvent *event);
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
private:
    void setButtonVisible();
    void setButtonDisvisible();


//摄像头数据显示部分
public:
#if defined(Q_OS_LINUX)
//sp<CameraHardwareInterface>     mHardware;
//dvr_factory *dvr;
//dvr_factory *dvr1;
struct view_info mvv;
DvrCamera dvrCamera[SUPPORT_CAMERA_NUM];
int setCameraDispRect(){
    //mvv.x=
    //mvv.y=
    //mvv.w=
    //mvv.h=
    return 0;
}
#endif
    int  startAllCameraWithPreview(int camera_no /* nouse now*/);
    int cur_camera;
    void videoshow()
    {
        //on_pushButton_play_clicked();
        startAllCameraWithPreview(0);
    }
    void startCurCamera()
    {
        //on_pushButton_play_clicked();
    }
    bool isaf;//什么作用

    #if defined(Q_OS_LINUX)
    Mutex                           mObjectLock;
    #endif
    void startRecord();
    void stopRecord(){
        //on_pushButton_stop_clicked();
    }
};

#endif // MAIN_DESKTOP_H

