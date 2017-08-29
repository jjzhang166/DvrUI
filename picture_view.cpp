#include "picture_view.h"
#include "ui_picture_view.h"
#include "picturewidget.h"
#include "ui_picturewidget.h"


// This is available in all editors.
extern int which_pic_show_big;
extern QString which_filename_show_big;

Picture_view::Picture_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Picture_view),number_to_show(which_pic_show_big)
{
    ui->setupUi(this);
    pictureLabel = new QLabel(this);
    scrollArea=new QScrollArea(this);
    scrollArea->setWidget(pictureLabel);
    scrollArea->setBaseSize(QSize(260,260));

    qDebug()<<"需要打开第"<<number_to_show<<"张图片";
    qDebug()<<which_pic_show_big;
    qDebug()<<"打开的文件名称为"<<which_filename_show_big;
    QDirIterator m_DirIterator(QString("../DvrUI/image"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
//    qDebug()<<" 当前目录为："<<m_DirIterator.path();
//    int count=0;
    while (m_DirIterator.hasNext()) {
        QString tempFile=m_DirIterator.next();
        QString tempFileName=tempFile;
        tempFileName=tempFileName.remove(QString("../DvrUI/image/"),Qt::CaseSensitive);

        if(which_filename_show_big==tempFileName){
            qDebug()<<"打开图片ing";
            QPixmap objPixmap(tempFile);
            pictureLabel->setPixmap(objPixmap);
            pictureLabel->resize(QSize(objPixmap.width(),objPixmap.height()));
            break;
        }
        else{
            continue;
        }
    }
//    QHBoxLayout *layout1=new QHBoxLayout();
//    layout1->addSpacing(2);
//    layout1->addWidget(ui->closeButton);
//    QHBoxLayout *layout2=new QHBoxLayout();
//    layout2->addWidget(scrollArea);
//    QHBoxLayout *layouts=new QHBoxLayout;
//    layouts->addLayout(layout1);
//    layouts->addLayout(layout2);

    //去掉scroll的边框
    scrollArea->setWidget(pictureLabel);
    scrollArea->setFrameShape(QFrame::NoFrame);
    pictureLabel->setAlignment(Qt::AlignCenter);  // 图片居中
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidgetResizable(true);  // 自动调整大小
    scrollArea->setFixedSize(180,180);
    pictureLabel->setAlignment(Qt::AlignCenter);
    scrollArea->setAlignment(Qt::AlignCenter);  // 居中对齐
//    scrollArea->setWidget(ui->closeButton);


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
