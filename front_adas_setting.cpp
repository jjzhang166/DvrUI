#include "front_adas_setting.h"
#include "ui_front_adas_setting.h"
#include "adas.h"
#include "ui_adas.h"
front_ADAS_setting::front_ADAS_setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::front_ADAS_setting)
{
    ui->setupUi(this);

    this->horizontal_values=0;
    this->model=0;
    this->vertical_values=0;

    ui->VerticelLine->setLineWidth(1);
    ui->HorizontalLine->setLineWidth(1);
    ui->left_right_Slider->setRange(0,80);
    ui->high_low_Slider->setRange(0,60);
//    ui->HorizontalLine->setStyleSheet(tr("background-color:red"));
    connect(ui->left_right_Slider,SIGNAL(valueChanged(int)),this,SLOT(change_line_value(int)));
    connect(ui->high_low_Slider,SIGNAL(valueChanged(int)),this,SLOT(change_line_value(int)));
}

front_ADAS_setting::~front_ADAS_setting()
{
    delete ui;
}
void front_ADAS_setting::on_returnButton_clicked()
{
    this->close();
    emit unHideSettings();
}
void front_ADAS_setting::on_nextButton_clicked()
{
    qDebug()<<"设置完成执行下一步";
    this->close();
}
void front_ADAS_setting::change_line_value(int value)
{
    qDebug()<<"滑块值的变化为："<<value;
    QObject* sender=QObject::sender();
    if(sender==ui->left_right_Slider){
        qDebug()<<"横滑块变化";
        this->horizontal_values=3*value;
        ui->VerticelLine->move(10+this->horizontal_values,40);
        this->model=2;
//        update();
    }else if(sender==ui->high_low_Slider){
        qDebug()<<"竖滑块变化";
        this->vertical_values=3*value;
        this->model=1;
        ui->HorizontalLine->move(0,260-this->vertical_values);
//        update();
    }else{
        qDebug()<<"无信号检测到";
    }

}
