#include "video_player.h"
#include "ui_video_player.h"
#include <QDirIterator>
#include "videowidget.h"
#include "ui_videowidget.h"
#include "main_desktop.h"
extern QString which_filename_to_play;
extern main_desktop* pStaticMaindesktop;

Video_Player::Video_Player(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Video_Player)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    qDebug()<<"将要播放的视频名称为："<<which_filename_to_play;
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
                      ap->play();
                #endif
                astatus = ASTATUS_PLAYING;
            #endif
            break;
        }
        else{
            continue;
        }
    }
    printf("---------------------------------cant find the correct video\n");
    #else
        QDirIterator m_DirIterator(QString("E:/tech_practise/DvrUI/DvrUI/video/"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
        while (m_DirIterator.hasNext()) {
            QString tempFile=m_DirIterator.next();
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
void Video_Player::seek(int seconds)
{

//    player->setPosition(seconds * 1000);
}

void Video_Player::on_playButton_clicked()
{
    qDebug()<<"start video";
#if defined(Q_OS_LINUX)
    #if defined(USE_AUTPLAYER)
        ap->pause();
    #endif
        if (astatus == ASTATUS_PAUSED)
        {
          astatus = ASTATUS_PLAYING;
        }
        else
        {
          astatus = ASTATUS_PAUSED;

        }
#else
    player->play();
#endif
}

void Video_Player::on_pauseButton_clicked()
{
    qDebug()<<"pause video";
//    player->pause();
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
    qDebug()<<"set muted";
}

void Video_Player::on_voiceSlider_sliderMoved(int position)
{

}

void Video_Player::on_progressSlider_sliderMoved(int position)
{

}

void Video_Player::on_pushButton_clicked()
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
//scrollArea->setStyleSheet("QScrollArea {background-color:transparent;}")

//scrollArea->viewport()->setStyleSheet("background-color:transparent;");
