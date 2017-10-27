#include "adas.h"
#include "ui_adas.h"

ADAS::ADAS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ADAS)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    front_ADAS_label=new myLabel();
    front_ADAS_label->setBaseSize(60,20);
    front_ADAS_label->setStyleSheet(tr("background-image: url(:/frontCamera.png);"));
    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addSpacing(25);
    vbox2->addWidget(front_ADAS_label);
    ui->groupBox->setLayout(vbox2);
    connect(front_ADAS_label,SIGNAL(clicked()),this,SLOT(on_pictureLabel2_clicked()));
    rear_ADAS_label=new myLabel();
    rear_ADAS_label->setBaseSize(60,20);
    rear_ADAS_label->setStyleSheet(tr("background-image: url(:/rearCamera.png);"));
    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addSpacing(25);
    vbox1->addWidget(rear_ADAS_label);
    ui->groupBox_2->setLayout(vbox1);
    connect(rear_ADAS_label,SIGNAL(clicked()),this,SLOT(on_pictureLabel1_clicked()));
}

ADAS::~ADAS()
{
    delete ui;
}
void ADAS::on_pictureLabel1_clicked()
{
    qDebug()<<"open adas ";
    rear_adas_settings=new rear_ADAS_setting(this);
    connect(rear_adas_settings,SIGNAL(unHideSettings()),this,SLOT(on_unhideSettings()));
    emit hideSettings();
    rear_adas_settings->showNormal();
}
void ADAS::on_pictureLabel2_clicked()
{
    qDebug()<<"open front adas";
    front_adas_settings=new front_ADAS_setting(this);
    connect(front_adas_settings,SIGNAL(unHideSettings()),this,SLOT(on_unhideSettings()));
    emit hideSettings();
    front_adas_settings->showNormal();
}
void ADAS::on_unhideSettings()
{
    emit unhideSettings_signal();
}
