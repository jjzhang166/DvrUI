#include "video_player.h"
#include "ui_video_player.h"
#include "frmmessagebox.h"
#include <QDirIterator>
#include "videowidget.h"
#include "ui_videowidget.h"
#include "main_desktop.h"
#include <QFontDatabase>
#include <QPushButton>
#include <QDesktopWidget>
#include <QPalette>

#include "moviedesk1.h"
#include "frmmessagebox.h"
#if defined(Q_OS_LINUX)
extern int is_dir_exist(const char *dir_path);
#endif
#define SEEKTO 1
extern movieDesk1* pStaticMovieDesk;
Video_Player* pStaticVideoPlayer=NULL;
static int subtitle_num_now=1;
static int audio_num_now=1;
#if defined(Q_OS_LINUX)
#define AUDIO_PCM_OUTPUT 128
#if  1
int autCb_func(int32_t msgType, void *user,void* data,int len)
{
#if defined(Q_OS_LINUX)
    SubtitleItem* pItem = (SubtitleItem*)data;
    switch(msgType)
    {
        case AUDIO_PCM_OUTPUT:
            break;

        case SUBCTRL_SUBTITLE_AVAILABLE:
        {
            
            if(pItem == NULL)
            {
                logw("error pItem == NULL");
                return -1;
            }
            if(pItem->bText!=0)
            {
                if(pItem->pText!=NULL){
                    QString str=QString(pItem->pText);
                    pStaticVideoPlayer->subtitle_label->setVisible(true);
                    pStaticVideoPlayer->subtitle_label->setAlignment(Qt::AlignCenter);

                    printf("-----now is text subtitle--------\n");
                    printf("subtitle show,text=%s\n",pItem->pText);

                    pStaticVideoPlayer->subtitle_label->setText(str);
                    qDebug()<<"subtitle show done";
                }
            }else{
                printf("-----now is bitmap subtitle--------\n");
                pStaticVideoPlayer->subtitle_label->setVisible(true);
                pStaticVideoPlayer->subtitle_label->setAlignment(Qt::AlignCenter);
                int w,h;
                w=pItem->nBitmapWidth;
                h=pItem->nBitmapHeight;
                QImage* img=new QImage((const uchar*)pItem->pBitmapData,w,h,QImage::Format_RGBA8888);
                QImage imgScaled=img->scaled(pStaticVideoPlayer->subtitle_label->width(),pStaticVideoPlayer->subtitle_label->height(),Qt::KeepAspectRatio);
                QPixmap pix=QPixmap();
                pix.convertFromImage(imgScaled);
                pStaticVideoPlayer->subtitle_label->setPixmap(pix);
            }
            break;
        }
        case SUBCTRL_SUBTITLE_EXPIRED:
        {
            printf("subtitle disable\n");
            if(pItem == NULL)
            {
                printf("error: pItem == NULL");
                return -1;
            }
            if(pItem->bText!=0)
            {
                printf("-----now is text subtitle--------\n");
                pStaticVideoPlayer->subtitle_label->setVisible(true);
                pStaticVideoPlayer->subtitle_label->setText(" ");
            }else{
                printf("-----now is bitmap subtitle--------\n");
                pStaticVideoPlayer->subtitle_label->setVisible(false);
            }
            break;
        }
        case AWPLAYER_MEDIA_PREPARED:
        case AWPLAYER_MEDIA_PLAYBACK_COMPLETE:
            break;
        case AWPLAYER_MEDIA_SEEK_COMPLETE:
        {
            qDebug()<<"call back seek to is complete ";
            pStaticVideoPlayer->seekto_is_ok=1;
            break;
        }
        case AWPLAYER_MEDIA_LOG_RECORDER:
        case AWPLAYER_MEDIA_BUFFERING_UPDATE:
        case AWPLAYER_MEDIA_ERROR:
        case AWPLAYER_MEDIA_INFO:
        case AWPLAYER_MEDIA_SET_VIDEO_SIZE:
            break;
        default:
        {
            //logw("message 0x%x not handled.", messageId);
            break;
        }
    }
#endif
    return 0;
}
#endif
#endif
//const QString win_path="E:/tech_practise/DvrUI/DvrUI/video/";
//const QString linux_path="/mnt/sdcard/mmcblk1p1/frontVideo/";//sdcard
//const QString linux_path="/mnt/usb/sda4/";//upan
extern QFileInfo fileInfo_to_play;
extern main_desktop* pStaticMaindesktop;
static int test=100;
const int interval=10;//
Video_Player::Video_Player(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Video_Player)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    qDebug()<<"the filename will play is"<<fileInfo_to_play.completeBaseName();
    ui->playButton->setIcon(QIcon(":/icon/Pause.png"));
    ui->playButton->setIconSize(QSize(51,51));
    ui->stopButton->setIcon(QIcon(":/icon/Stop.png"));
    ui->stopButton->setIconSize(QSize(51,51));
    ui->muteButton->setIcon(QIcon(":/icon/voice.png"));
    ui->muteButton->setIconSize(QSize(51,51));
    ui->voiceSlider->setRange(0,100);
    ui->voiceSlider->setValue(30);
    ui->fastBackButton->setEnabled(true);
    ui->fastFrontButton->setEnabled(true);
    show_title();
    FormInCenter();
    current_path=fileInfo_to_play.absolutePath();
    qDebug()<<"current path is:"<<current_path;
#if defined(Q_OS_LINUX)
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(100);
    if(!is_dir_exist((char*)current_path.toStdString().c_str()))
    {
        dir.setPath(current_path);
        file_list=GetFileList(dir);
        all_file_list=GetAllFileList(dir);
        for(int i=0;i<file_list.size();i++)
        {
            if(file_list[i]==fileInfo_to_play){
                current_video=i;
                break;
            }
        }
    }else{
        qDebug()<<"path is not exist";
    }
#else
    if(current_path==""){
        qDebug()<<"the wrong path";
    }else{
        dir.setPath(current_path);
        file_list=GetFileList(dir);
        all_file_list=GetAllFileList(dir);
        for(int i=0;i<file_list.size();i++)
        {
            if(file_list[i]==fileInfo_to_play){
                current_video=i;
                break;
            }
        }
    }

#endif
    isMuted=false;
    isPlaying=true;
    seekto_is_ok=1;

    ui->widget->setVisible(true);
    ui->widget_2->move(10,467);
    ui->widget_2->setGeometry(10,467,1024,60);
    setMouseTracking(true);
    mouseMoveTime = new QTimer();
    connect(mouseMoveTime,SIGNAL(timeout()),this,SLOT(on_timeout_mouserMoveTime()));
    mouseMoveTime->start(100000);
    pStaticVideoPlayer=this;
    subtitle_label=new QLabel();
    subtitle_label->resize(QSize(600,60));
    subtitle_label->setAlignment(Qt::AlignCenter);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    subtitle_label->setPalette(pe);
    subtitle_label->setParent(ui->widget_2);
    subtitle_label->move((ui->widget_2->width()-subtitle_label->width())/2,(ui->widget_2->height()-subtitle_label->height())/2);

    ui->fastBackButton->setAutoRepeat(true);
    ui->fastFrontButton->setAutoRepeat(true);

#if defined(Q_OS_LINUX)
    ap = new AUTPlayer();
    if(ap == NULL){
        qDebug()<<"new AutPlayer error";
        return ;
    }
    astatus = ASTATUS_STOPPED;
    QFileInfo file_path_to_play=file_list.at(current_video);
    QString ab_file_path=file_path_to_play.absoluteFilePath();
    qDebug()<<"the file's absoluteFilePath is"<<ab_file_path;
    QString file_name=file_path_to_play.fileName();
    qDebug()<<"the video name: "<<file_name<<"while play";

    int ret = ap->setUrl((char*)ab_file_path.toStdString().c_str());
    if (ret < 0)
    {
        qDebug()<<"setUrl is error-------------------";
        on_stopButton_clicked();
         ui->label->setText("");
         emit video_source_error();
        frmMessageBox *msg=new frmMessageBox;
        msg->SetMessage(QString(tr("video not support!")),0);
        msg->exec();
    }else{
        ap->setUserCb(autCb_func, this);
        ap->setViewWindow(0, 0, 1024, 600);
        find_correspond_subtitle_file(ab_file_path);
        int linux_duration=ap->getDuration()/1000;
        ui->progressSlider->setRange(0,linux_duration);
        printf("----------------------the video totaltime is %d\n",linux_duration);
        QString totalTime=QDateTime::fromTime_t(linux_duration).toString("mm:ss");
        printf("----------------------the video totaltime is %s\n",(char*)totalTime.toStdString().c_str());
        ret=ap->play();
        if(ret!=0)
            qDebug()<<"play bug";
        astatus = ASTATUS_PLAYING;
        this->duration=linux_duration;
        qDebug()<<"total time is"<<duration;
        //can't catch
        //method 1
//        QByteArray ba=file_name.toLocal8Bit();
//        const char* str=ba.data();
        //method2
        const char* str=file_name.toLatin1().data();
        ui->label->setText(str);
        connect(ui->voiceSlider,SIGNAL(valueChanged(int)),this,SLOT(setVolume(int)));
        connect(ui->progressSlider,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));
    }



    //printf("---------------------------------cant find the correct video\n");


#else
        QFileInfo file_path_to_play=file_list.at(current_video);
        QString ab_file_path=file_path_to_play.absoluteFilePath();
        QString file_name=file_path_to_play.fileName();

        player = new QMediaPlayer(0,QMediaPlayer::VideoSurface);
        my_video_widget=new QVideoWidget();
        ui->scrollArea->setWidget(my_video_widget);
        player->setVideoOutput(my_video_widget);
        my_video_widget->show();

        player->setMedia(QMediaContent(QUrl::fromLocalFile(ab_file_path)));
        player->play();

        ui->label->setText(QString(tr("now playing:"))+file_name);
        ui->progressSlider->setRange(0,player->duration()/1000);
        connect(ui->voiceSlider,SIGNAL(valueChanged(int)),player,SLOT(setVolume(int)));
        connect(ui->progressSlider,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));
        connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
        connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));

#endif
    connect(this,SIGNAL(main_desktop_visible()),pStaticMaindesktop,SLOT(on_main_desktop_visible()));
    connect(this,SIGNAL(video_source_error()),this,SLOT(on_video_source_error()));
    ui->voiceSlider->setRange(0,200);
    ui->voiceSlider->setValue(100);
//    ui->widget->setAttribute(Qt::WA_TranslucentBackground, true);
//    ui->widget_2->setAttribute(Qt::WA_TranslucentBackground, true);

}
void Video_Player::on_video_source_error()
{
    qDebug()<<"video is not support";
}
void Video_Player::find_correspond_subtitle_file(QFileInfo fileInfo)
{
    QMap<QString,QFileInfo>tmp_map=get_subtitle_files(fileInfo);
    qDebug()<<"this video has the subtitle file is:";
    show_subtitle_file_name(tmp_map);
    int ret=0;
    if(tmp_map.size()==1)
    {
        qDebug()<<"only have 1 subtitle file";
        QString file_name=tmp_map.begin().value().absoluteFilePath();
        qDebug()<<"set the subtitle file is:"<<file_name;
    #if defined(Q_OS_LINUX)
        if(!file_name.isEmpty())
        {
            ret=ap->setsubtitleUrl((char*)file_name.toStdString().c_str());
            if(ret!=0){
                qDebug()<<"subtitle set fail";
                frmMessageBox *msg=new frmMessageBox;
                msg->SetMessage(QString(tr("subtitle set fail!")),2);
                QTimer::singleShot(1000, msg, SLOT(close()));
                msg->exec();
                return ;
            }
        }
    #endif
    }else if(tmp_map.size()==2){
        qDebug()<<tmp_map.contains("sub")<<tmp_map.contains("idx");
        if(!tmp_map.contains("sub")&&!tmp_map.contains("idx"))
        {
            QString filename=tmp_map[tmp_map.firstKey()].absoluteFilePath();
            qDebug()<<"find 2 subtitle select any one";
            if(!filename.isEmpty())
            {
            #if defined(Q_OS_LINUX)
                ret=ap->setsubtitleUrl((char*)filename.toStdString().c_str());
                if(ret!=0){
                    qDebug()<<"setsubtitleUrl subtitle set fail";
                    frmMessageBox *msg=new frmMessageBox;
                    msg->SetMessage(QString(tr("setsubtitleUrl set fail!")),2);
                    QTimer::singleShot(1000, msg, SLOT(close()));
                    msg->exec();
                    return ;
                }
            #endif
            }
            return ;
        }
        qDebug()<<"have 2 subtitle file";
        QString file_name_1=tmp_map["sub"].absoluteFilePath();
        QString file_name_2=tmp_map["idx"].absoluteFilePath();
    #if defined(Q_OS_LINUX)
        qDebug()<<"the subtitle "<<"\n"<<file_name_1<<"\n"<<file_name_2<<"\n"<<"while be set";
        if(!file_name_1.isEmpty()&&!file_name_2.isEmpty())
        {  
            ret=ap->setsubtitleUrlFd((char*)file_name_1.toStdString().c_str(),
                        (char*)file_name_2.toStdString().c_str());
            if(ret!=0){
                qDebug()<<"setsubtitleUrlFd subtitle set fail";
                frmMessageBox *msg=new frmMessageBox;
                msg->SetMessage(QString(tr("setsubtitleUrlFd set fail!")),2);
                QTimer::singleShot(1000, msg, SLOT(close()));
                msg->exec();
                return ;
            }
        }
    #endif
    }else if(tmp_map.size()==0){
        qDebug()<<"this subtitle is not outside";
    }
    return ;
}


void Video_Player::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(0, 0, 10, 90));
}

Video_Player::~Video_Player()
{
	if(timer != NULL)
		delete timer;
    delete ui;
    #if defined(USE_AUTPLAYER)
    	if(ap != NULL)
        	delete ap;
        astatus = ASTATUS_STOPPED;
    #endif
}

void Video_Player::setVolume(int n_value)
{
#if defined(Q_OS_LINUX)
    qDebug()<<"player ready to change the voice";
    QString cmd="tinymix 2 "+ QString::number(n_value,10);
    system((char*)cmd.toStdString().c_str());
#endif
}
void Video_Player::timerUpdate()
{
//    qDebug()<<"update label in every 10ms";
#if defined(Q_OS_LINUX)
    int currentTime=ap->getCurrentPosition()/1000;
//    qDebug()<<"current video time is"<<currentTime;
    ui->progressSlider->setValue(currentTime);
    updateDurationInfo(currentTime);
#endif
}

void Video_Player::durationChanged(qint64 duration)
{
    qDebug()<<"------durationchanged";
    this->duration = duration/1000;
    ui->progressSlider->setMaximum(duration / 1000);
}

void Video_Player::positionChanged(qint64 progress)
{
    qDebug()<<"----------positionchanged";
    if (!ui->progressSlider->isSliderDown()) {
        ui->progressSlider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}
void Video_Player::updateDurationInfo(qint64 currentInfo)
{
//    qDebug()<<"update the time label";
    QString tStr;

#if defined(Q_OS_LINUX)
    int linux_duration=ap->getDuration()/1000;
    if(linux_duration==(int)currentInfo)
    {
        currentInfo=0;
        ui->progressSlider->setValue(0);
        astatus = ASTATUS_PAUSED;
        ui->playButton->setIcon(QIcon(":/icon/play.png"));
    }
#endif
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    ui->nowTimeLabel->setText(tStr);
}
void Video_Player::seek(int seconds)
{
#if defined(Q_OS_LINUX)
    int ret=0;
    qDebug()<<"ready to seekto"<<seconds<<"s";
    timer->stop();
    int currentTime=ap->getCurrentPosition()/1000;
    astatus = ASTATUS_SEEKING;
    ret=ap->seekto(seconds);
    if(ret!=0)
        qDebug()<<"seekto bug";
    if (!ui->progressSlider->isSliderDown()) {
        ui->progressSlider->setValue(currentTime);
    }
    updateDurationInfo(seconds);
    timer->start(100);
#else
    player->setPosition(seconds * 1000);
#endif
}

void Video_Player::on_playButton_clicked()
{
//    qDebug()<<"start video";
#if defined(Q_OS_LINUX)
    if(ap==NULL)
        return;
    int ret=0;
    ret=ap->pause();
    if(ret!=0)
        qDebug()<<"pause bug";
    if (astatus == ASTATUS_PAUSED)
    {
      astatus = ASTATUS_PLAYING;
      ui->playButton->setIcon(QIcon(":/icon/Pause.png"));
    }
    else
    {
      astatus = ASTATUS_PAUSED;
      ui->playButton->setIcon(QIcon(":/icon/play.png"));

    }
#else
    if(isPlaying){
        qDebug()<<"pause video";
        player->pause();
        ui->playButton->setIcon(QIcon(":/icon/play.png"));
        isPlaying=false;
    }else{
        qDebug()<<"start video";
        player->play();
        ui->playButton->setIcon(QIcon(":/icon/Pause.png"));
        isPlaying=true;
    }

#endif
}

void Video_Player::on_stopButton_clicked()
{
    qDebug()<<"close video";
#if defined(Q_OS_LINUX)
    #if defined(USE_AUTPLAYER)
        timer->stop();
        if(ap!=NULL){
            ap->stop();
            delete ap;
        }
        ap=NULL;
        ui->fastBackButton->setEnabled(false);
        ui->fastFrontButton->setEnabled(false);
        ui->playButton->setIcon(QIcon(":/icon/play.png"));
    #endif
    astatus = ASTATUS_STOPPED;

#else
    player->stop();
#endif
}

void Video_Player::on_muteButton_clicked()
{
#if !defined(Q_OS_LINUX)
    qDebug()<<"set muted";
    if(!isMuted){
        qDebug()<<"now is not muted";
        player->setMuted(true);
        ui->muteButton->setIcon(QIcon(":/icon/mute.png"));
        ui->voiceSlider->setValue(0);
        isMuted=true;
    }else{
        qDebug()<<"now is muted";
        player->setMuted(false);
        ui->voiceSlider->setValue(100);
        ui->muteButton->setIcon(QIcon(":/icon/voice.png"));
        isMuted=false;
    }
#else
    qDebug()<<"set muted";
    if(!isMuted){
        qDebug()<<"now is not muted";
        ui->muteButton->setIcon(QIcon(":/icon/mute.png"));
        ui->voiceSlider->setValue(0);
        isMuted=true;
    }else{
        qDebug()<<"now is muted";
        ui->voiceSlider->setValue(50);
        ui->muteButton->setIcon(QIcon(":/icon/voice.png"));
        isMuted=false;
    }
#endif
}

void Video_Player::on_btnMenu_Min_clicked()
{
    m_menu=new QMenu();
    m_subtitleSelect=new QAction(m_menu);
    m_audioFileSelect=new QAction(m_menu);
    m_subtitleSelect->setText(QObject::tr("subtitle select"));
    m_audioFileSelect->setText(QObject::tr("audio select"));
    m_menu->addAction(m_subtitleSelect);
    m_menu->addAction(m_audioFileSelect);
    QMenu* m_subMenu_subtitle=new QMenu();
    QMenu* m_subMenu_audio=new QMenu();
    m_subtitleSelect->setMenu(m_subMenu_subtitle);
    m_audioFileSelect->setMenu(m_subMenu_audio);

    QPoint pos(ui->btnMenu_Min->x()-40,ui->btnMenu_Min->y()+ui->btnMenu_Min->height());

#if defined(Q_OS_LINUX)
    int num_subtitle;
#else
    int num_subtitle=4;
#endif
    getAllSubtitle(num_subtitle);
    for(int i=0;i<num_subtitle;i++)
    {
        QAction* file=new QAction(m_subMenu_subtitle);
        QString str=QString(tr("subtitle%1")).arg(i+1);
        file->setText(str);
        m_subMenu_subtitle->addAction(file);
        if(i==subtitle_num_now-1)
        {
            file->setCheckable(true);
            file->setChecked(true);
        }
    }
#if defined(Q_OS_LINUX)
    int num_audio;
#else
    int num_audio=3;
#endif
    getAllAudio(num_audio);
    for(int i=0;i<num_audio;i++)
    {
        QAction* file=new QAction(m_subMenu_audio);
        QString str=QString(tr("audio%1")).arg(i+1);
        file->setText(str);
        m_subMenu_audio->addAction(file);
        if(i==audio_num_now-1)
        {
            file->setCheckable(true);
            file->setChecked(true);
        }
    }
    connect(m_subMenu_subtitle,SIGNAL(triggered(QAction*)),this,SLOT(setSubtitleNum(QAction*)));
    connect(m_subMenu_audio,SIGNAL(triggered(QAction*)),this,SLOT(setAudioNum(QAction*)));
    m_menu->exec(mapToGlobal(pos));

}
void Video_Player::setSubtitleNum(QAction* pAction)
{
    QString file=pAction->text();
    pAction->setCheckable(true);
    pAction->setChecked(true);
    qDebug()<<"triggered the "<<file;
    QString tmp;
    for(int i=0;i<file.size();i++)
    {
        if(file[i].isDigit())
        {
            tmp+=file[i];
        }
    }
    int num=tmp.toInt();
    subtitle_num_now=num;
    qDebug()<<"the subtitle "<<num<<" will be set";
#if defined(Q_OS_LINUX)
    ap->switchSubtitle(num);
#endif
}
void Video_Player::getAllSubtitle(int& nums)
{
#if defined(Q_OS_LINUX)
    qDebug()<<"get subtitle num";
    MediaInfo* mediaInfo=ap->getMediaInfo();
    nums=mediaInfo->nSubtitleStreamNum;
    qDebug()<<"the video have "<<nums<<"subtitles";
#endif
}
void Video_Player::setAudioNum(QAction* pAction)
{
    QString file=pAction->text();
    pAction->setCheckable(true);
    pAction->setChecked(true);
    qDebug()<<"triggered the "<<file;
    QString tmp;
    for(int i=0;i<file.size();i++)
    {
        if(file[i].isDigit())
        {
            tmp+=file[i];
        }
    }
    int num=tmp.toInt();
    audio_num_now=num;
    qDebug()<<"the audio "<<num<<" will be set";
#if defined(Q_OS_LINUX)
    ap->switchAudio(num);
#endif
}
void Video_Player::getAllAudio(int& nums)
{
#if defined(Q_OS_LINUX)
    qDebug()<<"get audio num";
    MediaInfo* mediaInfo=ap->getMediaInfo();
    nums=mediaInfo->nAudioStreamNum;
    qDebug()<<"the video have "<<nums<<"audio file";
#endif
}

void Video_Player::on_btnMenu_Max_clicked()
{
    #if defined(Q_OS_LINUX)
    timer->stop();
        #if defined(USE_AUTPLAYER)
            ap->stop();
        #endif
        astatus = ASTATUS_STOPPED;

    #else
        player->stop();
    #endif
    this->close();
//        pStaticMaindesktop->setHidden(false);
     emit main_desktop_visible();
        pStaticMovieDesk->setHidden(false);
}

void Video_Player::on_btnMenu_Close_clicked()
{

#if defined(Q_OS_LINUX)
    #if defined(USE_AUTPLAYER)
    timer->stop();
    if(ap!=NULL){
        ap->stop();
        delete ap;
    }
    ap=NULL;

    #endif
    astatus = ASTATUS_STOPPED;

#else
    player->stop();
#endif
this->close();
//    pStaticMaindesktop->setHidden(false);
    emit main_desktop_visible();
    pStaticMovieDesk->setHidden(false);

}
void Video_Player::show_title()
{
    int fontId = QFontDatabase::addApplicationFont(":/font/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont iconFont = QFont(fontName);
    iconFont.setPointSize(10);
    ui->btnMenu_Close->setFont(iconFont);
    ui->btnMenu_Close->setText(QChar(0xf00d));
    ui->btnMenu_Max->setFont(iconFont);
    ui->btnMenu_Max->setText( QChar(0xf096));
    ui->btnMenu_Min->setFont(iconFont);
    ui->btnMenu_Min->setText( QChar(0xf068));
    iconFont.setPointSize(12);
    ui->lab_Ico->setFont(iconFont);
    ui->lab_Ico->setText( QChar(0xf015));
#if !defined(Q_OS_LINUX)
    ui->label->setFont(iconFont);
    ui->nowTimeLabel->setFont(iconFont);
#endif
}

void Video_Player::FormInCenter()
{
    int frmX = this->width();
    int frmY = this->height();
    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    this->move(movePoint);
}

void Video_Player::on_preMovieButton_clicked()
{
    qDebug()<<"-----------------on_preMovieButton_clicked,here";
#if defined(Q_OS_LINUX)
    if(current_video-1>=0){
        timer->stop();
        
        if(ap!=NULL){
            ap->stop();
        delete ap;
        }
        astatus = ASTATUS_STOPPED;
        ap = new AUTPlayer();
        if(ap == NULL){
            qDebug()<<"new AutPlayer error";
            return ;
        }
        ui->fastBackButton->setEnabled(true);
        ui->fastFrontButton->setEnabled(true);
        if(!is_dir_exist((char*)current_path.toStdString().c_str()))
        {
            int ret=0;
//            file_list=GetFileList(dir);
            QFileInfo file_path_to_play=file_list.at(--current_video);
            QString ab_file_path=file_path_to_play.absoluteFilePath();
            QString file_name=file_path_to_play.fileName();
            if(ab_file_path.isEmpty())
            {
                qDebug()<<"file path is error";
            }
            ret = ap->setUrl((char*)ab_file_path.toStdString().c_str());
		    if (ret < 0)
		    {
		        qDebug()<<"setUrl is error-------------------";
		        on_stopButton_clicked();
                 ui->label->setText("");
		        frmMessageBox *msg=new frmMessageBox;
		        msg->SetMessage(QString(tr("video not support!")),0);
		        msg->exec();
		        return ;
		    }
            find_correspond_subtitle_file(ab_file_path);
            ap->setUserCb(autCb_func, this);
            ret=ap->setViewWindow(0, 0, 1024, 600);
            if(ret!=0)
                qDebug()<<"setViewWindow bug";
            int linux_duration=ap->getDuration()/1000;
            ui->progressSlider->setRange(0,linux_duration);
            printf("----------------------the video totaltime is %d\n",linux_duration);
            QString totalTime=QDateTime::fromTime_t(linux_duration).toString("mm:ss");
            printf("----------------------the video totaltime is %s\n",(char*)totalTime.toStdString().c_str());
            ret=ap->play();
            if(ret!=0)
                qDebug()<<"play bug";
            this->duration=linux_duration;
            qDebug()<<"total time is"<<duration;
            QByteArray ba=file_name.toLocal8Bit();
            const char* str=ba.data();
            ui->label->setText(QString(tr("now play:"))+QString(str));
            timer->start(100);
            astatus = ASTATUS_PLAYING;
            ui->playButton->setIcon(QIcon(":/icon/Pause.png"));
        }
    }else{
        qDebug()<<"this is the first video"<<current_video<<file_list.size();
        frmMessageBox *msg=new frmMessageBox;
        msg->SetMessage(QString(tr("this is first video!")),0);
        QTimer::singleShot(8000, msg, SLOT(close()));
        msg->show();
    }
#else
    if(current_video-1>=0){
        file_list=GetFileList(dir);
        QFileInfo file_path_to_play=file_list.at(--current_video);
        QString ab_file_path=file_path_to_play.absoluteFilePath();
        QString file_name=file_path_to_play.fileName();
        player->setMedia(QMediaContent(QUrl::fromLocalFile(ab_file_path)));
        player->play();

        ui->label->setText(QString(tr("now playing:"))+file_name);
        ui->progressSlider->setRange(0,player->duration()/1000);
    }

#endif
}

void Video_Player::on_fastBackButton_clicked()
{
#if defined(Q_OS_LINUX)

//    int nowTime=ap->getCurrentPosition()/1000;
//    if(nowTime<20)
//    {
//        qDebug()<<"------------------begin play less than 20s";
//        ap->seekto(1);
//        updateDurationInfo(1);
//        return ;
//    }
//    qDebug()<<"------------------begin play lager than 20s";
    mouseMoveTime->stop();
    timer->stop();
    int ret;
#if defined(SEEKTO)

    qDebug()<<"-----------------now is seekto";
    int currentTime=ap->getCurrentPosition()/1000;

    if(seekto_is_ok==1)
    {
        qDebug()<<"-------------------now seek to is ok";
        currentTime-=interval;
        ret=ap->seekto(currentTime);
        if(ret!=0)
            qDebug()<<"seekto bug";
        seekto_is_ok=0;
    }else{
        qDebug()<<"-------------------seek to is not ok";
        currentTime=currentTime;
    }
#else
    ap->setSpeed(-10);
    int currentTime=ap->getCurrentPosition()/1000;
#endif
    ui->progressSlider->setValue(currentTime);
    updateDurationInfo(currentTime);
    timer->start(100);
    mouseMoveTime->start(100000);
#else
    int currentValue=ui->progressSlider->value();
    currentValue-=interval;
    if(currentValue<this->duration){
        ui->progressSlider->setValue(currentValue);
        updateDurationInfo(currentValue);
        player->setPosition(currentValue*1000);
    }else{
        qDebug()<<"out of the media time";
    }
#endif
}

void Video_Player::on_fastFrontButton_clicked()
{
#if defined(Q_OS_LINUX)
    mouseMoveTime->stop();
    timer->stop();
#if defined(SEEKTO)
    int ret=0;
    qDebug()<<"-----------------now is seekto";
    int currentTime=ap->getCurrentPosition()/1000;
    if(seekto_is_ok==1)
    {
        qDebug()<<"-------------------now seek to is ok";
        currentTime+=interval;
        if(currentTime<duration)
        {
            ret=ap->seekto(currentTime);
            if(ret!=0)
                qDebug()<<"seekto bug";
            seekto_is_ok=0;
        }else{

        }
    }else{
        qDebug()<<"-------------------seek to is not ok";
        currentTime=currentTime;
    }
#else
    ap->setSpeed(10);
    int currentTime=ap->getCurrentPosition()/1000;
#endif

     ui->progressSlider->setValue(currentTime);
     updateDurationInfo(currentTime);
     timer->start(100);
     mouseMoveTime->start(100000);
#else
    int currentValue=ui->progressSlider->value();
    currentValue+=interval;
    if(currentValue<this->duration){
        ui->progressSlider->setValue(currentValue);
        updateDurationInfo(currentValue);
        player->setPosition(currentValue*1000);
    }else{
        qDebug()<<"out of the media time";
    }
#endif
}

void Video_Player::on_nextMovieButton_clicked()
{
    qDebug()<<"-----------------on_nextMovieButton_clicked,here";
#if defined(Q_OS_LINUX)
    if(current_video+1<file_list.size()){
        qDebug()<<"----------------now is:"<<current_video<<"\n-----------------------total is"<<file_list.size();
        timer->stop();
        astatus = ASTATUS_STOPPED;
        if(ap!=NULL){
            ap->stop();
            delete ap;
        }
        ap = new AUTPlayer();
        if(ap == NULL){
        	qDebug()<<"new AutPlayer error";
			return ;
        }
        ui->fastBackButton->setEnabled(true);
        ui->fastFrontButton->setEnabled(true);
        if(!is_dir_exist((char*)current_path.toStdString().c_str()))
        {
            int ret=0;
//            file_list=GetFileList(dir);
            QFileInfo file_path_to_play=file_list.at(++current_video);
            QString ab_file_path=file_path_to_play.absoluteFilePath();
            QString file_name=file_path_to_play.fileName();

            if(ab_file_path.isEmpty())
            {
                qDebug()<<"file path is error";
            }
            ret = ap->setUrl((char*)ab_file_path.toStdString().c_str());
		    if (ret < 0)
		    {
		        qDebug()<<"setUrl is error-------------------";
		        on_stopButton_clicked();
                 ui->label->setText("");
		        frmMessageBox *msg=new frmMessageBox;
		        msg->SetMessage(QString(tr("video not support!")),0);
		        msg->exec();
		        return ;
		    }
            find_correspond_subtitle_file(ab_file_path);
            ap->setUserCb(autCb_func, this);
            ap->setViewWindow(0, 0, 1024, 600);
            int linux_duration=ap->getDuration()/1000;
            ui->progressSlider->setRange(0,linux_duration);
            printf("----------------------the video totaltime is %d\n",linux_duration);
            QString totalTime=QDateTime::fromTime_t(linux_duration).toString("mm:ss");
            printf("----------------------the video totaltime is %s\n",(char*)totalTime.toStdString().c_str());
            ret=ap->play();
            if(ret!=0)
                qDebug()<<"play bug";
            this->duration=linux_duration;
            qDebug()<<"total time is"<<duration;
            QByteArray ba=file_name.toLocal8Bit();
            const char* str=ba.data();
            ui->label->setText(QString(tr("now play:"))+QString(str));
            astatus = ASTATUS_PLAYING;
            timer->start(100);
            ui->playButton->setIcon(QIcon(":/icon/Pause.png"));
        }

    }else{//the last video
        qDebug()<<"this is the last video"<<current_video<<file_list.size();
        frmMessageBox *msg=new frmMessageBox;
        msg->SetMessage(QString(tr("this is last video!")),0);
        QTimer::singleShot(8000, msg, SLOT(close()));
        msg->show();
    }
#else
    if(current_video+1<file_list.size()){
        file_list=GetFileList(dir);
        QFileInfo file_path_to_play=file_list.at(++current_video);
        QString ab_file_path=file_path_to_play.absoluteFilePath();
        QString file_name=file_path_to_play.fileName();
        player->setMedia(QMediaContent(QUrl::fromLocalFile(ab_file_path)));
        player->play();
        ui->label->setText(QString(tr("now playing"))+file_name);
        ui->progressSlider->setRange(0,player->duration()/1000);
    }

#endif
}
QFileInfoList Video_Player::GetFileList(QDir dir)
{
    qDebug()<<"get all video file name";
    QStringList filters;
    filters << "*.mp4" <<"*.MP4"<< "*.avi"<<"*.AVI"<<"*.mkv"<<"*.MKV"<<"*.mp3"<<"*.VOB"<<"*.vob"<<"*.FLV"<<"*.flv"<<"*.WMV";
    dir.setNameFilters(filters);
    QFileInfoList file_list=dir.entryInfoList();
    for(int i=0;i<file_list.size();i++)
    {
        QFileInfo fileInfo=file_list.at(i);
        fileInfo.absoluteFilePath();
//        qDebug()<<fileInfo.fileName();
    }
    return file_list;
}
QFileInfoList Video_Player::GetAllFileList(QDir dir)
{
    qDebug()<<"get all file name";
    QFileInfoList all_file_list=dir.entryInfoList();
    for(int i=0;i<all_file_list.size();i++)
    {
        QFileInfo fileInfo=all_file_list.at(i);
        fileInfo.absoluteFilePath();
//        qDebug()<<fileInfo.fileName();
    }
    return all_file_list;
}

void Video_Player::mouseDoubleClickEvent(QMouseEvent *)
{
    on_mouse_active();
}
void Video_Player::mouseMoveEvent(QMouseEvent *)
{
    on_mouse_active();
}
void Video_Player::mousePressEvent(QMouseEvent *)
{
    on_mouse_active();
}
void Video_Player::mouseReleaseEvent(QMouseEvent *)
{
    on_mouse_active();
}

void Video_Player::on_mouse_active()
{
    mouseMoveTime->stop();
    mouseMoveTime->start(100000);

    ui->widget->setVisible(true);
    ui->titlewidget->setVisible(true);
    ui->widget_2->move(10,467);
}
void Video_Player::on_timeout_mouserMoveTime()
{
    ui->widget->setVisible(false);
    ui->titlewidget->setVisible(false);
    ui->widget_2->move(10,529);
}

QMap<QString,QFileInfo> Video_Player::get_subtitle_files(QFileInfo fileInfo)
{
    QString complete_file_name=fileInfo.fileName();
    QString pure_file_name=fileInfo.completeBaseName();
    int index=complete_file_name.lastIndexOf(".");
    QString suffix_file_name=complete_file_name.right(complete_file_name.size()-index-1);
    qDebug()<<"file name is :"<<pure_file_name<<"suffix is:"<<suffix_file_name;
    QMap<QString,QFileInfo> tmp_map;
    tmp_map.clear();
    for(int i=0;i<all_file_list.size();i++)
    {
        QFileInfo tmp_file_info=all_file_list.at(i);
        QString complete_tmp_file_name=tmp_file_info.fileName();
        QString tmp_file_info_pure_file_name=tmp_file_info.completeBaseName();
        int index=complete_tmp_file_name.lastIndexOf(".");
        QString tmp_file_info_suffix_file_name=complete_tmp_file_name.right(complete_tmp_file_name.size()-index-1);
         if(suffix_file_name!=tmp_file_info_suffix_file_name&&tmp_file_info_pure_file_name==pure_file_name&&
                 (suffix_file_name=="sub"||suffix_file_name=="idx"||suffix_file_name=="dvdsub"||suffix_file_name=="pgs"
                  ||suffix_file_name=="divx"||suffix_file_name=="dvdsub"||suffix_file_name=="txt"
                  ||suffix_file_name=="timetext"||suffix_file_name=="ssa"||suffix_file_name=="smi"||suffix_file_name=="srt"))
        {
            qDebug()<<"add in map:"<<tmp_file_info.absoluteFilePath();
            tmp_map.insert(tmp_file_info_suffix_file_name,tmp_file_info);
        }else{
            continue;
        }
    }
    return tmp_map;
}
void Video_Player::show_subtitle_file_name(QMap<QString,QFileInfo> tmp_map )
{
    QMap<QString,QFileInfo>::Iterator iter=tmp_map.begin();
    for(;iter!=tmp_map.end();iter++)
    {
        QFileInfo tmp1=iter.value();
        QString tmp2=iter.key();
        qDebug()<<tmp1.absoluteFilePath();
    }
}
