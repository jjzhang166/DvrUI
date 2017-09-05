#include "video_player.h"
#include "ui_video_player.h"
#include <QDirIterator>
#include "videowidget.h"
#include "ui_videowidget.h"

extern QString which_filename_to_play;

Video_Player::Video_Player(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Video_Player)
{
    ui->setupUi(this);
    player = new QMediaPlayer(0,QMediaPlayer::VideoSurface);
//    my_video_widget=new video_widgets();
    my_video_widget=new QVideoWidget();
    ui->scrollArea->setWidget(my_video_widget);


    ui->progressSlider->setRange(0, player->duration() / 1000);

//    player->setVideoOutput(my_video_widget);
    my_video_widget->show();
    qDebug()<<"将要播放的视频名称为："<<which_filename_to_play;
    QDirIterator m_DirIterator(QString("E:/tech_practise/T3_linux/T3_Linux/T3_Linux/video"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    while (m_DirIterator.hasNext()) {
        QString tempFile=m_DirIterator.next();
        qDebug()<<tempFile;
        QString tempFileName=tempFile;
        tempFileName=tempFileName.remove(QString("E:/tech_practise/T3_linux/T3_Linux/T3_Linux/video/"),Qt::CaseSensitive);

        if(which_filename_to_play==tempFileName){
            player->setMedia(QMediaContent(QUrl::fromLocalFile(tempFile)));

            player->play();
            break;
        }
        else{
            continue;
        }
    }
    qDebug()<<"退出循环";
}

Video_Player::~Video_Player()
{
    delete ui;
}
void Video_Player::seek(int seconds)
{

//    player->setPosition(seconds * 1000);
}

void Video_Player::on_playButton_clicked()
{
    qDebug()<<"start video";
//    player->play();
}

void Video_Player::on_pauseButton_clicked()
{
    qDebug()<<"pause video";
//    player->pause();
}

void Video_Player::on_stopButton_clicked()
{

    qDebug()<<"close video";
//    player->stop();
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
    this->close();
    videoWidget* pVideoWidget=static_cast<videoWidget*>(parentWidget());
    pVideoWidget->setHidden(false);
}
