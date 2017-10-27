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
    void FormInCenter();
public slots:
    void on_returnButton_clicked();
    void on_mainCameraButton_clicked();
    void on_viceButton_clicked();
    void deal_fileDialog(int a);
    void deal_fileDialog1();
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
    bool isMainCamera;
    void on_usb_mount();
    void on_usb_umount();
public slots:
    void on_hide_moviedesktop();
    void on_unhide_moviedesktop();
    void open_file(QString);
signals:
    void main_desktop_disvisible();
private slots:
    void on_videoButton_clicked();
    void on_picButton_clicked();
    void on_scanButton_clicked();
};



#endif // MOVIEDESK_H
