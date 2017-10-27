#include "picture_view.h"
#include "ui_picture_view.h"
#include "picturewidget.h"
#include "ui_picturewidget.h"
#include <QGridLayout>
#include <QFontDatabase>
#include <QDesktopWidget>
#include "main_desktop.h"
#include "moviedesk1.h"
extern QFileInfo fileInfo_to_show;
extern movieDesk1* pStaticMovieDesk;
extern main_desktop* pStaticMaindesktop;
#if defined(Q_OS_LINUX)
extern int is_dir_exist(const char *dir_path);

#endif

Picture_view::Picture_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Picture_view),pic_view(0)
{
    ui->setupUi(this);
    show_title();
    current_pic=0;
    FormInCenter();
    pic_view=new CProjectionPicture();
    ui->scrollArea->setWidget(pic_view);

    qDebug()<<"the filename will play is"<<fileInfo_to_show.completeBaseName();
    current_path=fileInfo_to_show.absolutePath();
    qDebug()<<"current path is:"<<current_path;
    #if defined(Q_OS_LINUX)
        if(!is_dir_exist((char*)current_path.toStdString().c_str()))
        {
            dir.setPath(current_path);
            file_list=GetFileList(dir);
            for(int i=0;i<file_list.size();i++)
            {
                if(file_list[i]==fileInfo_to_show){
                    current_pic=i;
                    break;
                }
            }
        }else{
            qDebug()<<"path is not exist";
        }
    #else
        if(current_path==""){
            qDebug()<<"the wrong path";
        }else{
            dir.setPath(current_path);
            file_list=GetFileList(dir);
            for(int i=0;i<file_list.size();i++)
            {
                if(file_list[i]==fileInfo_to_show){
                    current_pic=i;
                    break;
                }
            }
        }
    #endif
    show_image(fileInfo_to_show);
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

QFileInfoList Picture_view::GetFileList(QDir dir)
{
    qDebug()<<"get all video file name";
    QStringList filters;
    filters << "*.bmp"<<"*.jpg"<<"*.jpeg"<<"*.png"<<"*.BMP";
    dir.setNameFilters(filters);
    QFileInfoList file_list=dir.entryInfoList();
    for(int i=0;i<file_list.size();i++)
    {
        QFileInfo fileInfo=file_list.at(i);
        fileInfo.absoluteFilePath();
//        qDebug()<<fileInfo.fileName();
    }
    return file_list;
}

void Picture_view::show_image(QFileInfo fileInfo_to_show)
{
    printf("-------------------------------open pic ing\n");
    QImage pic;
    pic.load(fileInfo_to_show.absoluteFilePath());

    pic_view->setPicture(pic);
    ui->label->setText(fileInfo_to_show.completeBaseName());
}

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
    pStaticMaindesktop->setHidden(false);
    pStaticMovieDesk->setHidden(false);

}

void Picture_view::on_btnMenu_Max_clicked()
{
    this->close();
    pStaticMaindesktop->setHidden(false);
    pStaticMovieDesk->setHidden(false);
}

void Picture_view::on_btnMenu_Close_clicked()
{
    this->close();
    pStaticMaindesktop->setHidden(false);
    pStaticMovieDesk->setHidden(false);
}

void Picture_view::on_prePicButton_clicked()
{
    if(current_pic-1>=0)
    {
        show_image(file_list.at(--current_pic));
    }else{
        qDebug()<<"no more pic";
        return ;
    }

}

void Picture_view::on_nextPicButton_clicked()
{
    if(current_pic+1<=file_list.size())
    {
        show_image(file_list.at(++current_pic));
    }else{
        qDebug()<<"no more pic";
        return ;
    }
}

void Picture_view::on_largerButton_clicked()
{
    pic_view->zoomIn();
}

void Picture_view::on_smallerButton_clicked()
{
    pic_view->zoomOut();
}
