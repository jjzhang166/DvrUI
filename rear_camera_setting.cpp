#include "rear_camera_setting.h"
#include "ui_rear_camera_setting.h"
#include "preview.h"
#include "ui_preview.h"
#include <QDesktopWidget>
Rear_Camera_Setting::Rear_Camera_Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rear_Camera_Setting)
{
    ui->setupUi(this);
    FormInCenter();
//    connect(ui->returnButton,SIGNAL(clicked(bool)),this,SLOT(on_returnButton_clicked(bool)));
//    connect(ui->upperButton,SIGNAL(clicked(bool)),this,SLOT(on_upperButton_clicked(bool)),Qt::UniqueConnection);
//    connect(ui->downButton,SIGNAL(clicked(bool)),this,SLOT(on_downButton_clicked(bool)),Qt::UniqueConnection);
//    connect(ui->sureButton,SIGNAL(clicked(bool)),this,SLOT(on_sureButton_clicked(bool)),Qt::UniqueConnection);
}
//窗体居中显示
void Rear_Camera_Setting::FormInCenter()
{
    int frmX = this->width();
    int frmY = this->height();
    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    this->move(movePoint);
}
Rear_Camera_Setting::~Rear_Camera_Setting()
{
    delete ui;
}
void Rear_Camera_Setting::on_returnButton_clicked(bool)
{
    this->close();
    emit unHideSettings();
}
void Rear_Camera_Setting::on_upperButton_clicked(bool)
{
    qDebug()<<"rear camera upper";
}
void Rear_Camera_Setting::on_downButton_clicked(bool)
{
    qDebug()<<"rear camera down";
}
void Rear_Camera_Setting::on_sureButton_clicked(bool)
{
    qDebug()<<"sure to adjust camera";
    this->close();
}
