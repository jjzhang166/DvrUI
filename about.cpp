#include "about.h"
#include "ui_about.h"
#include <QMessageBox>
#include <QListView>
About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->ROM_usage->setRange(0,100);
    connect(ui->formatButton,SIGNAL(clicked(bool)),this,SLOT(on_formatButton_clicked()),Qt::UniqueConnection);
    /******
     * 生成用于测试的随机数
     *****/
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    test =qrand()%100;
    qDebug()<<"随机数"<<test;
    ui->ROM_usage->setValue(test);
//    ui->languageSelect->setStyleSheet("QComboBox{border:1px solid rgb(100, 100, 100);border-radius: 4px;}"
//      "QComboBox QAbstractItemView::item{height:40px;}"
//      "QComboBox::down-arrow{image:url(:/icon/arrowdown.png);}"
//      "QComboBox::drop-down{border:0px;}");
      ui->languageSelect->setView(new QListView());
}

About::~About()
{
    delete ui;
}
void About::on_formatButton_clicked()
{
    QMessageBox::StandardButton ret =QMessageBox::question(this,tr("format question"),tr("sure to format?"),QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel);
    if(ret==QMessageBox::Ok){
        timer=new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(set_pro_value()));
        timer->start(10);
    }
}
void About::set_pro_value()
{
    if(test>=0){
        ui->ROM_usage->setValue(test--);
    }else{
        timer->stop();
    }
}
