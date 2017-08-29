#include "preview.h"
#include "ui_preview.h"
#include "settings.h"
#include "ui_settings.h"

Preview::Preview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preview)
{
    ui->setupUi(this);
    ui->pictureLabel1->setStyleSheet(tr("background-image: url(:/frontCamera.png);"));
    ui->pictureLabel2->setStyleSheet(tr("background-image: url(:/rearCamera.png);"));
    connect(ui->front_set_Button,SIGNAL(clicked(bool)),this,SLOT(on_frontButton_clicked(bool)));
    connect(ui->rear_set_Button,SIGNAL(clicked(bool)),this,SLOT(on_rearButton_clicked(bool)));

}

Preview::~Preview()
{
    delete ui;
}
void Preview::on_rearButton_clicked(bool)
{
    rear_camera_setting=new Rear_Camera_Setting(this);
    connect(rear_camera_setting,SIGNAL(unHideSettings()),this,SLOT(on_unhideSettings()));
    rear_camera_setting->showNormal();
    emit hideSettings();

}

void Preview::on_frontButton_clicked(bool)
{

    front_camera_setting=new Front_camera_setting(this);
    connect(front_camera_setting,SIGNAL(unHideSettings()),this,SLOT(on_unhideSettings()));
    front_camera_setting->showNormal();
    emit hideSettings();
}
void Preview::on_unhideSettings()
{
    emit unhideSettings_signal();
}
