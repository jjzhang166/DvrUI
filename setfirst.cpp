#include "setfirst.h"
#include "ui_setfirst.h"
#include <QDesktopWidget>
#if defined(Q_OS_LINUX)
#include <ls_ctrl.h>
#include <asoundlib.h>
#endif
extern int recordTime;
//设置开机录像，录音等功能
extern int open_recordVideo_front,open_recordVideo_rear;
extern int open_recordAudio_front,open_recordAudio_rear;
extern int open_reverseLine_front,open_reverseLine_rear;
extern int open_adas_front,open_adas_rear;
SetFirst::SetFirst(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetFirst)
{
    ui->setupUi(this);
//    int w,h;
//    w=(1024-this->width())/2;
//    h=(600-this->height())/2;
//    this->mapToParent(QPoint(w,h));
    FormInCenter();
    //设置界面的样式
    setWindowStyleSheet();
    ui->comboBox->setView(new QListView());
    voiceButtonState=true;
    //连接前后两级页面
    connect(ui->settingsButton,SIGNAL(clicked(bool)),this,SLOT(on_click_setttingsButton(bool)));
    connect(ui->returnButton,SIGNAL(clicked(bool)),this,SLOT(on_click_returnButton(bool)));
    //设置音量和亮度调节
    //设置滚动条和显示数字联动
    //亮度的调节范围为0-20
    //声音的调节范围为0-100
#if defined(Q_OS_LINUX)
    lcd_blk_ctrl_init();
#endif
    ui->voiceSlider->setRange(0,100);
    ui->lightSlider->setRange(0,20);
    connect(ui->voiceSlider,SIGNAL(valueChanged(int)),this,SLOT(on_slider_valuechanged(int)));
    connect(ui->lightSlider,SIGNAL(valueChanged(int)),this,SLOT(on_slider_valuechanged(int)));
    ui->voiceSlider->setValue(50);
    ui->lightSlider->setValue(10);
    ui->voiceLabel->setText(tr("50"));
    ui->lightLabel->setText(tr("10"));

    //设置QSpinBox设置录像时间
    ui->movieTimeSetting->setRange(1,5);
    ui->movieTimeSetting->setSingleStep(2);
    ui->movieTimeSetting->setValue(1);
    ui->movieTimeSetting->setSuffix(tr("分钟"));

    //设置四个按钮的样式:按下和弹起操作
    ui->audioButton->setCheckable(true);
    ui->ADASButton->setCheckable(true);
    ui->reverseButton->setCheckable(true);
    ui->movieButton->setCheckable(true);
    ui->voiceButton->setCheckable(true);


    if(open_adas_front){
        ui->ADASButton->setChecked(true);
    }else{
        ui->ADASButton->setChecked(false);
    }
    if(open_recordVideo_front){
        ui->movieButton->setChecked(true);
    }else{
        ui->movieButton->setChecked(false);
    }
    if(open_recordAudio_front){
        ui->audioButton->setChecked(true);
    }else{
        ui->audioButton->setChecked(false);
    }
    if(open_reverseLine_front){
        ui->reverseButton->setChecked(true);
    }else{
        ui->reverseButton->setChecked(false);
    }

}
SetFirst::~SetFirst()
{
    delete ui;
}
//窗体居中显示
void SetFirst::FormInCenter()
{
    int frmX = this->width();
    int frmY = this->height();
    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    this->move(movePoint);
}
void SetFirst::on_click_setttingsButton(bool)
{
    setSecond_Desk=new Settings(this);
    this->hide();
    setSecond_Desk->showNormal();
}
void SetFirst::on_click_returnButton(bool)
{
    this->close();
}
//windows下可以显示选中的状态，linux下显示不出
void SetFirst::on_slider_valuechanged(int n_value)
{
    QObject* sender = QObject::sender();
    if(sender==ui->voiceSlider){
        qDebug()<<"change the voice"<<n_value;
        ui->voiceButton->setChecked(false);
    #if defined(Q_OS_LINUX)
        QString cmd="tinymix 1 "+ QString::number(n_value,10);
        system((char*)cmd.toStdString().c_str());
        ui->voiceLabel->setText(QString("%1").arg(n_value));
    #endif
    }
    else if(sender==ui->lightSlider){
        qDebug()<<"change the light:"<<n_value;
    #if defined(Q_OS_LINUX)
        lcd_blk_ctrl(n_value);
    #endif
        ui->lightLabel->setText(QString("%1").arg(n_value));
    }
    else{
        qDebug()<<"没有收到声音和亮度调节的信号";
    }
}
void SetFirst::on_audioButton_clicked()
{
    if(ui->audioButton->isChecked()){
        qDebug()<<"录音按钮处于按下状态";
    #if defined(Q_OS_LINUX)
        config_set_recordAudio(0,1);
    #endif
    }
    else{
        qDebug()<<"录音按钮处于弹起状态";
    #if defined(Q_OS_LINUX)
        config_set_recordAudio(0,0);
    #endif
    }
}
void SetFirst::on_ADASButton_clicked()
{
    if(ui->ADASButton->isChecked()){
        qDebug()<<"ADAS按钮处于按下状态";
    #if defined(Q_OS_LINUX)
        config_set_adas(0,1);
    #endif
    }
    else{
        qDebug()<<"ADAS按钮处于弹起状态";
    #if defined(Q_OS_LINUX)
        config_set_adas(0,0);
    #endif
    }
}
void SetFirst::on_movieButton_clicked()
{
    if(ui->movieButton->isChecked()){
        qDebug()<<"录像按钮处于按下状态";
    #if defined(Q_OS_LINUX)
        config_set_recordVideo(0,1);
    #endif
    }
    else{
        qDebug()<<"录像按钮处于弹起状态";
    #if defined(Q_OS_LINUX)
        config_set_recordVideo(0,0);
    #endif
    }
}
void SetFirst::on_reverseButton_clicked()
{
    if(ui->reverseButton->isChecked()){
        qDebug()<<"倒车线按钮处于按下状态";
    #if defined(Q_OS_LINUX)
        config_set_reverseLine(0,1);
    #endif
    }
    else{
        qDebug()<<"倒车线按钮处于弹起状态";
    #if defined(Q_OS_LINUX)
        config_set_reverseLine(0,0);
    #endif
    }
}

void SetFirst::on_voiceButton_clicked()
{
    if(!voiceButtonState){
        qDebug()<<"按下为静音";
        ui->voiceButton->setStyleSheet("QPushButton{border-image:url(:/icon/no_sound.png)};");
        ui->voiceSlider->setValue(0);
        ui->voiceSlider->setEnabled(false);
        voiceButtonState=true;
    }
    else{
        qDebug()<<"不是静音";
        ui->voiceButton->setStyleSheet("QPushButton{border-image:url(:/icon/sound.png)};");
        ui->voiceSlider->setValue(30);
        ui->voiceSlider->setEnabled(true);
        voiceButtonState=false;
    }

}

void SetFirst::on_movieTimeSetting_valueChanged(int value)
{
    qDebug() << "Value : "  << value;
    qDebug() << "Text : "  << ui->movieTimeSetting->text();
    recordTime=value;
}
void SetFirst::setWindowStyleSheet()
{

    QPalette p=QPalette();
    p.setColor(QPalette::Background,QColor(255,255,255));
    this->setPalette(p);

//    ui->lightButton->setStyleSheet("QPushButton{border-image:url(:/icon/brightness.png)};");
//    ui->voiceButton->setStyleSheet("QPushButton{border-image:url(:/icon/sound.png)};");
//    ui->comboBox->setStyleSheet("QComboBox{border:1px solid rgb(100, 100, 100); border-radius: 4px;}"
//      "QComboBox QAbstractItemView::item{height:40px;}"
//      "QComboBox::down-arrow{image:url(:/icon/arrowdown.png);}"
//      "QComboBox::drop-down{border:0px;}");

    ui->voiceSlider->setStyleSheet("QSlider::groove:vertical{background: #cbcbcb;width: 6px;border-radius: 1px;padding-left:-1px;padding-right:-1px;padding-top:-1px;padding-bottom:-1px; }"
                                   "QSlider::sub-page:vertical{background: #cbcbcb;border-radius: 2px;}"
                                   "QSlider::add-page:vertical{background: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 #439cf4, stop:1 #439cf4);\
                                   background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,stop: 0 #439cf4, stop: 1 #439cf4);\
                                   width: 10px;border-radius: 2px;}"
                                   "QSlider::handle:vertical{border-image: url(:/icon/circle-white.png);margin: -2px -7px -2px -7px; height: 17px;}"
                                    "QSlider{border-color: #cbcbcb;}"  );
    ui->lightSlider->setStyleSheet("QSlider::groove:vertical{background: #cbcbcb;width: 6px;border-radius: 1px;padding-left:-1px;padding-right:-1px;padding-top:-1px;padding-bottom:-1px; }"
                                   "QSlider::sub-page:vertical{background: #cbcbcb;border-radius: 2px;}"
                                   "QSlider::add-page:vertical{background: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 #439cf4, stop:1 #439cf4);\
                                   background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,stop: 0 #439cf4, stop: 1 #439cf4);\
                                   width: 10px;border-radius: 2px;}"
                                   "QSlider::handle:vertical{border-image: url(:/icon/circle-white.png);margin: -2px -7px -2px -7px; height: 17px;}"
                                    "QSlider{border-color: #cbcbcb;}"  );

    /*spinbox 按下和抬起样式*/
    ui->movieTimeSetting->setStyleSheet("QSpinBox::up-button {subcontrol-origin:border;subcontrol-position:right;image: url(:/icon/right_arrow.png);width: 32px;height: 32px;}"
                                        "QSpinBox::down-button {subcontrol-origin:border;subcontrol-position:left;border-image: url(:/icon/left_arrow.png);width: 32px;height: 32px;}"
                                        "QSpinBox::down-button:enabled:hover {background: rgb(255, 255, 255, 30);}"
                                        "QSpinBox::up-button:enabled:hover {background: rgb(255, 255, 255, 30);}"
                                        "QSpinBox::up-button:pressed{subcontrol-origin:border;subcontrol-position:right;image: url(:/icon/right_arrow.png); width: 32px;height: 32px;}"
                                        "QSpinBox::down-button:pressed{subcontrol-position:left;image: url(:/icon/left_arrow.png);width: 32px;height: 32px;}"
                                        "QSpinBox{border: 1px solid rgb(100, 100, 100);text-align: center; border-radius: 4px;}");

}
