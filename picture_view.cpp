#include "picture_view.h"
#include "ui_picture_view.h"
#include "picturewidget.h"
#include "ui_picturewidget.h"


// This is available in all editors.
extern int which_masterpic_show_big;
extern QString which_masterfilename_show_big;
extern int which_slavepic_show_big;
extern QString which_slavefilename_show_big;
extern bool pic_slave_or_master;


Picture_view::Picture_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Picture_view)
{
    ui->setupUi(this);
//    pictureLabel = new QLabel(this);
//    scrollArea=new QScrollArea(this);
//    scrollArea->setWidget(pictureLabel);
//    scrollArea->setBaseSize(QSize(260,260));



    if(pic_slave_or_master){
        qDebug()<<"显示主摄像头数据";
        which_pic_show_big=which_masterpic_show_big;
        which_filename_show_big=which_masterfilename_show_big;
    }else{
        qDebug()<<"显示副摄像头数据";
        which_pic_show_big=which_slavepic_show_big;
        which_filename_show_big=which_slavefilename_show_big;
    }
    qDebug()<<"需要打开第"<<which_pic_show_big<<"张图片";
    qDebug()<<which_pic_show_big;
    qDebug()<<"打开的文件名称为"<<which_filename_show_big;
    #if defined(Q_OS_LINUX)
        if(pic_slave_or_master){
            m_DirIterator=new QDirIterator(QString("/mnt/sdcard/mmcblk1p1/frontPicture/"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
        }else{
            m_DirIterator=new QDirIterator(QString("/mnt/sdcard/mmcblk1p1/rearPicture/"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
        }

    #else
        m_DirIterator=new QDirIterator(QString("../DvrUI/image"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    #endif
    show_image(m_DirIterator);

    //去掉scroll的边框
//    QGridLayout *layout=new QGridLayout();
//    scrollArea->setWidget(pictureLabel);
//    scrollArea->setFrameShape(QFrame::NoFrame);
//    pictureLabel->setAlignment(Qt::AlignCenter);  // 图片居中
//    scrollArea->setBackgroundRole(QPalette::Dark);
//    scrollArea->setWidgetResizable(true);  // 自动调整大小
//    scrollArea->setFixedSize(180,180);
//    scrollArea->setBaseSize(QSize(this->width()-25,this->height()));
//    pictureLabel->setAlignment(Qt::AlignCenter);
//    scrollArea->setAlignment(Qt::AlignCenter);  // 居中对齐
//    scrollArea->setWidget(ui->closeButton);
//    layout->addWidget(scrollArea,0,0);
//    layout->addLayout(ui->gridLayout,0,1);
//    setLayout(layout);
}

Picture_view::~Picture_view()
{
    delete ui;
}

void Picture_view::on_closeButton_clicked()
{
    this->close();
    pictureWidget* pPictureWidget=static_cast<pictureWidget*>(parentWidget());
    pPictureWidget->setHidden(false);
}
void Picture_view::show_image(QDirIterator* m_DirIterator)
{
    while (m_DirIterator->hasNext()) {
        QString tempFile=m_DirIterator->next();
        QString tempFileName=tempFile;
        qDebug()<<"打开的图片名为："<<tempFileName;
        #if defined(Q_OS_LINUX)
        if(pic_slave_or_master){
            tempFileName=tempFileName.remove(QString("/mnt/sdcard/mmcblk1p1/frontPicture/"),Qt::CaseSensitive);
        }else{
            tempFileName=tempFileName.remove(QString("/mnt/sdcard/mmcblk1p1/rearPicture/"),Qt::CaseSensitive);
        }
        #else
            tempFileName=tempFileName.remove(QString("../DvrUI/image/"),Qt::CaseSensitive);
        #endif

        if(which_filename_show_big==tempFileName){
            qDebug()<<"打开图片ing";
            printf("-------------------------------open pic ing\n");
            QPixmap objPixmap(tempFile);
            ui->pictureLabel->setPixmap(objPixmap);
//            ui->pictureLabel->resize(QSize(objPixmap.width(),objPixmap.height()));
//            ui->pictureLabel->resize(QSize(512,300));
            break;
        }
        else{
            continue;
        }
    }
    ui->pictureLabel->setAlignment(Qt::AlignCenter);  // 图片居中
}
