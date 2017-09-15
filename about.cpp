#include "about.h"
#include "ui_about.h"
#include <QMessageBox>
#include <QListView>
#include <QProcess>
#include <QByteArray>
#include "frmmessagebox.h"
About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->ROM_usage->setRange(0,100);
    connect(ui->formatButton,SIGNAL(clicked(bool)),this,SLOT(on_formatButton_clicked()),Qt::UniqueConnection);
#if defined(Q_OS_LINUX)
    QProcess process;
    QString cmd=QString("df /mnt/sdcard/mmcblk1p1 | grep %1").arg(QString("/dev/mmcblk1p1"));
    process.start(cmd);
    process.waitForFinished();
    QByteArray output = process.readAllStandardOutput();
    QString str_output = output;
    str_output=str_output.simplified();
    QStringList list=str_output.split(' ');
    qDebug()<<str_output<<"\n"<<"usage of sdcard"<<list[11];
    QString t=list[11];
    t=t.left(t.length()-1);
    test=t.toInt();
    qDebug()<<test;

#else
    #if 0
        /******
         * 生成用于测试的随机数
         *****/
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        test =qrand()%100;
        qDebug()<<"随机数"<<test;
    #endif
    test=66;
#endif
    ui->ROM_usage->setValue(test);
    ui->languageSelect->setView(new QListView());
}

About::~About()
{
    delete ui;
}
void About::on_formatButton_clicked()
{
//    QMessageBox::StandardButton ret =QMessageBox::question(this,tr("format question"),tr("sure to format?"),QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel);
    frmMessageBox* mesg=new frmMessageBox();
    mesg->SetMessage(QString(tr("确定格式化？")), 1);
    if(1==mesg->exec()){
    #if defined(Q_OS_LINUX)
        pfat=new Fat();
        pfat->format(PATH_SD_DEV, PATH_SDCARD);
    #else
        qDebug()<<"格式化";
    #endif
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
