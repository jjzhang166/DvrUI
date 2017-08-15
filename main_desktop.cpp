#include "main_desktop.h"
#include "ui_main_desktop.h"
#include <QDesktopWidget>
main_desktop::main_desktop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::main_desktop)
{
    ui->setupUi(this);

    //设置窗口为固定大小
//    this->setMaximumSize(610,215);
//    this->setMinimumSize(610,215);
    isLocked=false;
    //设置按钮图标
    //对于当点击图标后按钮的变化的效果可以使用多张图片的方式进行
//    ui->cameraButton->setStyleSheet(tr("background-image: url(:/image/image/camera.png);","background-color:transparent"));
//    ui->camera_change_Button->setStyleSheet(tr("background-image: url(:/image/image/change.png);"));
//    ui->lockButton->setStyleSheet(tr("background-image: url(:/image/image/lock.png);"));
//    ui->movieButton->setStyleSheet(tr("background-image: url(:/image/image/movie1.png);"));
//    ui->recordButton->setStyleSheet(tr("background-image: url(:/image/image/record.png);"));
//    ui->setFirstButton->setStyleSheet(tr("background-image: url(:/image/image/setting.png);"));

    //设置系统时间和显示
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(10);
    QFont ft;
    ft.setPointSize(12);
    ft.setBold(true);
    ft.setFamily("Microft YaHei");
    ui->time_Label->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::white);
    ui->time_Label->setPalette(pa);

    //摄像头数据显示cameraView
    connect(ui->camera_change_Button,SIGNAL(clicked()),this,SLOT(cameraChange()));
//    ui->cameraView->setStyleSheet(tr("background-image: url(:/image/image/picture.png);"));//初始化为前置
    cameraState=false;//点击切换后切换到后置

    //一段时间没有操作后自动隐藏图标
    ui->cameraButton->setVisible(true);
    ui->camera_change_Button->setVisible(true);
    ui->lockButton->setVisible(true);
    ui->movieButton->setVisible(true);
    ui->recordButton->setVisible(true);
    ui->setFirstButton->setVisible(true);
    ui->compassButton->setVisible(true);
    setMouseTracking(true);
    mouseMoveTime = new QTimer();
    connect(mouseMoveTime,SIGNAL(timeout()),this,SLOT(on_mouse_no_active_10_second()));
    mouseMoveTime->start(8000);

    //调出其它界面的信号
    connect(ui->setFirstButton,SIGNAL(clicked(bool)),this,SLOT(show_settingDesk()));
    connect(ui->cameraButton,SIGNAL(clicked(bool)),this,SLOT(show_photoDesk()));
    connect(ui->movieButton,SIGNAL(clicked(bool)),this,SLOT(show_movieDesk()));
    connect(ui->compassButton,SIGNAL(clicked(bool)),this,SLOT(show_dashboard()));


}

main_desktop::~main_desktop()
{
    delete ui;
}
void main_desktop::timerUpdate(void)
{
    QDateTime current_time = QDateTime::currentDateTime();//获取系统现在的时间
    QString time= current_time.toString("hh:mm:ss ddd"); //设置显示格式
    ui->time_Label->setText(time);//在标签上显示时间
}

//前后摄像头切换
void main_desktop::cameraChange(void)
{
    //如果现在是前置->切换为后置
    //如果为后置->切换为前置
    if(cameraState==true)
    {
        qDebug()<<"按下1";
        ui->cameraView->setStyleSheet(tr("background-image: url(:/picture.png);"));
        cameraState=false;
    }
    else
    {
        qDebug()<<"按下2";
        ui->cameraView->setStyleSheet(tr("background-image: url(:/picture1.png);"));
        cameraState=true;
    }
}
//无操作是隐藏图标
void main_desktop::on_mouse_no_active_10_second()
{
    ui->cameraButton->setVisible(false);
    ui->camera_change_Button->setVisible(false);
    ui->lockButton->setVisible(false);
    ui->movieButton->setVisible(false);
    ui->recordButton->setVisible(false);
    ui->setFirstButton->setVisible(false);
    ui->compassButton->setVisible(false);
}
//有操作出现
void main_desktop::on_mouse_active()
{
    mouseMoveTime->stop();
    mouseMoveTime->start();

    ui->cameraButton->setVisible(true);
    ui->camera_change_Button->setVisible(true);
    ui->lockButton->setVisible(true);
    ui->movieButton->setVisible(true);
    ui->recordButton->setVisible(true);
    ui->setFirstButton->setVisible(true);
    ui->compassButton->setVisible(true);
}
//重写QWidget的4个方法来保证有动作时显示，没动作时隐藏
void main_desktop::mouseDoubleClickEvent(QMouseEvent *)
{
    on_mouse_active();
}
void main_desktop::mouseMoveEvent(QMouseEvent *)
{
    on_mouse_active();
}
void main_desktop::mousePressEvent(QMouseEvent *)
{
    on_mouse_active();
}
void main_desktop::mouseReleaseEvent(QMouseEvent *)
{
    on_mouse_active();
}

//打开新的界面
void main_desktop::show_settingDesk()
{
    setting_desktop=new SetFirst();
    connect(setting_desktop,SIGNAL(send_data_to_main(results)),this,SLOT(recieve_setting_data(results)));
    setting_desktop->exec();

}
//暂时做成截图功能
void main_desktop::show_photoDesk()
{
    qDebug()<<"修改为截图功能";
}
void main_desktop::show_movieDesk()
{
    qDebug()<<"open movie";
    moviedesk=new movieDesk();
    moviedesk->exec();
}
void main_desktop::show_dashboard()
{
    dashboards=new dashBoard();
    QPalette pal(dashboards->palette());
    pal.setColor(QPalette::Background, Qt::black); //设置背景黑色
    dashboards->setAutoFillBackground(true);
    dashboards->setPalette(pal);
    dashboards->exec();
}
//void main_desktop::recieve_setting_data(results setting_result)
//{
//    qDebug()<<"I will accept data";
//    qDebug()<<setting_result.testString;
//}
//截图的方法
void main_desktop::on_cameraButton_clicked()
{
//    screenshot_pic=QScreen::grabWindow(this,0,0,600,200);
    ui->cameraButton->setVisible(true);
    ui->camera_change_Button->setVisible(true);
    ui->lockButton->setVisible(true);
    ui->movieButton->setVisible(true);
    ui->recordButton->setVisible(true);
    ui->setFirstButton->setVisible(true);
    ui->compassButton->setVisible(true);
    QScreen * pqscreen  = QGuiApplication::primaryScreen() ;
    QPixmap pixmap = pqscreen->grabWindow( QApplication::activeWindow()->winId(), -2,-2,QApplication::activeWindow()->width() + 1, QApplication::activeWindow()->height() + 1);

    //截图保存
    QString filename=QFileDialog::getSaveFileName(this,"ScreenShot Save",QDir::currentPath());
    if(!filename.isEmpty()){
        pixmap.save(filename);
    }else{
        QMessageBox::information(this,"错误","请选择一个名称 ");
        return;
    }
    ui->cameraButton->setVisible(false);
    ui->camera_change_Button->setVisible(false);
    ui->lockButton->setVisible(false);
    ui->movieButton->setVisible(false);
    ui->recordButton->setVisible(false);
    ui->setFirstButton->setVisible(false);
    ui->compassButton->setVisible(false);
}
//锁定屏幕，使用锁定屏幕所有按钮的方法
void main_desktop::on_lockButton_clicked()
{
    if(isLocked==false){
        ui->cameraButton->setEnabled(false);
        ui->camera_change_Button->setEnabled(false);
        ui->movieButton->setEnabled(false);
        ui->recordButton->setEnabled(false);
        ui->setFirstButton->setEnabled(false);
        ui->compassButton->setEnabled(false);

        isLocked=true;
    }else{
        ui->cameraButton->setEnabled(true);
        ui->camera_change_Button->setEnabled(true);
        ui->movieButton->setEnabled(true);
        ui->recordButton->setEnabled(true);
        ui->setFirstButton->setEnabled(true);
        ui->compassButton->setEnabled(true);
        ui->cameraButton->setVisible(true);
        ui->camera_change_Button->setVisible(true);
        ui->lockButton->setVisible(true);
        ui->movieButton->setVisible(true);
        ui->recordButton->setVisible(true);
        ui->setFirstButton->setVisible(true);
        ui->compassButton->setVisible(true);
        isLocked=false;
    }
}
