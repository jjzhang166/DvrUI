#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->ROM_usage->setRange(0,100);
    connect(ui->formatButton,SIGNAL(clicked(bool)),this,SLOT(on_formatButton_clicked()));
    /******
     * 生成用于测试的随机数
     *****/
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    test =qrand()%100;
    qDebug()<<"随机数"<<test;
    ui->ROM_usage->setValue(test);
}

About::~About()
{
    delete ui;
}
void About::on_formatButton_clicked()
{
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(set_pro_value()));
    timer->start(10);
}
void About::set_pro_value()
{
    if(test>=0){
        ui->ROM_usage->setValue(test--);
    }else{
        timer->stop();
    }
}
