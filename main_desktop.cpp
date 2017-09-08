#include "main_desktop.h"
#include "ui_main_desktop.h"
#include <QDesktopWidget>
#include "frmmessagebox.h"
#include "videowidget.h"
#if defined(Q_OS_LINUX)
dvr_factory* pdvr;
dvr_factory* pdvr1;
#endif
int g_recordstatus=0;

main_desktop* pStaticMaindesktop=NULL;
#if defined(Q_OS_LINUX)
using namespace android;
void usernotifyCallback(int32_t msgType, int32_t ext1, int32_t ext2, void* user){
    LOGE("msgType =%d-----data=%p-----%d)",msgType,user);

    if ((msgType&CAMERA_MSG_ERROR) ==CAMERA_MSG_ERROR)
    {
        LOGE("(msgType =CAMERA_MSG_ERROR)");

    }
    if ((msgType&CAMERA_MSG_DVR_NEW_FILE) ==CAMERA_MSG_DVR_NEW_FILE)
    {
        dvr_factory *p_dvr=(dvr_factory *)user;
        LOGE("(msgType =CAMERA_MSG_DVR_NEW_FILE camera %d idx =%d)",p_dvr->mCameraId,ext1);
    }

    if ((msgType&CAMERA_MSG_DVR_STORE_ERR) ==CAMERA_MSG_DVR_STORE_ERR)
    {
        LOGE("msgType =CAMERA_MSG_DVR_STORE_ERR)");
        dvr_factory *p_dvr=(dvr_factory *)user;
        p_dvr->storage_state=0;//tmp
        p_dvr->stopRecord();
    }
}
void userdataCallback(int32_t msgType,char* dataPtr, camera_frame_metadata_t *metadata, void* user)
{
    //LOGE("userdataCallback=-----------)");
}

void userdataCallbackTimestamp(nsecs_t timestamp, int32_t msgType, char* dataPtr, void* user){
    char *ptp=(char *)dataPtr;
    VencInputBuffer *p=(VencInputBuffer *)(ptp+4);
    // memcpy(inputBuffer.pAddrVirY,(unsigned char*)p->pAddrVirY,encode_param.src_width*encode_param.src_height);
    // memcpy(inputBuffer.pAddrVirC,(unsigned char*)p->pAddrVirY+encode_param.src_width*encode_param.src_height,encode_param.src_width*encode_param.src_height/2);
}

status_t usr_datacb(int32_t msgType, char *dataPtr,int dalen, void *user){
    return 0;
}
#ifdef AUTEVENT_TEST//define
void testaut_event_cb_func(NetlinkEvent *evt,void *usrdata){
    ALOGV("event act=%d %s ",evt->getAction(),evt->getSubsystem());

    int action = evt->getAction();
    const char *devtype = evt->findParam("DEVTYPE");

    if (action == NetlinkEvent::NlActionAdd)
    {
        int major = atoi(evt->findParam("MAJOR"));
        int minor = atoi(evt->findParam("MINOR"));
        char nodepath[255];

        if (!strcmp(devtype, "disk"))
        {
            ALOGW("line=%d,devtype=%s",__LINE__,devtype);
            //handleDiskAdded(dp, evt);
            char bufname[512];
            memset(bufname,0,512);
            char tmpbuf[256];
            char real_path[256];
            config_get_curfiledir(0,bufname);
            int len = strlen(bufname);
            const char *tmp = evt->findParam("NPARTS");
            if (tmp) {
            int npart = atoi(tmp);
            if(npart==0)
            {
                const char *devname = evt->findParam("DEVNAME");

                char buf[256];
                if(strstr(devname,"sd"))
                    sprintf(buf,"mount -t vfat /dev/%s /mnt/usb -o defaults,noatime,async,iocharset=cp936",devname);
                else if(strstr(devname,"mmcblk1"))
                    sprintf(buf,"mount -t vfat /dev/%s /mnt/sdcard/mmcblk1p1 -o defaults,noatime,async,iocharset=cp936",devname);
                else
                    sprintf(buf,"echo mount fail > /dev/ttyS0");
                system(buf);
            }
        } else {
            SLOGW("Kernel block uevent missing 'NPARTS'");
            //mDiskNumParts = 1;
        }
        strcpy(tmpbuf,bufname);
        for(int i=0; i<len; i++)
        {
            if(tmpbuf[len-1-i]=='/')
            {
                tmpbuf[len-1-i]=0;
            }
            else
            {
                break;
            }
        }

        int rmret = readlink(tmpbuf,real_path,sizeof(real_path));
        if(rmret<0)
        {
            ALOGV("mount path not a symlink %s err=%d",tmpbuf,errno);

            strcpy(real_path,tmpbuf);
        }
        else
        {
            ALOGV("mount real path is %s \r\n",real_path);
        }
        for(int i=0;i<6;i++)
        {
             if(isMounted(real_path))
                break;

              sleep(1);
        }
            if(g_recordstatus==0){
                pdvr->startRecord();
                pdvr1->startRecord();

            }else
                ALOGE("record status error");
        }

    }
    else if (action == NetlinkEvent::NlActionRemove)
    {
        if (!strcmp(devtype, "disk"))
        {
            if(g_recordstatus==1){
                pdvr->storage_state=0;//tmp
                pdvr->stopRecord();
                pdvr1->storage_state=0;//tmp
                pdvr1->stopRecord();
            }
        }
    }
}
#endif
#endif


main_desktop::main_desktop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::main_desktop)
{
#if defined(Q_OS_LINUX)

    HwDisplay* mcd=NULL;
    int i;
    int tvmode=3;//0 disable ,1 tvout pal ;2 tvout ntsc;3 hdmi 1280x720;4 hdmi 1920x1080
    cur_camera = 0 ;
    //test screen mode
    //0 disable
    //config_set_tvout(0,tvmode);
    int rt=0;
    sdk_log_setlevel(3);
    mcd=HwDisplay::getInstance();

    #ifdef AUTEVENT_TEST//defined
        NetlinkManager *nm;
        if (!(nm = NetlinkManager::Instance())) {
            SLOGE("Unable to create NetlinkManager");
        };
        printf("--------------------------------NetlinkManager done\n");
    #endif
    pgEmulatedCameraFactory=new HALCameraFactory();
    pdvr=new dvr_factory();
    pdvr1=new dvr_factory();
    #ifdef AUDIO_TEST //defined
        //AudioCapThread audiocap;
        sp<AudioCapThread> audiocap=new AudioCapThread();
        int hdl=audiocap->reqAudioSource(pdvr->__audio_data_cb_timestamp,(void *) pdvr);
        pdvr->setAudioOps(audiocap,hdl);
         printf("--------------------------------AudioCapThread done\n");
    #endif
    #ifdef AUTEVENT_TEST//defined
         AutEvent *pevent=AutEvent::Instance();
         pevent->setEventCb(testaut_event_cb_func,(void *)pdvr);
         if (nm->start()) {
             SLOGE("Unable to start NetlinkManager (%s)", strerror(errno));
         }
         //sleep(1000);
        printf("--------------------------------AutEvent done\n");
    #endif
    tvmode=config_get_tvout(TEST_CAMERA_ID);
    printf("--------------tvmode =%d \r\n",tvmode);
    mcd->hwd_screen1_mode(tvmode);
    //mcd->hwd_tvout();
    config_set_startup(TEST_CAMERA_ID,1);
    printf("------------------------------------camera initial done\n");
#endif
    ui->setupUi(this);
    FormInCenter();
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
    cameraState=true;//点击切换后切换到后置

    //一段时间没有操作后自动隐藏图标
    setButtonVisible();
    setMouseTracking(true);
    mouseMoveTime = new QTimer();
    connect(mouseMoveTime,SIGNAL(timeout()),this,SLOT(on_mouse_no_active_10_second()));
    mouseMoveTime->start(8000);

//    setProperty("noinput",true);

    printf("main_desktop----------%p----\r\n",this);
    printf("-------------------------------------construction function done\n");


    pStaticMaindesktop=this;

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
    printf("startRecord--------------\r\n");
    Mutex::Autolock locker(&mObjectLock);
    char bufname[512];
    int rt=0;
    rt=pdvr->recordInit("0");
    rt=pdvr1->recordInit("1");
    pdvr->SetDataCB(usr_datacb,pdvr);
    pdvr->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,pdvr /*dump*/);
//    pdvr->prepare();
    pdvr->start();
    pdvr1->SetDataCB(usr_datacb,pdvr1);
    F_LOG;
    pdvr1->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,pdvr1 /*dump*/);
    F_LOG;
//    pdvr1->prepare();
    pdvr1->start();
    sleep(5);

    pdvr->enableWaterMark();
    sprintf(bufname,"64,64,0,64,250,T3L SDK,64,450,ASTEROID V1 alpha");
    pdvr->setWaterMarkMultiple(bufname);
#endif
}
int main_desktop::startAllCameraWithPreview(int camera_no /* nouse now*/)
{
    //sp<CameraHardwareInterface>     mHardware;
//    printf("startAllCameraWithPreview play %p\r\n",this);
    camera_no=camera_no;

#if defined(Q_OS_LINUX)
   startRecord();
   printf("startAllCameraWithPreview play %d\r\n",cur_camera);
   struct view_info vv= {0,0,1024,600};
   pdvr->startPriview(vv);
   pdvr1->startPriview(vv);
#endif
    return 0;
}


main_desktop::~main_desktop()
{
    delete ui;
    #if defined(Q_OS_LINUX)
    //delete dvr;
    pdvr->enc_de_init();
    F_LOG;
    pdvr->uninitializeDev();
    F_LOG;
    pdvr1->enc_de_init();
    F_LOG;
    pdvr1->uninitializeDev();
    F_LOG;
    delete pdvr;
    delete pdvr1;
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
void main_desktop::on_main_desktop_disvisible()
{
    qDebug()<<"main_desktop will hidden";
    this->setHidden(true);
    #if defined(Q_OS_LINUX)
        pdvr->stopPriview();
        pdvr1->stopPriview();
    #endif
}
void main_desktop::on_main_desktop_visible()
{
    this->setHidden(false);
    this->moviedesk->activateWindow();
    #if defined(Q_OS_LINUX)
        struct view_info vv= {0,0,1024,600};
        pdvr->startPriview(vv);
        pdvr1->startPriview(vv);
    #endif
}
////暂时做成截图功能
//void main_desktop::show_photoDesk()
//{
//    qDebug()<<"修改为截图功能";
//}


//截图的方法
void main_desktop::on_cameraButton_clicked()
{
    if(cameraState)
    {
        qDebug()<<"当前为前摄像头";
    #if defined(Q_OS_LINUX)
        printf("----------------------front camera take pic\n");
        Mutex::Autolock locker(&mObjectLock);
        status_t rt=pdvr->takePicture();
        if(NO_ERROR!=rt){
//            QMessageBox::information(this, tr("拍照"), tr("拍照失败!"),QMessageBox::Ok);
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("拍照失败!")),2);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
        }
        else
        {
//            QMessageBox* box = new QMessageBox;
//            QTimer::singleShot(1000, box, SLOT(close()));

//            box->setWindowTitle(tr("拍照"));
//            box->setStyleSheet(QStringLiteral("background-color: rgba(0, 200, 11,0);"));
//            box->setIconPixmap(QPixmap(":/icon/information.png"));
//            box->setText(tr("拍照成功"));
//            box->move(300,240);
//            box->show();
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("拍照成功!")),0);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();

        }
    #endif
    }else{
        qDebug()<<"当前为后摄像头";
    #if defined(Q_OS_LINUX)
        printf("----------------------rear camera take pic\n");
        Mutex::Autolock locker(&mObjectLock);
        status_t rt=pdvr1->takePicture();
        if(NO_ERROR!=rt){
//            QMessageBox::information(this, tr("拍照"), tr("拍照失败!"),QMessageBox::Ok);
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("拍照失败!")),2);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
        }else{
//            QMessageBox* box = new QMessageBox;
//            QTimer::singleShot(1000, box, SLOT(close()));

//            box->setWindowTitle(tr("拍照"));
//            box->setStyleSheet(QStringLiteral("background-color: rgba(0, 200, 11,0);"));
//            box->setIconPixmap(QPixmap(":/icon/information.png"));
//            box->setText(tr("拍照成功"));
//            box->move(300,240);
//            box->show();
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("拍照成功!")),0);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
        }
    #endif
    }
#if !defined(Q_OS_LINUX)
//在windows下为截图功能
//    screenshot_pic=QScreen::grabWindow(this,0,0,600,200);
    setButtonDisvisible();
    ui->cameraButton->setVisible(false);
    QScreen * pqscreen  = QGuiApplication::primaryScreen() ;
    QPixmap pixmap = pqscreen->grabWindow( QApplication::activeWindow()->winId(), -2,-2,QApplication::activeWindow()->width() + 1, QApplication::activeWindow()->height() + 1);

    //截图保存
    QString filename=QDateTime::currentDateTime().toString("yyyy-MM-ddHH-mm-ss")+".bmp";
    if(pixmap.isNull()){
//        QMessageBox::information(this,tr("错误"),"截屏失败！",QMessageBox::Ok);
        frmMessageBox *msg=new frmMessageBox;
        msg->SetMessage(QString(tr("截屏失败！")),2);
        QTimer::singleShot(1000, msg, SLOT(close()));
        msg->exec();
    }else{
        if(!pixmap.save("E:/tech_practise/DvrUI/DvrUI/screen/"+filename,"BMP")){
//            QMessageBox::information(this,tr("错误"),"无法保存！",QMessageBox::Ok);
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("截屏失败！")),2);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();


        }else{
//            QMessageBox::information(this,tr("成功"),"保存成功！",QMessageBox::Ok);
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("保存成功！")),0);
            QTimer::singleShot(3000, msg, SLOT(close()));
            msg->exec();
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
    if(cameraState)
    {
        qDebug()<<"当前为前摄像头";
    #if defined(Q_OS_LINUX)
        printf("----------------------front camera start recording\n");
        int retrec=pdvr->startRecord();

        if(retrec<0){
            ALOGE("!!start record fail pls insert tf card");
//            QMessageBox::warning(this,tr("提示"),tr("front camera recording error！"),QMessageBox::Yes);
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("前摄像头无法录像!")),2);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
            g_recordstatus=0;
        }else{
            g_recordstatus=1;
//            QMessageBox::information(this,tr("提示"),tr("front camera start recording！"),QMessageBox::Yes);
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("前摄像头开始录像!")),0);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
            pdvr->setDuration(30);//录像时间30s
            sleep(30);//等待录像结束
            pdvr->stopRecord();//结束录像
        }
    #endif
    }else{
        qDebug()<<"当前为后摄像头";
    #if defined(Q_OS_LINUX)
        printf("----------------------rear camera start recording\n");
        int retrec=pdvr1->startRecord();
        if(retrec<0){
            ALOGE("!!start record fail pls insert tf card");
//            QMessageBox::warning(this,tr("提示"),tr("rear camera recording error！"),QMessageBox::Yes);
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("后摄像头无法录像!")),2);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
            g_recordstatus=0;
        }else{
            g_recordstatus=1;
//            QMessageBox::information(this,tr("提示"),tr("rear camera start recording！"),QMessageBox::Yes);
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("后摄像头正在录像!")),0);
//            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();

            pdvr1->setDuration(30);//录像时间30s
            sleep(30);//等待录像结束
            pdvr1->stopRecord();//结束录像
        }
    #endif
    }
#if !defined(Q_OS_LINUX)
    //在windows下
     QMessageBox::information(this,tr("提示"),tr("start recording！"),QMessageBox::Yes);
#endif

}
void main_desktop::FormInCenter()
{
    int frmX = this->width();
    int frmY = this->height();
    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    this->move(movePoint);
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


//前后摄像头切换
void main_desktop::on_camera_change_Button_clicked()
{
    //如果现在是前置->切换为后置
    //如果为后置->切换为前置
    if(!cameraState)
    {
        qDebug()<<"按下1";
    #if defined(Q_OS_LINUX)
        struct view_info vv= {0,0,1024,600};
        pdvr1->stopPriview();
        pdvr->startPriview(vv);
    #else
        ui->cameraView->setStyleSheet(tr("background-image: url(:/picture.png);"));
    #endif
        cameraState=true;
    }
    else
    {
        qDebug()<<"按下2";
    #if defined(Q_OS_LINUX)
        struct view_info vv= {0,0,1024,600};

        pdvr->stopPriview();
        pdvr1->startPriview(vv);
    #else
        ui->cameraView->setStyleSheet(tr("background-image: url(:/picture1.png);"));
    #endif
        cameraState=false;
    }
}
//打开dashborad界面

void main_desktop::on_compassButton_clicked()
{
    dashboards=new dashBoard(this);//this的目的是将两级窗口设置为父子窗口
    QPalette pal(dashboards->palette());
    pal.setColor(QPalette::Background, Qt::black); //设置背景黑色

    dashboards->setAutoFillBackground(true);
    dashboards->setPalette(pal);

    dashboards->showNormal();
}
//打开设置界面
void main_desktop::on_setFirstButton_clicked()
{
    setting_desktop=new SetFirst(this);
//    connect(setting_desktop,SIGNAL(send_data_to_main(results)),this,SLOT(recieve_setting_data(results)));
    setting_desktop->exec();
}
//打开图片视频预览界面
void main_desktop::on_movieButton_clicked()
{
    qDebug()<<"open movie";

    moviedesk=new movieDesk();
    moviedesk->exec();

}
