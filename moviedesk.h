#ifndef MOVIEDESK_H
#define MOVIEDESK_H

#include <QDialog>
#include "editwidget.h"
#include "videowidget.h"
#include "picturewidget.h"
#include "settings.h"
#include "slave_picturewidget.h"
#include "slave_videowidget.h"
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
private:
    editWidget* editwidget;
    videoWidget* videowidget;
    pictureWidget* picturewidget;
    slave_PictureWidget* slave_picturewidget;
    slave_VideoWidget* slave_videowidget;
};
//将tabWidget设置为横向的


#endif // MOVIEDESK_H
