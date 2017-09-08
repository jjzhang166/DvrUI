#include "picture_view.h"
#include "ui_picture_view.h"
#include "picturewidget.h"
#include "ui_picturewidget.h"
#include <QGridLayout>
#include <QFontDatabase>
#include <QDesktopWidget>
// This is available in all editors.
extern int which_masterpic_show_big;
extern QString which_masterfilename_show_big;
extern int which_slavepic_show_big;
extern QString which_slavefilename_show_big;
extern bool pic_slave_or_master;


Picture_view::Picture_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Picture_view),pic_view(0)
{
    ui->setupUi(this);
    show_title();
//    int w,h;
//    w=(1024-this->width())/2;
//    h=(600-this->height())/2;
//    this->mapToGlobal(QPoint(w,h));
    FormInCenter();
//    pictureLabel = new QLabel(this);
//    scrollArea=new QScrollArea(this);
//    scrollArea->setWidget(pictureLabel);
//    scrollArea->setBaseSize(QSize(260,260));
    pic_view=new CProjectionPicture();

//    QGridLayout* layout;
//    layout->addWidget(pic_view);
//    layout->addLayout(ui->gridLayout,1,0);
//    layout->setRowStretch(1,6);
//    setLayout(layout);
    ui->scrollArea->setWidget(pic_view);

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
void Picture_view::show_title()
{
    int fontId = QFontDatabase::addApplicationFont(":/font/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont iconFont = QFont(fontName);
    iconFont.setPointSize(10);
    ui->btnMenu_Close->setFont(iconFont);
    ui->btnMenu_Close->setText(QChar(0xf00d));
    ui->btnMenu_Max->setFont(iconFont);
    ui->btnMenu_Max->setText( QChar(0xf096));
    ui->btnMenu_Min->setFont(iconFont);
    ui->btnMenu_Min->setText( QChar(0xf068));
    iconFont.setPointSize(12);
    ui->lab_Ico->setFont(iconFont);
    ui->lab_Ico->setText( QChar(0xf015));
    ui->label->setFont(iconFont);
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
            QImage pic;
            pic.load(tempFile);

            pic_view->setPicture(pic);
            ui->label->setText(tempFileName);
//            QPixmap objPixmap(tempFile);
//            ui->pictureLabel->setPixmap(objPixmap);
//            ui->pictureLabel->resize(QSize(objPixmap.width(),objPixmap.height()));
//            ui->pictureLabel->resize(QSize(512,300));
            break;
        }
        else{
            continue;
        }
    }
//    ui->pictureLabel->setAlignment(Qt::AlignCenter);  // 图片居中
}
//窗体居中显示
void Picture_view::FormInCenter()
{
    int frmX = this->width();
    int frmY = this->height();
    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();
    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    this->move(movePoint);
}
void Picture_view::on_btnMenu_Min_clicked()
{
    this->close();
//    pictureWidget* pPictureWidget=static_cast<pictureWidget*>(parentWidget());
//    pPictureWidget->setHidden(false);
    emit p_unhide_moviedesktop();
}

void Picture_view::on_btnMenu_Max_clicked()
{
    this->close();
//    pictureWidget* pPictureWidget=static_cast<pictureWidget*>(parentWidget());
//    pPictureWidget->setHidden(false);
    emit p_unhide_moviedesktop();
}

void Picture_view::on_btnMenu_Close_clicked()
{
    this->close();
//    pictureWidget* pPictureWidget=static_cast<pictureWidget*>(parentWidget());
//    pPictureWidget->setHidden(false);
    emit p_unhide_moviedesktop();
}
