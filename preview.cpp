#include "preview.h"
#include "ui_preview.h"

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
    rear_camera_setting=new Rear_Camera_Setting();
    rear_camera_setting->exec();

}
void Preview::on_frontButton_clicked(bool)
{
    front_camera_setting=new Front_camera_setting();
    front_camera_setting->exec();
}
