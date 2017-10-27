#include "moviedesk.h"
#include "ui_moviedesk.h"
#include <QMessageBox>
#include "midwindow.h"
#include "frmmessagebox.h"
//extern movieDesk* pStaticMovieDesk=NULL;
#include <QButtonGroup>
extern QFileInfo fileInfo_to_play;
extern QFileInfo fileInfo_to_show;
extern MidWindow* midwindow;
movieDesk::movieDesk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::movieDesk),cur_index(0)
{
    ui->setupUi(this);
//    pStaticMovieDesk=this;
    isMainCamera=true;
    connect(ui->returnButton,SIGNAL(clicked(bool)),SLOT(on_returnButton_clicked()));

    slave_picturewidget=new slave_PictureWidget(ui->widget_1);
    slave_videowidget=new slave_VideoWidget(ui->widget_1);
    videowidget=new videoWidget(ui->widget_1);
    fileDialog=new QFileDialog(ui->widget_1,Qt::SubWindow);
    picturewidget=new pictureWidget(ui->widget_1);
    QButtonGroup* btnGroup1=new QButtonGroup();
    btnGroup1->addButton(ui->mainCameraButton);
    btnGroup1->addButton(ui->viceButton);
    QButtonGroup* btnGroup2=new QButtonGroup();
    btnGroup2->addButton(ui->videoButton);
    btnGroup2->addButton(ui->picButton);
    btnGroup1->addButton(ui->scanButton);
#if defined(Q_OS_LINUX)
    fileDialog->setDirectory("/");
#else
    fileDialog->setDirectory("E:");
#endif
    ui->mainCameraButton->setCheckable(true);
    ui->viceButton->setCheckable(true);
    ui->videoButton->setCheckable(true);
    ui->picButton->setCheckable(true);
    ui->scanButton->setCheckable(true);

    ui->scanButton->setChecked(true);
    ui->mainCameraButton->setChecked(true);

    connect(fileDialog,SIGNAL(fileSelected(QString)),this,SLOT(open_file(QString)));

    connect(picturewidget,SIGNAL(hide_moviedesktop()),this,SLOT(on_hide_moviedesktop()));
    connect(picturewidget,SIGNAL(on_unhide_moviedesktop()),this,SLOT(on_unhide_moviedesktop()));

    connect(slave_picturewidget,SIGNAL(hide_moviedesktop()),this,SLOT(on_hide_moviedesktop()));
    connect(slave_picturewidget,SIGNAL(on_unhide_moviedesktop()),this,SLOT(on_unhide_moviedesktop()));

    connect(videowidget,SIGNAL(hide_moviedesktop()),this,SLOT(on_hide_moviedesktop()));
    connect(videowidget,SIGNAL(on_unhide_moviedesktop()),this,SLOT(on_unhide_moviedesktop()));

    connect(slave_videowidget,SIGNAL(hide_moviedesktop()),this,SLOT(on_hide_moviedesktop()));
    connect(slave_videowidget,SIGNAL(on_unhide_moviedesktop()),this,SLOT(on_unhide_moviedesktop()));

    connect(fileDialog,SIGNAL(finished(int)),this,SLOT(deal_fileDialog(int)));
    connect(fileDialog,SIGNAL(accepted()),this,SLOT(deal_fileDialog1()));

    connect(midwindow,SIGNAL(usb_insert()),this,SLOT(on_usb_mount()));
    connect(midwindow,SIGNAL(usb_delete()),this,SLOT(on_usb_umount()));
}

movieDesk::~movieDesk()
{
    delete ui;
}
void movieDesk::on_usb_mount()
{
    qDebug()<<"filedialog get the signal usb insert";
    frmMessageBox *msg=new frmMessageBox;
    msg->SetMessage(QString(tr("usb is insert!")),0);
    QTimer::singleShot(2000, msg, SLOT(close()));
//    msg->exec();
    fileDialog->update();
}
void movieDesk::on_usb_umount()
{
    qDebug()<<"filedialog get the signal usb delete";

    frmMessageBox *msg=new frmMessageBox;
    msg->SetMessage(QString(tr("usb is out!")),0);
    QTimer::singleShot(2000, msg, SLOT(close()));
//    msg->exec();
    fileDialog->update();
}

void movieDesk::deal_fileDialog(int a)
{
    qDebug()<<"cancel is done"<<a;
    fileDialog->show();
}
void movieDesk::deal_fileDialog1()
{
    qDebug()<<"accept is done";
    fileDialog->show();
}
void movieDesk::open_file(QString fileName)
{
//    QMessageBox::information(NULL, tr("Path"), tr("You selected ") + fileName);
    QFileInfo fileInfo(fileName);
     QString file_suffix=fileName.right(fileName.size()-fileName.lastIndexOf(".")-1);
    QString no_suffix_fileName=fileInfo.completeBaseName();
    QString file_path=fileInfo.absolutePath();

    if(file_suffix=="mp4"||file_suffix=="avi"||file_suffix=="mkv"||file_suffix=="mp3"||file_suffix=="FLV"||file_suffix=="flv"||file_suffix=="WMV"||
            file_suffix=="MKV"||file_suffix=="MP4"||file_suffix=="AVI")
    {
        fileInfo_to_play=fileInfo;
        qDebug()<<"the path is"<<fileInfo.absolutePath();
        player=new Video_Player();
        on_hide_moviedesktop();
        emit main_desktop_disvisible();
//        this->setHidden(true);
        player->show();
    }else if(file_suffix=="jpg"||file_suffix=="jpeg"||file_suffix=="png"||file_suffix=="bmp"||file_suffix=="BMP")
    {
        fileInfo_to_show=fileInfo;
        pic_view=new Picture_view();
        on_hide_moviedesktop();
        emit main_desktop_disvisible();
        pic_view->show();

    }else{
        QMessageBox::information(NULL,tr("Information"), tr("Haven't support this file type ,thanks!"));
    }
}

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
    qDebug()<<"main camera data";
    ui->mainCameraButton->setChecked(true);
    ui->viceButton->setChecked(false);
    if(ui->videoButton->isChecked())
    {
        on_videoButton_clicked();
    }else if(ui->picButton->isChecked())
    {
        on_picButton_clicked();
    }else
    {
        on_scanButton_clicked();
    }
}
void movieDesk::on_viceButton_clicked()
{
    qDebug()<<"vice camera data";
    ui->mainCameraButton->setChecked(true);
    ui->viceButton->setChecked(false);
    if(ui->videoButton->isChecked())
    {
        on_videoButton_clicked();
    }else if(ui->picButton->isChecked())
    {
        on_picButton_clicked();
    }else
    {
        on_scanButton_clicked();
    }
}
void movieDesk::on_hide_moviedesktop()
{
    this->setHidden(true);
}
void movieDesk::on_unhide_moviedesktop()
{
    this->setHidden(false);
}

void movieDesk::on_videoButton_clicked()
{
    slave_picturewidget->setHidden(true);
    slave_videowidget->setHidden(true);
    videowidget->setHidden(true);
    fileDialog->setHidden(true);
    picturewidget->setHidden(true);
    ui->scanButton->setChecked(false);
    ui->videoButton->setChecked(true);
    ui->picButton->setChecked(false);

    if(ui->mainCameraButton->isChecked())
    {
        videowidget->show();
    }else if(ui->viceButton->isChecked()){
        slave_videowidget->show();
    }
}

void movieDesk::on_picButton_clicked()
{
    slave_picturewidget->setHidden(true);
    slave_videowidget->setHidden(true);
    videowidget->setHidden(true);
    fileDialog->setHidden(true);
    picturewidget->setHidden(true);
    ui->scanButton->setChecked(false);
    ui->videoButton->setChecked(false);
    ui->picButton->setChecked(true);

    if(ui->mainCameraButton->isChecked())
    {
        picturewidget->show();
    }else if(ui->viceButton->isChecked()){
        slave_picturewidget->show();
    }
}

void movieDesk::on_scanButton_clicked()
{
    slave_picturewidget->setHidden(true);
    slave_videowidget->setHidden(true);
    videowidget->setHidden(true);
    fileDialog->setHidden(true);
    picturewidget->setHidden(true);
    ui->scanButton->setChecked(true);
    ui->videoButton->setChecked(false);
    ui->picButton->setChecked(false);

    fileDialog->show();
}
