#include "main_desktop.h"
#include "ui_main_desktop.h"
#include <QDesktopWidget>
#include "frmmessagebox.h"
#include "videowidget.h"
#include "reverseline_setting.h"
#include "midwindow.h"
#if defined(os_test)
dvr_factory* pdvr;
dvr_factory* pdvr1;
#endif
QFileInfo fileInfo_to_play;
QFileInfo fileInfo_to_show;
int g_recordstatus=0;

int recordTime=1;//set record time
bool isTrue=true;

#if 0
extern int startA,startB,widthA,heightA;
#endif
int rotate_angle=15;

int open_recordVideo_front,open_recordVideo_rear;
int open_recordAudio_front,open_recordAudio_rear;
int open_reverseLine_front,open_reverseLine_rear;
int open_adas_front,open_adas_rear;
main_desktop* pStaticMaindesktop=NULL;
extern ReverseLine_Setting *pStatic_reverseLine;
QString linux_usb_path="/mnt/usb/sda4/";//usb path init;
QString linux_sdcard_path;//sd卡
MidWindow* midwindow=NULL;
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
                if(strstr(devname,"sd")){
                    sprintf(buf,"mount -t vfat /dev/%s /mnt/usb -o defaults,noatime,async,iocharset=cp936",devname);

                }
                else if(strstr(devname,"mmcblk1")){
                    sprintf(buf,"mount -t vfat /dev/%s /mnt/sdcard/mmcblk1p1 -o defaults,noatime,async,iocharset=cp936",devname);
                }
                else{
                    sprintf(buf,"echo mount fail > /dev/ttyS0");
                }

//                system(buf);
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
//            if(g_recordstatus==0){
//                pdvr->startRecord();
//                pdvr1->startRecord();

//            }else
//                ALOGE("record status error");
        }

    }
    else if (action == NetlinkEvent::NlActionRemove)
    {
//        if (!strcmp(devtype, "disk"))
//        {
//            if(g_recordstatus==1){
//                pdvr->storage_state=0;//tmp
//                pdvr->stopRecord();
//                pdvr1->storage_state=0;//tmp
//                pdvr1->stopRecord();
//            }
//        }
    }
}
#endif
#ifdef AUTEVENT_TEST//detect usb event
void testaut_event_cb_func1(NetlinkEvent *evt,void *usrdata)
{
    qDebug()<<"---------------------callback  testaut_event_cb_func1";
    ALOGV("event act=%d %s ",evt->getAction(),evt->getSubsystem());

    int action = evt->getAction();
    const char *devtype = evt->findParam("DEVTYPE");

    if (action == NetlinkEvent::NlActionAdd)
    {
        qDebug()<<"-----------------some machine is here";
        int major = atoi(evt->findParam("MAJOR"));
        int minor = atoi(evt->findParam("MINOR"));
        if (!strcmp(devtype, "disk"))
        {
            char buf[256];
            const char *devname = evt->findParam("DEVNAME");

            if(strstr(devname,"sd"))
            {
                sprintf(buf,"mount -t vfat /dev/%s /mnt/usb -o defaults,noatime,async,iocharset=cp936",devname);
                qDebug()<<"---------------------------mount the usb";
                midwindow->usb_insert();
                qDebug()<<"midwindow is done";
                linux_usb_path=QString("/mnt/usb/"+QString(QLatin1String(devname))+"4/");
                qDebug()<<"main_desktop u-disk is mounted in"<<linux_usb_path;
            }
            else if(strstr(devname,"mmcblk1")){
                sprintf(buf,"mount -t vfat /dev/%s /mnt/sdcard/mmcblk1p1 -o defaults,noatime,async,iocharset=cp936",devname);
                qDebug()<<"---------------------------mount the sdcard";
            }else
            {
                sprintf(buf,"echo mount fail > /dev/ttyS0");
                qDebug()<<"---------------------------mount the other";
            }
        }
    }
    else if (action == NetlinkEvent::NlActionRemove)
    {
        qDebug()<<"-----------------some machine is not here";
        if (!strcmp(devtype, "disk"))
        {
            char buf[256];
            const char *devname = evt->findParam("DEVNAME");
            if(strstr(devname,"sd"))
            {
                qDebug()<<"-----------usb is out of machine";
                midwindow->usb_delete();
                system("cd /");
                system("umount -f /mnt/usb/sd*");
                system("rm -rf /mnt/usb");
                qDebug()<<"midwindow is done";
            }
            else if(strstr(devname,"mmcblk1")){
                qDebug()<<"usb is out of machine";
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
#if defined(os_test)

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
    pgEmulatedCameraFactory=new HALCameraFactory();//实例化cameraHardware和v4l2cameradevice
    pdvr=new dvr_factory();
    pdvr1=new dvr_factory();
    #ifdef AUDIO_TEST //defined
        //AudioCapThread audiocap;
        sp<AudioCapThread> audiocap=new AudioCapThread();
        int hdl=audiocap->reqAudioSource(pdvr->__audio_data_cb_timestamp,(void *) pdvr);
        pdvr->setAudioOps(audiocap,hdl);

        sp<AudioCapThread> audiocap1=new AudioCapThread();
        int hdl1=audiocap1->reqAudioSource(pdvr1->__audio_data_cb_timestamp,(void *) pdvr1);
        pdvr1->setAudioOps(audiocap1,hdl1);
         printf("--------------------------------AudioCapThread done\n");
    #endif
    #ifdef AUTEVENT_TEST//defined
         AutEvent *pevent=AutEvent::Instance();
         pevent->setEventCb(testaut_event_cb_func,(void *)pdvr);
         qDebug()<<"------------------------ready to callback";
         pevent->setEventCb(testaut_event_cb_func1,(void *)0);
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
    config_set_startup(1,1);
    //if start record when machine is start
    //read config.ini
#if 0
    open_recordVideo_front=config_get_recordVideo(0);
    open_recordAudio_front=config_get_recordAudio(0);
    open_reverseLine_front=config_get_reverseLine(0);
    open_adas_front=config_get_adas(0);
#endif
//    if(open_recordVideo_front){
//        printf("----------------start record when machine is start\n");
//        pdvr->startRecord();
//        pdvr->setDuration(recordTime*60);

//
//    }
    printf("------------------------------------camera initial done\n");
#endif
    ui->setupUi(this);
    pStaticMaindesktop=this;


    FormInCenter();
    setAttribute(Qt::WA_TranslucentBackground, true);


//    this->setMaximumSize(610,215);
//    this->setMinimumSize(610,215);



//    ui->cameraButton->setStyleSheet(tr("background-image: url(:/image/image/camera.png);","background-color:transparent"));
//    ui->camera_change_Button->setStyleSheet(tr("background-image: url(:/image/image/change.png);"));
//    ui->lockButton->setStyleSheet(tr("background-image: url(:/image/image/lock.png);"));
//    ui->movieButton->setStyleSheet(tr("background-image: url(:/image/image/movie1.png);"));
//    ui->recordButton->setStyleSheet(tr("background-image: url(:/image/image/record.png);"));
//    ui->setFirstButton->setStyleSheet(tr("background-image: url(:/image/image/setting.png);"));


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
//    ui->cameraView->setHidden(true);

    cameraState=true;


    setButtonVisible();
    setMouseTracking(true);
    mouseMoveTime = new QTimer();
    connect(mouseMoveTime,SIGNAL(timeout()),this,SLOT(on_mouse_no_active_10_second()));
    mouseMoveTime->start(8000);

//    setProperty("noinput",true);
#if !defined(os_test)
    open_reverseLine_front=1;
#endif
    reverseLinewidget=new reverseLineWidget();
    if(open_reverseLine_front)
    {
         reverseLinewidget->setParent(ui->widget);
    }

    printf("main_desktop----------%p----\r\n",this);
    printf("-------------------------------------construction function done\n");
    midwindow=new MidWindow();
    setting_desktop=new SetFirst(this);
    moviedesk=new movieDesk1(this);

    setting_desktop->FormInCenter();
    moviedesk->FormInCenter();

//    QPixmap pixmap(":/icon/lock.png");
//    QPixmap fitpixmap=pixmap.scaled(81,81,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    ui->lockButton->setIcon(QIcon(fitpixmap));
//    ui->lockButton->setFlat(true);
//    ui->lockButton->setStyleSheet("border:0px");
    connect(moviedesk,SIGNAL(main_desktop_disvisible()),this,SLOT(on_main_desktop_disvisible()));
//    connect(pStatic_reverseLine,SIGNAL(reverseLine_repaint()),this,SLOT(on_reverseLine_repaint()));

}

void main_desktop::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(0, 0, 10, 90));
}

void main_desktop::startRecord()
{
#if defined(os_test)
    printf("startRecord--------------\r\n");
    Mutex::Autolock locker(&mObjectLock);
    char bufname[512];
    int rt=0;
    qDebug()<<"-------------------------------initing";
    rt=pdvr->recordInit("0");
    rt=pdvr1->recordInit("1");
    pdvr->SetDataCB(usr_datacb,pdvr);
    pdvr->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,pdvr /*dump*/);
    pdvr->start();
    pdvr1->SetDataCB(usr_datacb,pdvr1);
    F_LOG;
    pdvr1->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,pdvr1 /*dump*/);
    F_LOG;
    pdvr1->start();
//    sleep(5);
    qDebug()<<"-----------------------------initing done";
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

#if defined(os_test)
   startRecord();
   printf("startAllCameraWithPreview play %d\r\n",cur_camera);
#if 0
   startA=0;startB=150;widthA=1024;heightA=300;
#endif
   struct view_info vv= {0,0,1024,600};
   pdvr->startPriview(vv);
   pdvr1->startPriview(vv);
#endif
    return 0;
}


main_desktop::~main_desktop()
{
    delete ui;
    delete setting_desktop;
    delete moviedesk;
    delete reverseLinewidget;
    #if defined(os_test)
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
    QDateTime current_time = QDateTime::currentDateTime();
    QString time= current_time.toString("hh:mm:ss ddd");
    ui->time_Label->setText(time);
}

void main_desktop::on_mouse_no_active_10_second()
{
    setButtonDisvisible();
    ui->widget->move(QPoint(10,71));
}

void main_desktop::on_mouse_active()
{
    mouseMoveTime->stop();
    mouseMoveTime->start();

    setButtonVisible();
}

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
    #if defined(os_test)
        pdvr->stopPriview();
        pdvr1->stopPriview();
    #endif
}
void main_desktop::on_main_desktop_visible()
{
    this->setHidden(false);
    this->moviedesk->activateWindow();
    #if defined(os_test)
        struct view_info vv= {0,0,1024,600};
        pdvr->startPriview(vv);
        pdvr1->startPriview(vv);
    #endif
}


void main_desktop::on_cameraButton_clicked()
{
    if(cameraState)
    {
        qDebug()<<"now is front camera";
    #if defined(os_test)
        printf("----------------------front camera take pic\n");
        Mutex::Autolock locker(&mObjectLock);
        status_t rt=pdvr->takePicture();
        if(NO_ERROR!=rt){
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("take pic fail!")),2);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
        }
        else
        {
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("take pic success!")),0);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
        }
    #endif
    }else{
        qDebug()<<"now is rear camera";
    #if defined(os_test)
        printf("----------------------rear camera take pic\n");
        Mutex::Autolock locker(&mObjectLock);
        status_t rt=pdvr1->takePicture();
        if(NO_ERROR!=rt){
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("take pic fail!")),2);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
        }else{
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("take pic success!")),0);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
        }
    #endif
    }

}



void main_desktop::on_recordButton_clicked()
{
    if(cameraState)
    {
        qDebug()<<"now is front camera";
    #if defined(os_test)
        printf("----------------------front camera start recording\n");
        int retrec=pdvr->startRecord();

        if(retrec<0){
            ALOGE("!!start record fail pls insert tf card");
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("Please insert sdcard!")),2);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
            g_recordstatus=0;
        }else{
            g_recordstatus=1;
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("front camera start record!")),0);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
            pdvr->setDuration(recordTime*60);
            pdvr->stopRecord();
        }
    #endif
    }else{
        qDebug()<<"now is rear camera";
    #if defined(os_test)
        printf("----------------------rear camera start recording\n");
        int retrec=pdvr1->startRecord();
        if(retrec<0){
            ALOGE("!!start record fail pls insert tf card");

            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("Please insert sdcard!")),2);
            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();
            g_recordstatus=0;
        }else{
            g_recordstatus=1;

            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString(tr("rear camera is recording!")),0);
//            QTimer::singleShot(1000, msg, SLOT(close()));
            msg->exec();

            pdvr1->setDuration(recordTime*60);
            pdvr1->stopRecord();
        }
    #endif
    }


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
//    ui->cameraButton->setVisible(true);
//    ui->camera_change_Button->setVisible(true);
//    ui->lockButton->setVisible(true);
//    ui->movieButton->setVisible(true);
//    ui->recordButton->setVisible(true);
//    ui->setFirstButton->setVisible(true);
//    ui->compassButton->setVisible(true);
    ui->widget_2->setVisible(true);
}
void main_desktop::setButtonDisvisible()
{
//    ui->cameraButton->setVisible(false);
//    ui->camera_change_Button->setVisible(false);
//    ui->lockButton->setVisible(false);
//    ui->movieButton->setVisible(false);
//    ui->recordButton->setVisible(false);
//    ui->setFirstButton->setVisible(false);
//    ui->compassButton->setVisible(false);
    ui->widget_2->setVisible(false);
}



void main_desktop::on_camera_change_Button_clicked()
{

    if(!cameraState)
    {

    #if defined(os_test)
        struct view_info vv= {0,0,1024,600};
        pdvr1->stopPriview();
        pdvr->startPriview(vv);
    #else
//        ui->cameraView->setStyleSheet(tr("background-image: url(:/picture.png);"));
    #endif
        cameraState=true;
    }
    else
    {
    #if defined(os_test)
        struct view_info vv= {0,0,1024,600};

        pdvr->stopPriview();
        pdvr1->startPriview(vv);
    #else
//        ui->cameraView->setStyleSheet(tr("background-image: url(:/picture1.png);"));
    #endif
        cameraState=false;
    }
}

void main_desktop::on_setFirstButton_clicked()
{
    setting_desktop->exec();
}

void main_desktop::on_movieButton_clicked()
{
    qDebug()<<"open movie";
    moviedesk->show();
}
void main_desktop::on_reverseLine_repaint()
{
    reverseLinewidget->update();
}

void main_desktop::on_pushButton_clicked()
{
    if(isTrue){
#if defined(os_test)
    pdvr->stopPriview();
    pdvr1->stopPriview();
#endif
    isTrue=false;
    }else{
#if defined(os_test)
    struct view_info vv= {0,0,1024,600};
    pdvr->startPriview(vv);
    pdvr1->startPriview(vv);
#endif
    isTrue=true;
    }
}
