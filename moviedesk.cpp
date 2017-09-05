#include "moviedesk.h"
#include "ui_moviedesk.h"

//用来判断预览的是哪个摄像头保存的数据，true为master false为slave
bool pic_slave_or_master;
bool video_slave_or_master;

movieDesk::movieDesk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::movieDesk),cur_index(0)
{
    ui->setupUi(this);

    connect(ui->returnButton,SIGNAL(clicked(bool)),SLOT(on_returnButton_clicked()));

    //将Tab设置为横向
    ui->tabWidget->setTabPosition(QTabWidget::West);
//    ui->tabWidget->setTabShape( QTabWidget::Rounded );
    ui->tabWidget->tabBar()->setStyle(new CustomTabStyle(90,60));

    ui->tabWidget->clear();
    #if 0
    ui->tabWidget->setStyleSheet("QTabWidget::pane{ \
            border-left: 1px solid #eeeeee;\
        }");
    #endif
    //为tabbar加入widget
    slave_picturewidget=new slave_PictureWidget();
    slave_videowidget=new slave_VideoWidget();
    videowidget=new videoWidget();
    editwidget=new editWidget();
    picturewidget=new pictureWidget();
    ui->tabWidget->addTab(videowidget,tr("视频"));
    ui->tabWidget->insertTab(1,editwidget,tr("编辑"));
    ui->tabWidget->insertTab(2,picturewidget,tr("图片"));
    //此处加图标有问题
    ui->tabWidget->tabBar()->setTabIcon(2,QIcon(QPixmap("./image/camera.png")));
    ui->tabWidget->tabBar()->setTabIcon(1,QIcon(QPixmap("./image/camera.png")));
    ui->tabWidget->tabBar()->setTabIcon(0,QIcon(QPixmap("./image/cameras.png")));
}

movieDesk::~movieDesk()
{
    delete ui;
}
void movieDesk::on_returnButton_clicked()
{
    this->close();
}
void movieDesk::on_mainCameraButton_clicked()
{
    qDebug()<<"调整为主摄像头";
    pic_slave_or_master=true;
    cur_index=ui->tabWidget->currentIndex();
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(2);
    ui->tabWidget->addTab(videowidget,tr("视频"));
    ui->tabWidget->insertTab(1,editwidget,tr("编辑"));
    ui->tabWidget->insertTab(2,picturewidget,tr("图片"));
    ui->tabWidget->setCurrentIndex(cur_index);
}
void movieDesk::on_viceButton_clicked()
{
    qDebug()<<"调整为副摄像头";
    pic_slave_or_master=false;
    cur_index=ui->tabWidget->currentIndex();
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(2);
    ui->tabWidget->addTab(slave_videowidget,tr("视频"));
    ui->tabWidget->insertTab(1,editwidget,tr("编辑"));
    ui->tabWidget->insertTab(2,slave_picturewidget,tr("图片"));
    ui->tabWidget->setCurrentIndex(cur_index);

}
