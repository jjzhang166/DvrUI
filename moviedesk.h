#ifndef MOVIEDESK_H
#define MOVIEDESK_H

#include <QDialog>
#include <QDesktopWidget>
#include<QFileDialog>
//#include "editwidget.h"
#include "videowidget.h"
#include "picturewidget.h"
#include "settings.h"
#include "slave_picturewidget.h"
#include "slave_videowidget.h"
#include "video_player.h"
#include "picture_view.h"

namespace Ui {
class movieDesk;
}

class movieDesk : public QDialog
{
    Q_OBJECT

public:
    explicit movieDesk(QWidget *parent = 0);
    ~movieDesk();
public slots:
    void on_returnButton_clicked();
    void on_mainCameraButton_clicked();
    void on_viceButton_clicked();
private:
    Ui::movieDesk *ui;
    int cur_index;
private:
//    editWidget* editwidget;
    videoWidget* videowidget;
    pictureWidget* picturewidget;
    slave_PictureWidget* slave_picturewidget;
    slave_VideoWidget* slave_videowidget;
    QFileDialog *fileDialog;
    Video_Player* player;
    Picture_view* pic_view;
    void FormInCenter();
public slots:
    void on_hide_moviedesktop();
    void on_unhide_moviedesktop();
    void open_file(QString);
signals:
    void main_desktop_disvisible();
};
//将tabWidget设置为横向的


#endif // MOVIEDESK_H
