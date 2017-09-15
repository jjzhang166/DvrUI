#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <QDialog>
#include <QDebug>
#include <QMediaPlayer>
#include "video_widgets.h"
#include <QMediaObject>
#include <QPainter>
#include <QTimer>
#if defined(Q_OS_LINUX)
#define USE_AUTPLAYER 1
#endif
#if defined(USE_AUTPLAYER)
#include "AutPlayer.h"
using namespace android;
#endif
static const int ASTATUS_STOPPED   = 0;
static const int ASTATUS_PREPARING = 1;
static const int ASTATUS_PREPARED  = 2;
static const int ASTATUS_PLAYING   = 3;
static const int ASTATUS_PAUSED    = 4;
static const int ASTATUS_SEEKING   = 5;

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

    void on_stopButton_clicked();

    void on_muteButton_clicked();

    void on_btnMenu_Min_clicked();

    void on_btnMenu_Max_clicked();

    void on_btnMenu_Close_clicked();

    void durationChanged(qint64 duration);

    void positionChanged(qint64 progress);

    void updateDurationInfo(qint64 currentInfo);

    void setVolume(int);

    void timerUpdate();

    void on_preMovieButton_clicked();

    void on_fastBackButton_clicked();

    void on_fastFrontButton_clicked();

    void on_nextMovieButton_clicked();

protected:
    void paintEvent(QPaintEvent *event);
private:
    Ui::Video_Player *ui;
    bool isMuted;
    bool isPlaying;
    qint64 duration;//时间表时
    QTimer *timer;
//    QMediaPlayer::State playerState;
//    video_widgets* my_video_widget;
    #if defined(USE_AUTPLAYER)
        AUTPlayer *ap;
    #else
        QVideoWidget* my_video_widget;
        QMediaPlayer *player;
    #endif
    #if defined(USE_AUTPLAYER)
        static int end(int32_t msgType, void *user);
        int astatus;
    #endif

signals:
    void p_unhide_moviedesktop();
    void main_desktop_visible();
private:
    void show_title();
    void FormInCenter();
};

#endif // VIDEO_PLAYER_H
