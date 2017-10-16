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

#include "moviedesk.h"
#include "frmmessagebox.h"
#if defined(Q_OS_LINUX)
extern int is_dir_exist(const char *dir_path);
#endif
#define SEEKTO 1
extern movieDesk* pStaticMovieDesk;
Video_Player* pStaticVideoPlayer=NULL;
static int subtitle_num_now=0;
static int audio_num_now=0;
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
            printf("subtitle show,text=%s\n",pItem->pText);
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
//                    pStaticVideoPlayer->subtitle_label->resize(QSize(1024,61));
                    printf("-----now is text subtitle--------\n");
                    qDebug()<<"the subtitle is: "<<str;
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
const int interval=10;//每次快进10s
Video_Player::Video_Player(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Video_Player)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    qDebug()<<"将要播放的视频名称为："<<fileInfo_to_play.completeBaseName();
    ui->playButton->setIcon(QIcon(":/icon/Pause.png"));
    ui->playButton->setIconSize(QSize(51,51));
    ui->stopButton->setIcon(QIcon(":/icon/Stop.png"));
    ui->stopButton->setIconSize(QSize(51,51));
    ui->muteButton->setIcon(QIcon(":/icon/voice.png"));
    ui->muteButton->setIconSize(QSize(51,51));
    ui->voiceSlider->setRange(0,100);
    ui->voiceSlider->setValue(30);
    show_title();
    FormInCenter();
    current_path=fileInfo_to_play.absolutePath();
    qDebug()<<"current path is:"<<current_path;
#if defined(Q_OS_LINUX)
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
    isMuted=false;//初始化为关闭静音
    isPlaying=true;//开始播放
    seekto_is_ok=1;
    //一段时间没有操作后自动隐藏图标
    ui->widget->setVisible(true);
    ui->widget_2->move(10,467);
    ui->widget_2->setGeometry(10,467,1024,60);
    setMouseTracking(true);
    mouseMoveTime = new QTimer();
    connect(mouseMoveTime,SIGNAL(timeout()),this,SLOT(on_timeout_mouserMoveTime()));
    mouseMoveTime->start(3000);
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
    astatus = ASTATUS_STOPPED;
    QFileInfo file_path_to_play=file_list.at(current_video);
    QString ab_file_path=file_path_to_play.absoluteFilePath();
    qDebug()<<"the file's absoluteFilePath is"<<ab_file_path;
    QString file_name=file_path_to_play.fileName();
    qDebug()<<"the video name: "<<file_name<<"while play";
    #if defined(USE_AUTPLAYER)
          int ret = ap->setUrl((char*)ab_file_path.toStdString().c_str());
          find_correspond_subtitle_file(ab_file_path);
          if (ret < 0)
          {
              qDebug()<<"setUrl is error-------------------";
          }else{
              ap->setUserCb(autCb_func, this);
              ap->setViewWindow(0, 0, 1024, 600);
              int linux_duration=ap->getDuration()/1000;
              ui->progressSlider->setRange(0,linux_duration);
              printf("----------------------the video totaltime is %d\n",linux_duration);
              QString totalTime=QDateTime::fromTime_t(linux_duration).toString("mm:ss");
              printf("----------------------the video totaltime is %s\n",(char*)totalTime.toStdString().c_str());
              ap->play();
              astatus = ASTATUS_PLAYING;
              this->duration=linux_duration;
              qDebug()<<"total time is"<<duration;
              ui->label->setText(QString(tr("now play:"))+file_name);
          }
          connect(ui->voiceSlider,SIGNAL(valueChanged(int)),this,SLOT(setVolume(int)));
          connect(ui->progressSlider,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));

    #endif
    //每10ms钟获取当前的播放位置，并更新slider和label
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(100);
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

        ui->label->setText(QString(tr("当前播放："))+file_name);
        ui->progressSlider->setRange(0,player->duration()/1000);
        connect(ui->voiceSlider,SIGNAL(valueChanged(int)),player,SLOT(setVolume(int)));
        connect(ui->progressSlider,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));
        connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
        connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));

#endif
    qDebug()<<"退出循环";
//    connect(this,SIGNAL(main_desktop_visible()),pStaticMaindesktop,SLOT(on_main_desktop_visible()));
    ui->voiceSlider->setRange(0,200);
    ui->voiceSlider->setValue(100);

}
//外挂字幕的显示
void Video_Player::find_correspond_subtitle_file(QFileInfo fileInfo)
{
    QString pure_file_name=fileInfo.completeBaseName();
    QString suffix_file_name=fileInfo.completeSuffix();
    qDebug()<<"file name is :"<<pure_file_name<<"suffix is:"<<suffix_file_name;
    QMap<QString,QFileInfo> tmp_map;
    tmp_map.clear();
    for(int i=0;i<all_file_list.size();i++)
    {
        QFileInfo tmp_file_info=all_file_list.at(i);
        QString tmp_file_info_pure_file_name=tmp_file_info.completeBaseName();
        QString tmp_file_info_suffix_file_name=tmp_file_info.completeSuffix();
        if(suffix_file_name!=tmp_file_info_suffix_file_name&&tmp_file_info_pure_file_name==pure_file_name)
        {
            qDebug()<<"add in map:"<<tmp_file_info.absoluteFilePath();
            tmp_map.insert(tmp_file_info_suffix_file_name,tmp_file_info);
        }else{
            continue;
        }
    }
    qDebug()<<"this video has the subtitle file is:";
    QMap<QString,QFileInfo>::Iterator iter=tmp_map.begin();
    for(;iter!=tmp_map.end();iter++)
    {
        QFileInfo tmp1=iter.value();
        QString tmp2=iter.key();
        if(tmp2!=suffix_file_name)
            qDebug()<<tmp1.absoluteFilePath();
    }
    int ret=0;
    if(tmp_map.size()==1)
    {
        qDebug()<<"only have 1 subtitle file";
        QString file_name=tmp_map.begin().value().absoluteFilePath();
        qDebug()<<"set the subtitle file is:"<<file_name;

    #if defined(Q_OS_LINUX)
        ret=ap->setsubtitleUrl((char*)file_name.toStdString().c_str());
        if(ret!=0)
            qDebug()<<"subtitle set fail";
    #endif
    }else if(tmp_map.size()==2){
        //这里有个坑，如果一个视频有多个subtitle文件这里需要重写，目前测试一个视频文件只有一个subtitle
        qDebug()<<"have 2 subtitle file";
        QString file_name_1=tmp_map["sub"].absoluteFilePath();
        QString file_name_2=tmp_map["idx"].absoluteFilePath();
    #if defined(Q_OS_LINUX)
        ret=ap->setsubtitleUrlFd((char*)file_name_1.toStdString().c_str(),
                    (char*)file_name_2.toStdString().c_str());
        if(ret!=0)
            qDebug()<<"subtitle set fail";
    #endif
    }else if(tmp_map.size()==0){
        qDebug()<<"can't find this video's subtitle";
    }
}

  //设置窗口为透明的，重载了paintEvent
void Video_Player::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(0, 0, 10, 90));  //QColor最后一个参数80代表背景的透明度
}

Video_Player::~Video_Player()
{
    delete ui;
    #if defined(USE_AUTPLAYER)
        delete ap;
        astatus = ASTATUS_STOPPED;
    #endif
}

void Video_Player::setVolume(int n_value)
{
#if defined(Q_OS_LINUX)
    qDebug()<<"player ready to change the voice";
    //目前设置的是headphone的音量，
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
    //如果播放到末尾，那么将按钮设置为play 然后更新progressslider和nowTimeLabel为初始值
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
    qDebug()<<"ready to seekto"<<seconds<<"s";
    timer->stop();
    int currentTime=ap->getCurrentPosition()/1000;
    astatus = ASTATUS_SEEKING;
    ap->seekto(seconds);
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
#if defined(USE_AUTPLAYER)
    ap->pause();
#endif
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
        ap->stop();
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
//        ap->setMuted(true);
        ui->muteButton->setIcon(QIcon(":/icon/mute.png"));
        ui->voiceSlider->setValue(0);
        isMuted=true;
    }else{
        qDebug()<<"now is muted";
//        ap->setMuted(false);
        ui->voiceSlider->setValue(50);
        ui->muteButton->setIcon(QIcon(":/icon/voice.png"));
        isMuted=false;
    }
#endif
}

void Video_Player::on_btnMenu_Min_clicked()
{
    //实现QMenu选择字幕和音频文件
    m_menu=new QMenu();
    m_subtitleSelect=new QAction(m_menu);
    m_audioFileSelect=new QAction(m_menu);
    m_subtitleSelect->setText(QObject::tr("字幕选择"));
    m_audioFileSelect->setText(QObject::tr("音频选择"));
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
        QString str=QString(tr("字幕%1")).arg(i+1);
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
        QString str=QString(tr("音频%1")).arg(i+1);
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
        #if defined(USE_AUTPLAYER)
            ap->stop();
        #endif
        astatus = ASTATUS_STOPPED;
        timer->stop();
    #else
        player->stop();
    #endif
    this->close();
        pStaticMaindesktop->setHidden(false);
        pStaticMovieDesk->setHidden(false);
}

void Video_Player::on_btnMenu_Close_clicked()
{

#if defined(Q_OS_LINUX)
    #if defined(USE_AUTPLAYER)
        ap->stop();
    #endif
    astatus = ASTATUS_STOPPED;
    timer->stop();
#else
    player->stop();
#endif
this->close();
    pStaticMaindesktop->setHidden(false);
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
//窗体居中显示
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
#if defined(Q_OS_LINUX)
    if(current_video-1>=0){
        ap->stop();
        timer->stop();
        astatus = ASTATUS_STOPPED;
        if(ap!=NULL)
            delete ap;
        ap = new AUTPlayer();
        if(!is_dir_exist((char*)current_path.toStdString().c_str()))
        {
            file_list=GetFileList(dir);
            QFileInfo file_path_to_play=file_list.at(--current_video);
            QString ab_file_path=file_path_to_play.absoluteFilePath();
            QString file_name=file_path_to_play.fileName();

            int ret = ap->setUrl((char*)ab_file_path.toStdString().c_str());
            find_correspond_subtitle_file(ab_file_path);
            if (ret < 0)
              return ;
              ap->setUserCb(autCb_func, this);
            ap->setViewWindow(0, 0, 1024, 600);
            int linux_duration=ap->getDuration()/1000;
            ui->progressSlider->setRange(0,linux_duration);
            printf("----------------------the video totaltime is %d\n",linux_duration);
            QString totalTime=QDateTime::fromTime_t(linux_duration).toString("mm:ss");
            printf("----------------------the video totaltime is %s\n",(char*)totalTime.toStdString().c_str());
            ap->play();
            this->duration=linux_duration;
            qDebug()<<"total time is"<<duration;
            ui->label->setText(QString(tr("now play:"))+file_name);
            timer->start(100);
            astatus = ASTATUS_PLAYING;
            ui->playButton->setIcon(QIcon(":/icon/Pause.png"));
        }
    }
#else
    if(current_video-1>=0){
        file_list=GetFileList(dir);
        QFileInfo file_path_to_play=file_list.at(--current_video);
        QString ab_file_path=file_path_to_play.absoluteFilePath();
        QString file_name=file_path_to_play.fileName();
        player->setMedia(QMediaContent(QUrl::fromLocalFile(ab_file_path)));
        player->play();

        ui->label->setText(QString(tr("当前播放："))+file_name);
        ui->progressSlider->setRange(0,player->duration()/1000);
    }

#endif
}

void Video_Player::on_fastBackButton_clicked()
{
#if defined(Q_OS_LINUX)

//    int nowTime=ap->getCurrentPosition()/1000;//防止在视频刚开始时快退导致的问题
//    if(nowTime<20)//如果开始播放小于20s
//    {
//        qDebug()<<"------------------begin play less than 20s";
//        ap->seekto(1);//就把视频定位到1s
//        updateDurationInfo(1);
//        return ;
//    }
//    qDebug()<<"------------------begin play lager than 20s";
    mouseMoveTime->stop();
    timer->stop();
#if defined(SEEKTO)
    //seekto 实现快退
    qDebug()<<"-----------------now is seekto";
    int currentTime=ap->getCurrentPosition()/1000;

    if(seekto_is_ok==1)
    {
        qDebug()<<"-------------------now seek to is ok";
        currentTime-=interval;
        ap->seekto(currentTime);
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
    mouseMoveTime->start(3000);
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
    //seekto 实现快进
    qDebug()<<"-----------------now is seekto";
    int currentTime=ap->getCurrentPosition()/1000;
    if(seekto_is_ok==1)
    {
        qDebug()<<"-------------------now seek to is ok";
        currentTime+=interval;
        if(currentTime<duration)
        {
            ap->seekto(currentTime);
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
     mouseMoveTime->start(3000);
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
#if defined(Q_OS_LINUX)
    if(current_video+1<file_list.size()){
        ap->stop();
        timer->stop();
        astatus = ASTATUS_STOPPED;
        if(ap!=NULL)
            delete ap;
        ap = new AUTPlayer();
        if(!is_dir_exist((char*)current_path.toStdString().c_str()))
        {
            file_list=GetFileList(dir);
            QFileInfo file_path_to_play=file_list.at(++current_video);
            QString ab_file_path=file_path_to_play.absoluteFilePath();
            QString file_name=file_path_to_play.fileName();

            int ret = ap->setUrl((char*)ab_file_path.toStdString().c_str());
            find_correspond_subtitle_file(ab_file_path);
            if (ret < 0)
              return ;
              ap->setUserCb(autCb_func, this);
            ap->setViewWindow(0, 0, 1024, 600);
            int linux_duration=ap->getDuration()/1000;
            ui->progressSlider->setRange(0,linux_duration);
            printf("----------------------the video totaltime is %d\n",linux_duration);
            QString totalTime=QDateTime::fromTime_t(linux_duration).toString("mm:ss");
            printf("----------------------the video totaltime is %s\n",(char*)totalTime.toStdString().c_str());
            ap->play();
            this->duration=linux_duration;
            qDebug()<<"total time is"<<duration;
            ui->label->setText(QString(tr("now play:"))+file_name);
            astatus = ASTATUS_PLAYING;
            timer->start(100);
            ui->playButton->setIcon(QIcon(":/icon/Pause.png"));
        }

    }
#else
    if(current_video+1<file_list.size()){
        file_list=GetFileList(dir);
        QFileInfo file_path_to_play=file_list.at(++current_video);
        QString ab_file_path=file_path_to_play.absoluteFilePath();
        QString file_name=file_path_to_play.fileName();
        player->setMedia(QMediaContent(QUrl::fromLocalFile(ab_file_path)));
        player->play();
        ui->label->setText(QString(tr("当前播放："))+file_name);
        ui->progressSlider->setRange(0,player->duration()/1000);
    }

#endif
}
QFileInfoList Video_Player::GetFileList(QDir dir)
{
    qDebug()<<"get all video file name";
    QStringList filters;
    filters << "*.mp4" << "*.avi"<<"*.mkv"<<"*.mp3";
    dir.setNameFilters(filters);
    QFileInfoList file_list=dir.entryInfoList();
    for(int i=0;i<file_list.size();i++)
    {
        QFileInfo fileInfo=file_list.at(i);
        fileInfo.absoluteFilePath();
        qDebug()<<fileInfo.fileName();
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
        qDebug()<<fileInfo.fileName();
    }
    return all_file_list;
}
//重写QWidget的4个方法来保证有动作时显示，没动作时隐藏
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
//有操作出现
void Video_Player::on_mouse_active()
{
    mouseMoveTime->stop();
    mouseMoveTime->start();

    ui->widget->setVisible(true);
    ui->widget_2->move(10,467);
}
void Video_Player::on_timeout_mouserMoveTime()
{
    ui->widget->setVisible(false);
    ui->widget_2->move(10,529);
}


