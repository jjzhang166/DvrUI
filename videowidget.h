#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QDialog>
#include <QDebug>
#include <QListWidget>
#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QDirIterator>
#include <QRect>
#include <QProcess>
#include "video_player.h"
#if defined(Q_OS_LINUX)
#define AUTEVENT_TEST 1
#endif

namespace Ui {
class videoWidget;
}

class videoWidget : public QDialog
{
    Q_OBJECT

public:
    explicit videoWidget(QWidget *parent = 0);
    ~videoWidget();
    void show_file_by_iconview(QFileInfoList file_list);
    void show_file_by_listview(QFileInfoList file_list);
    void gen_shot_picture(QString tempFileName_NoSuffix,QString& file_path,QString type);
    QFileInfoList GetFileList(QDir dir);
    void show_file();
public slots:
    void play_video(QModelIndex pos);
private slots:
//    void on_change_viewmode_clicked();
signals:
    void hide_moviedesktop();
    void on_unhide_moviedesktop();
    void main_desktop_disvisible();
public slots:
    void deal_picture_views_signal();

    void on_usb_mount();
    void on_usb_umount();

private:
    Ui::videoWidget *ui;
    Video_Player* video_players;
    bool show_model;
//    QDirIterator* m_DirIterator;
    QFileInfoList file_list;

};

#endif // VIDEOWIDGET_H
