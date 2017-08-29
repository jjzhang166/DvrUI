#include "rear_camera_setting.h"
#include "ui_rear_camera_setting.h"
#include "preview.h"
#include "ui_preview.h"

Rear_Camera_Setting::Rear_Camera_Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rear_Camera_Setting)
{
    ui->setupUi(this);
//    connect(ui->returnButton,SIGNAL(clicked(bool)),this,SLOT(on_returnButton_clicked(bool)));
//    connect(ui->upperButton,SIGNAL(clicked(bool)),this,SLOT(on_upperButton_clicked(bool)),Qt::UniqueConnection);
//    connect(ui->downButton,SIGNAL(clicked(bool)),this,SLOT(on_downButton_clicked(bool)),Qt::UniqueConnection);
//    connect(ui->sureButton,SIGNAL(clicked(bool)),this,SLOT(on_sureButton_clicked(bool)),Qt::UniqueConnection);
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
    qDebug()<<"后置摄像头向上调整";
}
void Rear_Camera_Setting::on_downButton_clicked(bool)
{
    qDebug()<<"后置摄像头向下调";
}
void Rear_Camera_Setting::on_sureButton_clicked(bool)
{
    qDebug()<<"确认对后置摄像头的调整";
    this->close();
}
