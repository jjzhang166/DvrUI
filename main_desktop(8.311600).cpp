#include "main_desktop.h"
#include "ui_main_desktop.h"
#include <QDesktopWidget>
//#define USE_AW_360
//#define AAA
//#define SUPPORT_RTSP 1
#define HAVA_TWO_CAMERA 1  //While move to cfg
#define CAMERA_FONT 0  //While move to cfg
#define CAMERA_BACK 1 //While move to cfg
#define LOG_BUF_SIZE	1024
#define VIEW_WEITH 1024
#define VIEW_HEIGHT 600
main_desktop *pstaticthis=NULL;
#if defined(Q_OS_LINUX)
sdk_log_setlevel(3);
using namespace android;
void usernotifyCallback(int32_t msgType, int32_t ext1,
                        int32_t ext2, void* user)
{
//printf("cb------------------%p",user);
    if ((msgType&CAMERA_MSG_ERROR) ==CAMERA_MSG_ERROR)
    {
        ALOGE("(msgType =CAMERA_MSG_ERROR)");

    }

    if ((msgType&CAMERA_MSG_DVR_STORE_ERR) ==CAMERA_MSG_DVR_STORE_ERR)
    {
        ALOGE("msgType =CAMERA_MSG_DVR_STORE_ERR)");

        dvr_factory *p_dvr=(dvr_factory *)user;
        p_dvr->storage_state=0;//tmp
        p_dvr->stopRecord();
        if(pstaticthis!=NULL)
        {
            if(p_dvr->mCameraId<SUPPORT_CAMERA_NUM)
            {
                if(pstaticthis->dvrCamera[p_dvr->mCameraId].getRecord())
                    pstaticthis->dvrCamera[p_dvr->mCameraId].setRecord(false);//this is needed ,usrs must sync this ctrl status by self
            }else
            {
                int i;
                for(i=0;i<SUPPORT_CAMERA_NUM;i++){
                    if(p_dvr->mCameraId==pstaticthis->dvrCamera[i].dvr->mCameraId){
                        if(pstaticthis->dvrCamera[i].getRecord())
                            pstaticthis->dvrCamera[i].setRecord(false);//this is needed ,usrs must sync this ctrl status by self
                    }
                }
            }
        }

    }

//return NO_ERROR;

}
void userdataCallback(int32_t msgType,
                      char* dataPtr, camera_frame_metadata_t *metadata, void* user)
{
    //return NO_ERROR;
}

void userdataCallbackTimestamp(nsecs_t timestamp,
                               int32_t msgType, char* dataPtr, void* user) {}


status_t usr_h264datacb(int32_t msgType,
                        char *dataPtr,int dalen,
                        void *user)
{

}
#endif

main_desktop::main_desktop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::main_desktop)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    //设置窗口为固定大小
//    this->setMaximumSize(610,215);
//    this->setMinimumSize(610,215);
    isLocked=false;
    //设置按钮图标
    //对于当点击图标后按钮的变化的效果可以使用多张图片的方式进行
//    ui->cameraButton->setStyleSheet(tr("background-image: url(:/image/image/camera.png);","background-color:transparent"));
//    ui->camera_change_Button->setStyleSheet(tr("background-image: url(:/image/image/change.png);"));
//    ui->lockButton->setStyleSheet(tr("background-image: url(:/image/image/lock.png);"));
//    ui->movieButton->setStyleSheet(tr("background-image: url(:/image/image/movie1.png);"));
//    ui->recordButton->setStyleSheet(tr("background-image: url(:/image/image/record.png);"));
//    ui->setFirstButton->setStyleSheet(tr("background-image: url(:/image/image/setting.png);"));

    //设置系统时间和显示
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(10);
    QFont ft;
    ft.setPointSize(12);
    ft.setBold(true);
    ft.setFamily("Microft YaHei");
    ui->time_Label->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    ui->time_Label->setPalette(pa);
    ui->cameraView->setHidden(true);
    //摄像头数据显示cameraView
    connect(ui->camera_change_Button,SIGNAL(clicked()),this,SLOT(cameraChange()),Qt::UniqueConnection);
//    ui->cameraView->setStyleSheet(tr("background-image: url(:/image/image/picture.png);"));//初始化为前置
    cameraState=false;//点击切换后切换到后置

    //一段时间没有操作后自动隐藏图标
    setButtonVisible();
    setMouseTracking(true);
    mouseMoveTime = new QTimer();
    connect(mouseMoveTime,SIGNAL(timeout()),this,SLOT(on_mouse_no_active_10_second()));
    mouseMoveTime->start(8000);

    //调出其它界面的信号
    connect(ui->setFirstButton,SIGNAL(clicked()),this,SLOT(show_settingDesk()));
    //connect(ui->cameraButton,SIGNAL(clicked(bool)),this,SLOT(show_photoDesk()),on_cameraButton_clicked());
    connect(ui->movieButton,SIGNAL(clicked()),this,SLOT(show_movieDesk()));
    connect(ui->compassButton,SIGNAL(clicked()),this,SLOT(show_dashboard()));


#ifdef QT_ROTATE_APP
    QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsView *view = new QGraphicsView(parent);
    view->setGeometry(0,0,800,480); // actual Display size
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGraphicsProxyWidget *proxy = scene->addWidget(this);
    view->setScene(scene);
    view->show();
    view->rotate(180);
#endif
    setProperty("noinput",true);
    isaf=false;
#ifdef AAA
    setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
#endif
    #if defined(Q_OS_LINUX)

    HwDisplay* mcd=NULL;
    //test screen mode
    //0 disable
    int tvmode=0;
    //config_set_tvout(0,tvmode);
    mcd=HwDisplay::getInstance();
    tvmode=config_get_tvout(0);
    printf("--------------tvmode =%d \r\n",tvmode);
    mcd->hwd_screen1_mode(tvmode);
    //mcd->hwd_tvout();
    cur_camera = 0 ;
    pgEmulatedCameraFactory=new HALCameraFactory();
    //sleep(4);
    int i;
    for(i=0;i<SUPPORT_CAMERA_NUM;i++)
        dvrCamera[i].dvr=new dvr_factory();

#endif
    printf("Play----------%p----\r\n",this);
    pstaticthis=this;//fucking bad,single obj only
}
//设置窗口为透明的，重载了paintEvent
void main_desktop::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(0, 0, 10, 90));  //QColor最后一个参数80代表背景的透明度
}

void main_desktop::startRecord()
{
#if defined(Q_OS_LINUX)
    //int cycltime;
    char bufname[512];
   // int cx,cy;
    printf("startRecord--------------\r\n");
    Mutex::Autolock locker(&mObjectLock);

    dvr_factory * p_dvr;//=dvrCamera[i].dvr;
    int i;
    for(i=0;i<SUPPORT_CAMERA_NUM;i++)
    {
        p_dvr=dvrCamera[i].dvr;
        printf("get camera startup flag=%d \r\n",config_get_startup(i));
        if(1==config_get_startup(i))
        {
            if(dvrCamera[i].getPreview()){
                p_dvr->startRecord();
                isaf=true;
                dvrCamera[i].setRecord(true);
            }else{
                int rt ;
                if(dvrCamera[i].getRecord())
                 {continue ;}
            #ifdef USE_AW_360
                //config_set_heigth(360,960);
                //config_set_weith(360,1440);
                rt= p_dvr->recordInit("360");
            #else
                //config_set_heigth(0,720);
                //config_set_weith(0,1280);
                sprintf(bufname,"%d",i);
                rt= p_dvr->recordInit("bufname");
            #endif
                if(rt <0){
                    printf("init record fail camera[%s] ret =%d \r\n",bufname,rt);
                    continue;
                }
                p_dvr->SetDataCB(usr_h264datacb,p_dvr);
                p_dvr->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,p_dvr/* must dvr obj*/ /*dump*/);
//                p_dvr->prepare();
                p_dvr->start();
                p_dvr->enableWaterMark();
                sprintf(bufname,"64,64,0,64,250,T3L SDK,64,450,ASTEROID V1 alpha");
                p_dvr->setWaterMarkMultiple(bufname);
        #ifdef SUPPORT_RTSP
                p_dvr->set_rtsp_to_file(0);//test !!!! if 1 wirite to encode stream to file . must start before start_rtsp

                p_dvr->start_rtsp();
        #endif
                p_dvr->startRecord();
                isaf = true;
                dvrCamera[i].setRecord(true);
            }
        }
    }

#endif
}
int main_desktop::startAllCameraWithPreview(int camera_no /* nouse now*/)
{
    //sp<CameraHardwareInterface>     mHardware;
    printf("startAllCameraWithPreview play %p\r\n",this);
    camera_no=camera_no;

#if defined(Q_OS_LINUX)
   startRecord();
   printf("startAllCameraWithPreview play %d\r\n",cur_camera);
   dvr_factory *p_dvr=dvrCamera[cur_camera].dvr;

    if((!dvrCamera[cur_camera].getPreview())&&(dvrCamera[cur_camera].getRecord()))
    {
        struct view_info vv= {0,0,VIEW_WEITH,VIEW_HEIGHT};
        //ALOGV("vx=%d vy=%d sx=%d sy%d",mcd->lcdxres,mcd->lcdyres,dvr->recordwith,dvr->recordheith);
        p_dvr->startPriview(vv);
        #ifdef AAA
        setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
        #endif
        dvrCamera[cur_camera].setPreview(true);
    }

#endif
    return 0;
}
//前后摄像头切换
void main_desktop::cameraChange(void)
{
#if defined(Q_OS_LINUX)
    printf("--------------------------------begin change camera\n");
    if(SUPPORT_CAMERA_NUM<2)
    {
        printf("------------------------------only have one camera\n");
        return;
    }
    printf("------------------------current camera is %d\n",cur_camera);
    dvr_factory * p_dvr=dvrCamera[cur_camera].dvr;
    if(dvrCamera[cur_camera].getPreview()){
        p_dvr->stopPriview();
        printf("------------------------stop current camera %d\n",cur_camera);
        dvrCamera[cur_camera].setPreview(false);
    }
    cur_camera = (cur_camera==0)?1:0;
   printf("------------------------current camera changed as %d\n",cur_camera);
    //int cycltime;
    char bufname[512];
    //int cx,cy;
    bool rt;
    p_dvr=dvrCamera[cur_camera].dvr;
    if(!dvrCamera[cur_camera].getRecord())
    {
        printf("--------------------------------------set current camera %d getRecord\n",cur_camera);
           int rt ;
        #ifdef USE_AW_360
            config_set_heigth(360,960);
            config_set_weith(360,1440);
            rt= p_dvr->recordInit("360");
        #else
            //config_set_heigth(0,720);
            //config_set_weith(0,1280);
            sprintf(bufname,"%d",cur_camera);
            rt= p_dvr->recordInit(bufname);
        #endif
            if(rt <0)
                return;
            p_dvr->SetDataCB(usr_h264datacb,p_dvr);
            p_dvr->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,p_dvr/* must dvr obj*/ /*dump*/);
//            p_dvr->prepare();
            p_dvr->start();
            p_dvr->enableWaterMark();
            sprintf(bufname,"64,64,0,64,250,T2L SDK,64,450,RAINBOW V0.3a");
            p_dvr->setWaterMarkMultiple(bufname);
    #ifdef SUPPORT_RTSP
            p_dvr->set_rtsp_to_file(0);//test !!!! if 1 wirite to encode stream to file . must start before start_rtsp

            p_dvr->start_rtsp();
    #endif
            p_dvr->startRecord();
            isaf = true;
            dvrCamera[i].setRecord(true);
    }
    if(!dvrCamera[cur_camera].getPreview())
    {
        printf("--------------------------------------set current camera %d getPreview\n",cur_camera);
        struct view_info vv= {0,0,VIEW_WEITH,VIEW_HEIGHT};
        //ALOGV("vx=%d vy=%d sx=%d sy%d",mcd->lcdxres,mcd->lcdyres,dvr->recordwith,dvr->recordheith);
        p_dvr->startPriview(vv);
        #ifdef AAA
        setStyleSheet(QStringLiteral("background-color: rgba(112, 200, 11, 0);"));
        #endif

        dvrCamera[cur_camera].setPreview(true);
    }
    sleep(3);//temp add here,must use timestamp for response gap,i will impl next version
#else
    //如果现在是前置->切换为后置
    //如果为后置->切换为前置
    if(cameraState==true)
    {
        qDebug()<<"按下1";
        ui->cameraView->setStyleSheet(tr("background-image: url(:/picture.png);"));
        cameraState=false;
    }
    else
    {
        qDebug()<<"按下2";
        ui->cameraView->setStyleSheet(tr("background-image: url(:/picture1.png);"));
        cameraState=true;
    }
#endif
}

main_desktop::~main_desktop()
{
    delete ui;
    #if defined(Q_OS_LINUX)
    //delete dvr;
    int i;
    for(i=0;i<SUPPORT_CAMERA_NUM;i++)
       delete dvrCamera[i].dvr;
    delete pgEmulatedCameraFactory;
    #endif
}
void main_desktop::timerUpdate(void)
{
    QDateTime current_time = QDateTime::currentDateTime();//获取系统现在的时间
    QString time= current_time.toString("hh:mm:ss ddd"); //设置显示格式
    ui->time_Label->setText(time);//在标签上显示时间
}
//无操作时隐藏图标
void main_desktop::on_mouse_no_active_10_second()
{
    setButtonDisvisible();
}
//有操作出现
void main_desktop::on_mouse_active()
{
    mouseMoveTime->stop();
    mouseMoveTime->start();

    setButtonVisible();
}
//重写QWidget的4个方法来保证有动作时显示，没动作时隐藏
void main_desktop::mouseDoubleClickEvent(QMouseEvent *)
{
    on_mouse_active();
}
void main_desktop::mouseMoveEvent(QMouseEvent *)
{
    on_mouse_active();
}
void main_desktop::mousePressEvent(QMouseEvent *)
{
    on_mouse_active();
}
void main_desktop::mouseReleaseEvent(QMouseEvent *)
{
    on_mouse_active();
}

//打开新的界面
void main_desktop::show_settingDesk()
{
    setting_desktop=new SetFirst();
//    connect(setting_desktop,SIGNAL(send_data_to_main(results)),this,SLOT(recieve_setting_data(results)));
    setting_desktop->exec();

}
////暂时做成截图功能
//void main_desktop::show_photoDesk()
//{
//    qDebug()<<"修改为截图功能";
//}
void main_desktop::show_movieDesk()
{
    qDebug()<<"open movie";
    moviedesk=new movieDesk();
    moviedesk->exec();
}
void main_desktop::show_dashboard()
{
    dashboards=new dashBoard(this);//this的目的是将两级窗口设置为父子窗口
    QPalette pal(dashboards->palette());
    pal.setColor(QPalette::Background, Qt::black); //设置背景黑色

    dashboards->setAutoFillBackground(true);
    dashboards->setPalette(pal);

    dashboards->showNormal();
}
//截图的方法
void main_desktop::on_cameraButton_clicked()
{
#if defined(Q_OS_LINUX)
    printf("----------------------take pic\n");
    Mutex::Autolock locker(&mObjectLock);
    if(dvrCamera[cur_camera].getRecord())
    {
        status_t rt=dvrCamera[cur_camera].dvr->takePicture();
        if(NO_ERROR!=rt)
            QMessageBox::information(this, tr("拍照"), tr("拍照失败!"),QMessageBox::Ok);
        else
        {
            QMessageBox* box = new QMessageBox;
            QTimer::singleShot(1000, box, SLOT(close()));

            box->setWindowTitle(tr("takepic"));
            box->setStyleSheet(QStringLiteral("background-color: rgba(0, 200, 11,0);"));
//            box->setIcon(QMessageBox::information);
            box->setText(tr("pic success"));
            box->move(300,240);
            box->show();
        }
    }
#else
//    screenshot_pic=QScreen::grabWindow(this,0,0,600,200);
    setButtonDisvisible();
    ui->cameraButton->setVisible(false);
    QScreen * pqscreen  = QGuiApplication::primaryScreen() ;
    QPixmap pixmap = pqscreen->grabWindow( QApplication::activeWindow()->winId(), -2,-2,QApplication::activeWindow()->width() + 1, QApplication::activeWindow()->height() + 1);

    //截图保存
    QString filename=QDateTime::currentDateTime().toString("yyyy-MM-ddHH-mm-ss")+".bmp";
    if(pixmap.isNull()){
        QMessageBox::information(this,tr("错误"),"截屏失败！",QMessageBox::Ok);
    }else{
        if(!pixmap.save("E:/tech_practise/DvrUI/DvrUI/screen/"+filename,"BMP")){
            QMessageBox::information(this,tr("错误"),"无法保存！",QMessageBox::Ok);
        }else{
            QMessageBox::information(this,tr("成功"),"保存成功！",QMessageBox::Ok);
        }
    }
    setButtonVisible();
#endif
}
//锁定屏幕，使用锁定屏幕所有按钮的方法
void main_desktop::on_lockButton_clicked()
{
    if(isLocked==false){
        ui->cameraButton->setEnabled(false);
        ui->camera_change_Button->setEnabled(false);
        ui->movieButton->setEnabled(false);
        ui->recordButton->setEnabled(false);
        ui->setFirstButton->setEnabled(false);
        ui->compassButton->setEnabled(false);

        isLocked=true;
    }else{
        ui->cameraButton->setEnabled(true);
        ui->camera_change_Button->setEnabled(true);
        ui->movieButton->setEnabled(true);
        ui->recordButton->setEnabled(true);
        ui->setFirstButton->setEnabled(true);
        ui->compassButton->setEnabled(true);
        setButtonVisible();
        isLocked=false;
    }
}

void main_desktop::on_recordButton_clicked()
{
    QMessageBox::information(this,tr("提示"),tr("start recording！"),QMessageBox::Yes);
}

void main_desktop::setButtonVisible()
{
    ui->cameraButton->setVisible(true);
    ui->camera_change_Button->setVisible(true);
    ui->lockButton->setVisible(true);
    ui->movieButton->setVisible(true);
    ui->recordButton->setVisible(true);
    ui->setFirstButton->setVisible(true);
    ui->compassButton->setVisible(true);
}
void main_desktop::setButtonDisvisible()
{
    ui->cameraButton->setVisible(false);
    ui->camera_change_Button->setVisible(false);
    ui->lockButton->setVisible(false);
    ui->movieButton->setVisible(false);
    ui->recordButton->setVisible(false);
    ui->setFirstButton->setVisible(false);
    ui->compassButton->setVisible(false);
}


