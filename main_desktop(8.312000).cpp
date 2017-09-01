#include "main_desktop.h"
#include "ui_main_desktop.h"
#include <QDesktopWidget>

main_desktop *pstaticthis=NULL;
#if defined(Q_OS_LINUX)
//int g_recordstatus=0;
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
void userdataCallback(int32_t msgType,char* dataPtr, camera_frame_metadata_t *metadata, void* user){
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
                for(int j=0;j<SUPPORT_CAMERA_NUM;j++){
                    dvr_factory* pdvr=dvrCamera[j].dvr;
                    pdvr->startRecord();
                }

            }else
                ALOGE("record status error");
        }

    }
    else if (action == NetlinkEvent::NlActionRemove)
    {
        if (!strcmp(devtype, "disk"))
        {
            if(g_recordstatus==1){
                for(int j=0;j<SUPPORT_CAMERA_NUM;j++){
                    dvr_factory* pdvr=this.dvrCamera[j].dvr;
                    pdvr->storage_state=0;//tmp
                    pdvr->stopRecord();
                }
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

    setProperty("noinput",true);
    isaf=false;
    #if defined(Q_OS_LINUX)
    sdk_log_setlevel(3);
    HwDisplay* mcd=NULL;
    int i;
    int tvmode=0;
    cur_camera = 0 ;
    //test screen mode
    //0 disable
    //config_set_tvout(0,tvmode);
    mcd=HwDisplay::getInstance();
    #ifdef AUTEVENT_TEST//defined
        NetlinkManager *nm;
        if (!(nm = NetlinkManager::Instance())) {
            SLOGE("Unable to create NetlinkManager");
        };
        printf("--------------------------------NetlinkManager done\n");
    #endif
    pgEmulatedCameraFactory=new HALCameraFactory();
    for(i=0;i<SUPPORT_CAMERA_NUM;i++)
         dvrCamera[i].dvr=new dvr_factory();
    #ifdef AUDIO_TEST //defined
        sp<AudioCapThread> audiocap=new AudioCapThread();
        for(i=0;i<SUPPORT_CAMERA_NUM;i++){
            dvr_factory* pdvr=dvrCamera[i].dvr;
            int hdl=audiocap->reqAudioSource(pdvr->__audio_data_cb_timestamp,(void *) pdvr);
            pdvr->setAudioOps(audiocap,hdl);
        }
         printf("--------------------------------AudioCapThread done\n");
    #endif
    #ifdef AUTEVENT_TEST//defined
        AutEvent *pevent=AutEvent::Instance();
        for(i=0;i<SUPPORT_CAMERA_NUM;i++){
            dvr_factory* pdvr=dvrCamera[i].dvr;
            pevent->setEventCb(testaut_event_cb_func,(void *)pdvr);
        }
        if (nm->start()) {
            SLOGE("Unable to start NetlinkManager (%s)", strerror(errno));
        }
        printf("--------------------------------AutEvent done\n");
    #endif
    tvmode=config_get_tvout(0);
    printf("--------------tvmode =%d \r\n",tvmode);
    mcd->hwd_screen1_mode(tvmode);
    //mcd->hwd_tvout();
    config_set_startup(TEST_CAMERA_ID,1);
    printf("-------------------------------------construction function done\n");
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
                p_dvr->SetDataCB(usr_datacb,p_dvr);
                p_dvr->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,p_dvr /*dump*/);
                p_dvr->start();
                p_dvr->enableWaterMark();
                sprintf(bufname,"64,64,0,64,250,T3L SDK,64,450,ASTEROID V1 alpha");
                p_dvr->setWaterMarkMultiple(bufname);
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
            p_dvr->SetDataCB(usr_datacb,p_dvr);
            p_dvr->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,p_dvr /*dump*/);
            p_dvr->start();
            p_dvr->enableWaterMark();
            sprintf(bufname,"64,64,0,64,250,T2L SDK,64,450,RAINBOW V0.3a");
            p_dvr->setWaterMarkMultiple(bufname);
            p_dvr->startRecord();
            isaf = true;
            dvrCamera[cur_camera].setRecord(true);
    }
    if(!dvrCamera[cur_camera].getPreview())
    {
        printf("--------------------------------------set current camera %d getPreview\n",cur_camera);
        struct view_info vv= {0,0,VIEW_WEITH,VIEW_HEIGHT};
        //ALOGV("vx=%d vy=%d sx=%d sy%d",mcd->lcdxres,mcd->lcdyres,dvr->recordwith,dvr->recordheith);
        p_dvr->startPriview(vv);
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
    {
        dvrCamera[i].dvr->enc_de_init();
        dvrCamera[i].dvr->uninitializeDev();
        delete dvrCamera[i].dvr;
    }
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


