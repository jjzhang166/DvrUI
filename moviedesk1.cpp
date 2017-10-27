#include "moviedesk1.h"
#include "ui_moviedesk1.h"
#include <QMessageBox>
#include "midwindow.h"
#include "frmmessagebox.h"
movieDesk1* pStaticMovieDesk=NULL;
#include <QButtonGroup>
extern QFileInfo fileInfo_to_play;
extern QFileInfo fileInfo_to_show;
extern MidWindow* midwindow;
int video_support_or_not(QFileInfo file_to_play)
{
#if defined(Q_OS_LINUX)
    AUTPlayer *ap;
    if(ap==NULL)
    {
        QString ab_file_path=file_to_play.absoluteFilePath();
        ap = new AUTPlayer();
        int ret = ap->setUrl((char*)ab_file_path.toStdString().c_str());
        if (ret < 0)
        {
            qDebug()<<"setUrl is error-------------------";
            frmMessageBox *msg=new frmMessageBox;
            msg->SetMessage(QString("video not support!"),0);
            msg->exec();
            if(ap!=NULL)
                delete ap;
            ap=NULL;
            return -1;
        }else{
            return 0;
        }
    }
#endif
}
movieDesk1::movieDesk1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::movieDesk1),cur_index(0)
{
    ui->setupUi(this);
    pStaticMovieDesk=this;


    ui->mainCameraButton->setCheckable(true);
    ui->viceButton->setCheckable(true);
    ui->videoButton->setCheckable(true);
    ui->picButton->setCheckable(true);
    ui->scanButton->setCheckable(true);

    ui->videoButton->setChecked(true);
    ui->mainCameraButton->setChecked(true);
    ui->mainCameraButton->setStyleSheet("background-color:red");
    ui->scanButton->setStyleSheet("background-color:red");
    slave_picturewidget=new slave_PictureWidget();
    slave_videowidget=new slave_VideoWidget();
    videowidget=new videoWidget();
    fileDialog=new QFileDialog(ui->stackedWidget,Qt::SubWindow);
    picturewidget=new pictureWidget();

    QButtonGroup* btnGroup1=new QButtonGroup();
    btnGroup1->addButton(ui->mainCameraButton);
    btnGroup1->addButton(ui->viceButton);
    btnGroup1->setExclusive(true);
    QButtonGroup* btnGroup2=new QButtonGroup();
    btnGroup2->addButton(ui->videoButton);
    btnGroup2->addButton(ui->picButton);
    btnGroup2->addButton(ui->scanButton);
    btnGroup2->setExclusive(true);

    ui->stackedWidget->addWidget(slave_picturewidget);
    ui->stackedWidget->addWidget(slave_videowidget);
    ui->stackedWidget->addWidget(videowidget);
    ui->stackedWidget->addWidget(fileDialog);
    ui->stackedWidget->addWidget(picturewidget);
    ui->stackedWidget->setCurrentWidget(fileDialog);

#if defined(Q_OS_LINUX)
    fileDialog->setDirectory("/");
#else
    fileDialog->setDirectory("E:");
#endif

    connect(fileDialog,SIGNAL(fileSelected(QString)),this,SLOT(open_file(QString)));
    connect(ui->returnButton,SIGNAL(clicked(bool)),SLOT(on_returnButton_clicked()));

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

movieDesk1::~movieDesk1()
{
    delete ui;
}
void movieDesk1::on_usb_mount()
{
    qDebug()<<"filedialog get the signal usb insert";
    frmMessageBox *msg=new frmMessageBox;
    msg->SetMessage(QString(tr("usb is insert!")),0);
    QTimer::singleShot(2000, msg, SLOT(close()));
//    msg->exec();
    fileDialog->update();
}
void movieDesk1::on_usb_umount()
{
    qDebug()<<"filedialog get the signal usb delete";

    frmMessageBox *msg=new frmMessageBox;
    msg->SetMessage(QString(tr("usb is out!")),0);
    QTimer::singleShot(2000, msg, SLOT(close()));
//    msg->exec();
    fileDialog->update();
}

void movieDesk1::deal_fileDialog(int a)
{
    qDebug()<<"cancel is done"<<a;
    fileDialog->show();
}
void movieDesk1::deal_fileDialog1()
{
    qDebug()<<"accept is done";
    fileDialog->show();
}
void movieDesk1::open_file(QString fileName)
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
//        qDebug()<<"the path is"<<fileInfo.absolutePath();
        int ret=0;
        ret=video_support_or_not(fileInfo_to_play);
        if(!ret){
            player=new Video_Player();
            on_hide_moviedesktop();
            emit main_desktop_disvisible();
    //        this->setHidden(true);
            player->show();
        }

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

void movieDesk1::FormInCenter()
{
    int frmX = this->width();
    int frmY = this->height();
    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    this->move(movePoint);
}
void movieDesk1::on_returnButton_clicked()
{
    this->hide();
}
void movieDesk1::on_mainCameraButton_clicked()
{
    qDebug()<<"main camera data";
    ui->mainCameraButton->setChecked(true);
    ui->mainCameraButton->setStyleSheet("background-color:red");
    ui->viceButton->setStyleSheet("border-style: none;"
                                      "border: 0px;"
                                      "color: #F0F0F0;"
                                      "padding: 5px;	"
                                      "border-radius:5px;"
                                     " background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);");
    if(ui->videoButton->isChecked())
    {
        on_videoButton_clicked();
    }
    if(ui->picButton->isChecked())
    {
        on_picButton_clicked();
    }
    if(ui->scanButton->isChecked())
    {
        on_scanButton_clicked();
    }
}
void movieDesk1::on_viceButton_clicked()
{
    qDebug()<<"vice camera data";
    ui->viceButton->setChecked(true);
    ui->viceButton->setStyleSheet("background-color:red");
    ui->mainCameraButton->setStyleSheet("border-style: none;"
                                      "border: 0px;"
                                      "color: #F0F0F0;"
                                      "padding: 5px;	"
                                      "border-radius:5px;"
                                     " background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);");
    if(ui->videoButton->isChecked())
    {
        on_videoButton_clicked();
    }
    if(ui->picButton->isChecked())
    {
        on_picButton_clicked();
    }
    if(ui->scanButton->isChecked())
    {
        on_scanButton_clicked();
    }
}
void movieDesk1::on_hide_moviedesktop()
{
    this->setHidden(true);
}
void movieDesk1::on_unhide_moviedesktop()
{
    this->setHidden(false);
}

void movieDesk1::on_videoButton_clicked()
{
    ui->videoButton->setChecked(true);
    ui->videoButton->setStyleSheet("background-color:red");
    ui->picButton->setStyleSheet("border-style: none;"
                                      "border: 0px;"
                                      "color: #F0F0F0;"
                                      "padding: 5px;	"
                                      "border-radius:5px;"
                                     " background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);");
    ui->scanButton->setStyleSheet("border-style: none;"
                                      "border: 0px;"
                                      "color: #F0F0F0;"
                                      "padding: 5px;	"
                                      "border-radius:5px;"
                                     " background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);");

    if(ui->mainCameraButton->isChecked())
    {
        ui->stackedWidget->setCurrentWidget(videowidget);
    } else if(ui->viceButton->isChecked())
    {
        ui->stackedWidget->setCurrentWidget(slave_videowidget);
    }
}

void movieDesk1::on_picButton_clicked()
{
    ui->picButton->setChecked(true);
    ui->picButton->setStyleSheet("background-color:red");
    ui->videoButton->setStyleSheet("border-style: none;"
                                      "border: 0px;"
                                      "color: #F0F0F0;"
                                      "padding: 5px;	"
                                      "border-radius:5px;"
                                     " background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);");
    ui->scanButton->setStyleSheet("border-style: none;"
                                      "border: 0px;"
                                      "color: #F0F0F0;"
                                      "padding: 5px;	"
                                      "border-radius:5px;"
                                     " background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);");
    if(ui->mainCameraButton->isChecked())
    {
        ui->stackedWidget->setCurrentWidget(picturewidget);
    }else if(ui->viceButton->isChecked()){
        ui->stackedWidget->setCurrentWidget(slave_picturewidget);
    }
}

void movieDesk1::on_scanButton_clicked()
{
//    slave_picturewidget->setHidden(true);
//    slave_videowidget->setHidden(true);
//    videowidget->setHidden(true);
//    fileDialog->setHidden(true);
//    picturewidget->setHidden(true);
//    ui->scanButton->setChecked(true);
//    ui->videoButton->setChecked(false);
//    ui->picButton->setChecked(false);
    ui->scanButton->setChecked(true);
    ui->scanButton->setStyleSheet("background-color:red");
    ui->videoButton->setStyleSheet("border-style: none;"
                                      "border: 0px;"
                                      "color: #F0F0F0;"
                                      "padding: 5px;	"
                                      "border-radius:5px;"
                                     " background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);");
    ui->picButton->setStyleSheet("border-style: none;"
                                      "border: 0px;"
                                      "color: #F0F0F0;"
                                      "padding: 5px;	"
                                      "border-radius:5px;"
                                     " background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #0F7DBE, stop:1 #1582C3);");
    ui->stackedWidget->setCurrentWidget(fileDialog);
}
