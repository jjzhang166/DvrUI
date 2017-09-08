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
    void show_file_by_iconview(QDirIterator* m_DirIterator);
    void show_file_by_listview(QDirIterator* m_DirIterator);
    void gen_shot_picture(QString tempFileName_NoSuffix,QString& file_path,QString type);
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
private:
    Ui::videoWidget *ui;
    Video_Player* video_players;
    bool show_model;
    QDirIterator* m_DirIterator;

};

#endif // VIDEOWIDGET_H
