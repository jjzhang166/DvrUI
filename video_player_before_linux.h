#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <QDialog>
#include <QDebug>
#include <QMediaPlayer>
#include "video_widgets.h"
#include <QMediaObject>
namespace Ui {
class Video_Player;
}

class Video_Player : public QDialog
{
    Q_OBJECT

public:
    explicit Video_Player(QWidget *parent = 0);
    ~Video_Player();
public slots:
    void seek(int seconds);

private slots:
    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_stopButton_clicked();

    void on_muteButton_clicked();

    void on_voiceSlider_sliderMoved(int position);

    void on_progressSlider_sliderMoved(int position);

    void on_pushButton_clicked();

private:
    Ui::Video_Player *ui;
//    QMediaPlayer::State playerState;
//    video_widgets* my_video_widget;
    QVideoWidget* my_video_widget;
    QMediaPlayer *player;
};

#endif // VIDEO_PLAYER_H
