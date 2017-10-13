#include "moviedesk.h"
#include "ui_moviedesk.h"
#include <QMessageBox>
movieDesk* pStaticMovieDesk=NULL;
//用来判断预览的是哪个摄像头保存的数据，true为master false为slave
bool pic_slave_or_master;
bool video_slave_or_master;
extern QString which_filename_to_play;
extern int which_video_show_play;
extern QFileInfo fileInfo_to_play;
movieDesk::movieDesk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::movieDesk),cur_index(0)
{
    ui->setupUi(this);
    pStaticMovieDesk=this;
//    int w,h;
//    w=(1024-this->width())/2;
//    h=(600-this->height())/2;
//    this->mapToParent(QPoint(w,h));
    FormInCenter();
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
    slave_picturewidget=new slave_PictureWidget(this);
    slave_videowidget=new slave_VideoWidget(this);
    videowidget=new videoWidget(this);
//    editwidget=new editWidget();
    fileDialog=new QFileDialog();
    picturewidget=new pictureWidget(this);
#if defined(Q_OS_LINUX)
    fileDialog->setDirectory("/");
#else
    fileDialog->setDirectory("E:");
#endif
    connect(fileDialog,SIGNAL(fileSelected(QString)),this,SLOT(open_file(QString)));

    ui->tabWidget->addTab(videowidget,tr("视频"));
    ui->tabWidget->insertTab(1,picturewidget,tr("图片"));
    ui->tabWidget->insertTab(2,fileDialog,tr("浏览..."));
    //此处加图标有问题
    ui->tabWidget->tabBar()->setTabIcon(2,QIcon(QPixmap("./image/camera.png")));
    ui->tabWidget->tabBar()->setTabIcon(1,QIcon(QPixmap("./image/camera.png")));
    ui->tabWidget->tabBar()->setTabIcon(0,QIcon(QPixmap("./image/cameras.png")));
    connect(picturewidget,SIGNAL(hide_moviedesktop()),this,SLOT(on_hide_moviedesktop()));
    connect(picturewidget,SIGNAL(on_unhide_moviedesktop()),this,SLOT(on_unhide_moviedesktop()));

    connect(slave_picturewidget,SIGNAL(hide_moviedesktop()),this,SLOT(on_hide_moviedesktop()));
    connect(slave_picturewidget,SIGNAL(on_unhide_moviedesktop()),this,SLOT(on_unhide_moviedesktop()));

    connect(videowidget,SIGNAL(hide_moviedesktop()),this,SLOT(on_hide_moviedesktop()));
    connect(videowidget,SIGNAL(on_unhide_moviedesktop()),this,SLOT(on_unhide_moviedesktop()));

    connect(slave_videowidget,SIGNAL(hide_moviedesktop()),this,SLOT(on_hide_moviedesktop()));
    connect(slave_videowidget,SIGNAL(on_unhide_moviedesktop()),this,SLOT(on_unhide_moviedesktop()));
}

movieDesk::~movieDesk()
{
    delete ui;
}
void movieDesk::open_file(QString fileName)
{
    QMessageBox::information(NULL, tr("Path"), tr("You selected ") + fileName);
    QFileInfo fileInfo(fileName);
    QString file_suffix=fileInfo.completeSuffix();
    QString no_suffix_fileName=fileInfo.completeBaseName();
    QString file_path=fileInfo.absolutePath();

    if(file_suffix=="mp4"||file_suffix=="avi"||file_suffix=="mkv"||file_suffix==".mp3")
    {
        qDebug()<<"herrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr";

        fileInfo_to_play=fileInfo;
        qDebug()<<"the path is"<<fileInfo.absolutePath();
        player=new Video_Player();
        on_hide_moviedesktop();
        emit main_desktop_disvisible();
//        this->setHidden(true);
        player->show();
    }else if(file_suffix=="jpg"||file_suffix=="jpeg"||file_suffix=="png"||file_suffix=="bmp")
    {
        pic_view=new Picture_view();
        pic_view->show();
    }else{
        QMessageBox::information(NULL,tr("Information"), tr("Haven't support the file type ,thanks!"));
    }

}
//窗体居中显示
void movieDesk::FormInCenter()
{
    int frmX = this->width();
    int frmY = this->height();
    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    this->move(movePoint);
}
void movieDesk::on_returnButton_clicked()
{
    this->hide();
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
    ui->tabWidget->insertTab(1,picturewidget,tr("图片"));
    ui->tabWidget->insertTab(2,fileDialog,tr("浏览..."));

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
    ui->tabWidget->insertTab(1,slave_picturewidget,tr("图片"));
    ui->tabWidget->insertTab(2,fileDialog,tr("浏览..."));
    ui->tabWidget->setCurrentIndex(cur_index);

}
void movieDesk::on_hide_moviedesktop()
{
    this->setHidden(true);
}
void movieDesk::on_unhide_moviedesktop()
{
    this->setHidden(false);
}
