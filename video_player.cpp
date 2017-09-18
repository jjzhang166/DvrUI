#include "video_player.h"
#include "ui_video_player.h"
#include <QDirIterator>
#include "videowidget.h"
#include "ui_videowidget.h"
#include "main_desktop.h"
#include <QFontDatabase>
#include <QPushButton>
#include <QDesktopWidget>

extern QString which_filename_to_play;
extern main_desktop* pStaticMaindesktop;
static int test=100;
const int interval=10;//每次快进10s
Video_Player::Video_Player(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Video_Player)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    qDebug()<<"将要播放的视频名称为："<<which_filename_to_play;
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
    isMuted=false;//初始化为关闭静音
    isPlaying=true;//开始播放
    #if defined(Q_OS_LINUX)
    QDirIterator m_DirIterator(QString("/mnt/sdcard/mmcblk1p1/frontVideo/"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    while (m_DirIterator.hasNext()) {
        QString tempFile=m_DirIterator.next();
        qDebug()<<tempFile;
        QString tempFileName=tempFile;
        tempFileName=tempFileName.remove(QString("/mnt/sdcard/mmcblk1p1/frontVideo/"),Qt::CaseSensitive);
        printf("----------------------the video name is %s\n",(char*)tempFileName.toStdString().c_str());
        if(which_filename_to_play==tempFileName){
            #if defined(Q_OS_LINUX)
                ap = new AUTPlayer();
                astatus = ASTATUS_STOPPED;
                #if defined(USE_AUTPLAYER)
                      int ret = ap->setUrl((char*)tempFile.toStdString().c_str());
                      if (ret < 0)
                        return ;
                      ap->setUserCb(end, this);
                      ap->setViewWindow(0, 0, 1024, 600);
                      int linux_duration=ap->getDuration()/1000;
                      ui->progressSlider->setRange(0,linux_duration);
                      printf("----------------------the video totaltime is %d\n",linux_duration);
                      QString totalTime=QDateTime::fromTime_t(linux_duration).toString("mm:ss");
                      printf("----------------------the video totaltime is %s\n",(char*)totalTime.toStdString().c_str());
                      ap->play();
                      this->duration=linux_duration;
                      qDebug()<<"total time is"<<duration;
                      ui->label->setText(QString(tr("now play:"))+tempFileName);
                      connect(ui->voiceSlider,SIGNAL(valueChanged(int)),this,SLOT(setVolume(int)));
                      connect(ui->progressSlider,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));
                      //每10ms钟获取当前的播放位置，并更新slider和label
                      timer = new QTimer(this);
                      connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
                      timer->start(100);
                #endif
                astatus = ASTATUS_PLAYING;
            #endif
            break;
        }
        else{
            continue;
        }
    }
    //printf("---------------------------------cant find the correct video\n");
    #else
        m_DirIterator=new QDirIterator(QString("E:/tech_practise/DvrUI/DvrUI/video/"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
        while (m_DirIterator->hasNext()) {
            QString tempFile=m_DirIterator->next();
            qDebug()<<tempFile;
            QString tempFileName=tempFile;
            tempFileName=tempFileName.remove(QString("E:/tech_practise/DvrUI/DvrUI/video/"),Qt::CaseSensitive);

            if(which_filename_to_play==tempFileName){
                player = new QMediaPlayer(0,QMediaPlayer::VideoSurface);
                my_video_widget=new QVideoWidget();
                ui->scrollArea->setWidget(my_video_widget);
                player->setVideoOutput(my_video_widget);
                my_video_widget->show();

                player->setMedia(QMediaContent(QUrl::fromLocalFile(tempFile)));
                player->play();
//                ui->playButton->setStyleSheet("QPushButton::{border-image: url(:/icon/Pause.png);}");
                ui->label->setText(QString(tr("当前播放："))+tempFileName);
                ui->progressSlider->setRange(0,player->duration()/1000);
                connect(ui->voiceSlider,SIGNAL(valueChanged(int)),player,SLOT(setVolume(int)));
                connect(ui->progressSlider,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));
                connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
                connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
                break;
            }
            else{
                continue;
            }
        }
    #endif
    qDebug()<<"退出循环";
    connect(this,SIGNAL(main_desktop_visible()),pStaticMaindesktop,SLOT(on_main_desktop_visible()));
}

#if defined(USE_AUTPLAYER)
  int Video_Player::end(int32_t msgType, void *user) //结束MPlayer
  {
    Video_Player *p = (Video_Player*)user;
    //if (msgType == NOTIFY_PLAYBACK_COMPLETE)
    {
      printf("call end \r\n");
      //p->setStyleSheet(QStringLiteral("background-color: rgb(112, 200, 11);"));
    }
  }
#endif
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
    QString cmd="tinymix 1 "+ QString::number(n_value,10);
    system((char*)cmd.toStdString().c_str());
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
    if(isPlaying){
        qDebug()<<"pause video";
        ap->pause();
        ui->playButton->setIcon(QIcon(":/icon/play.png"));
        isPlaying=false;
    }else{
        qDebug()<<"start video";
        ap->play();
        ui->playButton->setIcon(QIcon(":/icon/Pause.png"));
        isPlaying=true;
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
        ui->voiceSlider->setValue(50);
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
    #if defined(Q_OS_LINUX)
        #if defined(USE_AUTPLAYER)
            ap->stop();
        #endif
        astatus = ASTATUS_STOPPED;
    #else
        player->stop();
    #endif
    this->close();
    //    videoWidget* pVideoWidget=static_cast<videoWidget*>(parentWidget());
    //    pVideoWidget->setHidden(false);
    emit p_unhide_moviedesktop();
    emit main_desktop_visible();
}

void Video_Player::on_btnMenu_Max_clicked()
{
    #if defined(Q_OS_LINUX)
        #if defined(USE_AUTPLAYER)
            ap->stop();
        #endif
        astatus = ASTATUS_STOPPED;
    #else
        player->stop();
    #endif
    this->close();
    //    videoWidget* pVideoWidget=static_cast<videoWidget*>(parentWidget());
    //    pVideoWidget->setHidden(false);
    emit p_unhide_moviedesktop();
    emit main_desktop_visible();
}

void Video_Player::on_btnMenu_Close_clicked()
{
    #if defined(Q_OS_LINUX)
        #if defined(USE_AUTPLAYER)
            ap->stop();
        #endif
        astatus = ASTATUS_STOPPED;
    #else
        player->stop();
    #endif
    this->close();
    //    videoWidget* pVideoWidget=static_cast<videoWidget*>(parentWidget());
    //    pVideoWidget->setHidden(false);
    emit p_unhide_moviedesktop();
    emit main_desktop_visible();
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

}

void Video_Player::on_fastBackButton_clicked()
{
#if defined(Q_OS_LINUX)
    //seekto 实现快退
    timer->stop();
    int currentTime=ap->getCurrentPosition()/1000;
    currentTime-=interval;
    ap->seekto(currentTime);
    ui->progressSlider->setValue(currentTime);
    updateDurationInfo(currentTime);
    timer->start(100);
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
    //seekto 实现快进
    timer->stop();
     int currentTime=ap->getCurrentPosition()/1000;
     currentTime+=interval;
     ap->seekto(currentTime);
     ui->progressSlider->setValue(currentTime);
     updateDurationInfo(currentTime);
     timer->start(100);
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

#else
    if(m_DirIterator->hasNext()){
        QString tempFile=m_DirIterator->next();
        qDebug()<<tempFile;
        QString tempFileName=tempFile;
        tempFileName=tempFileName.remove(QString("E:/tech_practise/DvrUI/DvrUI/video/"),Qt::CaseSensitive);
        player->setMedia(QMediaContent(QUrl::fromLocalFile(tempFile)));
        player->play();
        ui->label->setText(QString(tr("当前播放："))+tempFileName);
        ui->progressSlider->setRange(0,player->duration()/1000);
    }
#endif
}

