#include "timesetting.h"
#include "ui_timesetting.h"
#include <QDebug>
TimeSetting::TimeSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeSetting)
{
    ui->setupUi(this);
    ui->dateEdit->setCalendarPopup(true);
    connect(ui->dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(setDatesTimes()));
    connect(ui->timeEdit,SIGNAL(timeChanged(QTime)),this,SLOT(setDatesTimes()));
    QDate nowDate=QDate::currentDate();
    ui->label->setText("当前日期："+nowDate.toString("yyyy-MM-dd"));
}

TimeSetting::~TimeSetting()
{
    delete ui;
}
void TimeSetting::setDatesTimes()
{
    QObject* sender = QObject::sender();
    if(sender==ui->dateEdit){
        qDebug()<<"date changed";
        QDate datesetting=ui->dateEdit->date();
        qDebug()<<datesetting;
    }
    else if(sender==ui->timeEdit){
        qDebug()<<"time changed";
        QTime timesetting=ui->timeEdit->time();
        qDebug()<<timesetting;
    }
    else{
        qDebug()<<"no signal sender";
    }

}
//设定主屏幕时间
//void TimeSetting::accept()//override accept
//{
//    results setting_results;
//    setting_results.date_sets=QDate();
//    setting_results.time_sets=QTime();
//    setting_results.testString="hello world";
//    emit send_data_to_main(setting_results);
//    QDialog::accept();
//}
