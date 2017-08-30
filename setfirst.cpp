#include "setfirst.h"
#include "ui_setfirst.h"

SetFirst::SetFirst(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetFirst)
{
    ui->setupUi(this);
    //设置界面的样式
    setWindowStyleSheet();

    //连接前后两级页面
    connect(ui->settingsButton,SIGNAL(clicked(bool)),this,SLOT(on_click_setttingsButton(bool)));
    connect(ui->returnButton,SIGNAL(clicked(bool)),this,SLOT(on_click_returnButton(bool)));


    //设置音量和亮度调节
    //设置滚动条和显示数字联动
    connect(ui->voiceSlider,SIGNAL(valueChanged(int)),this,SLOT(on_slider_valuechanged(int)));
    connect(ui->lightSlider,SIGNAL(valueChanged(int)),this,SLOT(on_slider_valuechanged(int)));
    ui->voiceSlider->setValue(50);
    ui->lightSlider->setValue(50);
    ui->voiceLabel->setText(tr("50"));
    ui->lightLabel->setText(tr("50"));
    ui->voiceButton->setStyleSheet(tr("background-image: url(:/image/image/voice.png);"));
    ui->lightButton->setStyleSheet(tr("background-image: url(:/image/image/light.png);"));
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

}
SetFirst::~SetFirst()
{
    delete ui;
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
        qDebug()<<"调节声音";
        ui->voiceButton->setChecked(false);
        ui->voiceLabel->setText(QString("%1").arg(n_value));
    }
    else if(sender==ui->lightSlider){
        qDebug()<<"调节亮度";
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
    }
    else{
        qDebug()<<"录音按钮处于弹起状态";
    }
}
void SetFirst::on_ADASButton_clicked()
{
    if(ui->ADASButton->isChecked()){
        qDebug()<<"ADAS按钮处于按下状态";
    }
    else{
        qDebug()<<"ADAS按钮处于弹起状态";
    }
}
void SetFirst::on_movieButton_clicked()
{
    if(ui->movieButton->isChecked()){
        qDebug()<<"录像按钮处于按下状态";
    }
    else{
        qDebug()<<"录像按钮处于弹起状态";
    }
}
void SetFirst::on_reverseButton_clicked()
{
    if(ui->reverseButton->isChecked()){
        qDebug()<<"倒车线按钮处于按下状态";
    }
    else{
        qDebug()<<"倒车线按钮处于弹起状态";
    }
}

void SetFirst::on_voiceButton_clicked()
{
    if(ui->voiceButton->isChecked()){
        qDebug()<<"按下为静音";
    }
    else{
        qDebug()<<"不是静音";
    }

}

void SetFirst::on_movieTimeSetting_valueChanged(int value)
{
    qDebug() << "Value : "  << value;
    qDebug() << "Text : "  << ui->movieTimeSetting->text();
}
void SetFirst::setWindowStyleSheet()
{
    QPalette p=QPalette();
    p.setColor(QPalette::Background,QColor(255,255,255));
    this->setPalette(p);
    ui->comboBox->setStyleSheet("QComboBox{border:1px solid gray;}"
      "QComboBox QAbstractItemView::item{height:40px;}"
      "QComboBox::down-arrow{image:url(:/icon/arrowdown.png);}"
      "QComboBox::drop-down{border:0px;}");
      ui->comboBox->setView(new QListView());
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
}
