#include "reverseline.h"
#include "ui_reverseline.h"

ReverseLine::ReverseLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReverseLine)
{
    ui->setupUi(this);
    reverse_setting_label=new myLabel();
    reverse_setting_label->setStyleSheet(tr("background-image: url(:/reverseline2.png);"));
    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(reverse_setting_label);
    ui->groupBox_2->setLayout(vbox2);
    connect(reverse_setting_label,SIGNAL(clicked()),this,SLOT(on_pictureLabel2_clicked()));
    rearVision_setting_label=new myLabel();
    rearVision_setting_label->setStyleSheet(tr("background-image: url(:/reverseline1.png);"));
    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(rearVision_setting_label);
    ui->groupBox->setLayout(vbox1);
    connect(rearVision_setting_label,SIGNAL(clicked()),this,SLOT(on_pictureLabel1_clicked()));
}

ReverseLine::~ReverseLine()
{
    delete ui;
}
void ReverseLine::on_pictureLabel2_clicked()
{
    qDebug()<<"打开倒车线设置界面";
}
void ReverseLine::on_pictureLabel1_clicked()
{
    qDebug()<<"打开后视预览界面";
}

