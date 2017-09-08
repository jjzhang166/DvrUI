#include "front_camera_setting.h"
#include "ui_front_camera_setting.h"
#include "preview.h"
#include "ui_preview.h"
#include <QDesktopWidget>
Front_camera_setting::Front_camera_setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Front_camera_setting)
{
    ui->setupUi(this);
    FormInCenter();
    connect(ui->returnButton,SIGNAL(clicked(bool)),this,SLOT(on_returnButton_clicked(bool)));
    connect(ui->upperButton,SIGNAL(clicked(bool)),this,SLOT(on_upperButton_clicked(bool)),Qt::UniqueConnection);
    connect(ui->downButton,SIGNAL(clicked(bool)),this,SLOT(on_downButton_clicked(bool)),Qt::UniqueConnection);
    connect(ui->sureButton,SIGNAL(clicked(bool)),this,SLOT(on_sureButton_clicked(bool)),Qt::UniqueConnection);
}
void Front_camera_setting::FormInCenter()
{
    int frmX = this->width();
    int frmY = this->height();
    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    this->move(movePoint);
}
Front_camera_setting::~Front_camera_setting()
{
    delete ui;
}
void Front_camera_setting::on_returnButton_clicked(bool)
{
    this->close();
    emit unHideSettings();
}
void Front_camera_setting::on_upperButton_clicked(bool)
{
    qDebug()<<"前置摄像头向上调整";
}
void Front_camera_setting::on_downButton_clicked(bool)
{
    qDebug()<<"前置摄像头向下调";
}
void Front_camera_setting::on_sureButton_clicked(bool)
{
    qDebug()<<"确认对前置摄像头的调整";
    this->close();
}
