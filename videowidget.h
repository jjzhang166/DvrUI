#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QDialog>
#include <QDebug>
#include <QListWidget>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QRect>
#include <QProcess>
#include "video_player.h"
namespace Ui {
class videoWidget;
}

class videoWidget : public QDialog
{
    Q_OBJECT

public:
    explicit videoWidget(QWidget *parent = 0);
    ~videoWidget();
public slots:
    void play_video(QModelIndex pos);
private:
    Ui::videoWidget *ui;
    Video_Player* video_players;

};

#endif // VIDEOWIDGET_H
