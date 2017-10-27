#include "videowidget.h"
#include "ui_videowidget.h"
#include <QDesktopWidget>
#include "main_desktop.h"
#include "midwindow.h"
#include "frmmessagebox.h"
extern main_desktop* pStaticMaindesktop;
// This is available in all editors.
extern QFileInfo fileInfo_to_play;
const QString win_path="E:/tech_practise/DvrUI/DvrUI/video/";
//const QString linux_path="/mnt/sdcard/mmcblk1p1/frontVideo/";//sdcard
extern QString linux_usb_path;//U
extern QString linux_sdcard_path;//sd

extern MidWindow* midwindow;
extern int video_support_or_not(QFileInfo file_to_play);
videoWidget::videoWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::videoWidget)
{
    ui->setupUi(this);
//    ui->listWidget_file=new QListWidget(this);
    show_model=true;
    ui->listWidget_file->clear();

    show_file();

    connect(ui->listWidget_file,SIGNAL(clicked(QModelIndex)),this,SLOT(play_video(QModelIndex)));

    connect(this,SIGNAL(main_desktop_disvisible()),pStaticMaindesktop,SLOT(on_main_desktop_disvisible()));

    connect(midwindow,SIGNAL(usb_insert()),this,SLOT(on_usb_mount()));
    connect(midwindow,SIGNAL(usb_delete()),this,SLOT(on_usb_umount()));

}
void videoWidget::show_file()
{
    qDebug()<<"video widget is showing file";
    #if defined(Q_OS_LINUX)
    qDebug()<<"now path is "<<linux_usb_path;
    file_list=GetFileList(QDir(linux_usb_path));
    #else
    file_list=GetFileList(QDir(win_path));
    #endif
    ui->listWidget_file->setObjectName(QString::fromUtf8("listWidget_file"));
    ui->listWidget_file->setMovement(QListView::Static);

    ui->listWidget_file->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->listWidget_file->setSortingEnabled(true);


    //ui->listWidget_file->selectAll();
    ui->listWidget_file->setSpacing(12);
    ui->listWidget_file->setResizeMode(QListView::Adjust);
    if(show_model){

        ui->listWidget_file->setViewMode(QListView::IconMode);
        ui->listWidget_file->setIconSize(QSize(100,100));

        ui->listWidget_file->setDragEnabled(false);
        show_file_by_iconview(file_list);
    }else{

        ui->listWidget_file->setViewMode(QListView::ListMode);
        ui->listWidget_file->setDragEnabled(false);
        ui->listWidget_file->setIconSize(QSize(22,22));
        show_file_by_listview(file_list);
    }
}
void videoWidget::on_usb_mount()
{
    qDebug()<<"videowidget get the signal usb insert";
    frmMessageBox *msg=new frmMessageBox;
    msg->SetMessage(QString(tr("usb is insert!")),0);
    QTimer::singleShot(2000, msg, SLOT(close()));
    msg->exec();
    show_file();
}
void videoWidget::on_usb_umount()
{
    qDebug()<<"videowidget get the signal usb delete";
    ui->listWidget_file->clear();
    frmMessageBox *msg=new frmMessageBox;
    msg->SetMessage(QString(tr("usb is out!")),0);
    QTimer::singleShot(2000, msg, SLOT(close()));
    msg->exec();
}

QFileInfoList videoWidget::GetFileList(QDir dir)
{
    qDebug()<<"GetFileList:"<<"now path is :"<<dir.path();
    qDebug()<<"get all file name";
    QStringList filters;
    filters << "*.mp4" <<"*.MP4"<< "*.avi"<<"*.AVI"<<"*.mkv"<<"*.MKV"<<"*.mp3"<<"*.VOB"<<"*.vob"<<"*.FLV"<<"*.flv"<<"*.WMV";
    dir.setNameFilters(filters);
    if(!dir.exists())
    {
        qDebug()<<"usb dir is:"<<linux_usb_path;
        qDebug()<<"now dir path is:"<<dir.absolutePath();
        qDebug()<<"dir is not exisits";
    }
    QFileInfoList file_list=dir.entryInfoList();
    for(int i=0;i<file_list.size();i++)
    {
        QFileInfo fileInfo=file_list.at(i);
        fileInfo.absoluteFilePath();
//        qDebug()<<fileInfo.fileName();
    }
    return file_list;
}
void videoWidget::show_file_by_iconview(QFileInfoList file_list)
{
    qDebug()<<"show_file_by_iconview";
    qDebug()<<"file_list has "<<file_list.size()<<" files";
    ui->listWidget_file->setIconSize(QSize(100,100));
    for(int i=0;i<file_list.size();i++)
    {
        QFileInfo file_info=file_list.at(i);
        QString tempFilePath=file_info.absoluteFilePath();
        int filename_index=tempFilePath.lastIndexOf("/");
        QString tempFileName=tempFilePath.right(tempFilePath.size()-filename_index-1);
//        qDebug()<<"file name:"<<tempFileName;
        QString tempFileName_NoSuffix=tempFileName;
        int suffix_index=tempFileName_NoSuffix.lastIndexOf(".");
        tempFileName_NoSuffix.truncate(suffix_index);
//        qDebug()<<"the no_suffix filename:"<<tempFileName_NoSuffix;
        QString icon_file_path="";
        gen_shot_picture(tempFileName_NoSuffix,icon_file_path,tempFileName);
        QByteArray ba=tempFileName.toLocal8Bit();
        const char* str=ba.data();
        QListWidgetItem *pItem;
        if(icon_file_path==""){
            QPixmap objPixmap1(":/icon/no_shotvideo.png");
            pItem=new QListWidgetItem(QIcon(objPixmap1.scaled(QSize(100,70))),QString(str));
        }else{
            QPixmap objPixmap(icon_file_path);
            pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(100,70))),QString(str));
        }
        pItem->setSizeHint(QSize(100,90));
        ui->listWidget_file->addItem(pItem);
    }
}
void videoWidget::show_file_by_listview(QFileInfoList file_list)
{
    ui->listWidget_file->setIconSize(QSize(22,22));
    for(int i=0;i<file_list.size();i++)
    {
        QFileInfo file_info=file_list.at(i);
        QString tempFilePath=file_info.absoluteFilePath();
        int filename_index=tempFilePath.lastIndexOf("/");
        QString tempFileName=tempFilePath.right(tempFilePath.size()-filename_index-1);

//        qDebug()<<"file name:"<<tempFileName;
        QString tempFileName_NoSuffix=tempFileName;
        int suffix_index=tempFileName_NoSuffix.lastIndexOf(".");
        tempFileName_NoSuffix.truncate(suffix_index);
//        qDebug()<<"the no_suffix filename:"<<tempFileName_NoSuffix;
        QString icon_file_path="";
        gen_shot_picture(tempFileName_NoSuffix,icon_file_path,tempFileName);

        QListWidgetItem *pItem;
        if(icon_file_path==""){
            pItem=new QListWidgetItem(QIcon(":/icon/no_shotvideo.png"),tempFileName);
        }else{

            QPixmap objPixmap(icon_file_path);
            QListWidgetItem *pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(20,20))),tempFileName);
            pItem->setSizeHint(QSize(22,22));
        }
        pItem->setSizeHint(QSize(90,90));
        ui->listWidget_file->addItem(pItem);
    }

}

void videoWidget::gen_shot_picture(QString tempFileName_NoSuffix,QString& file_path,QString tempFileName)
{
    #if defined(Q_OS_LINUX)

        file_path="";
    #else

        const QString cmd="E:\\tech_practise\\T3_linux\\T3_linux\\T3_linux\\shared\\bin\\ffmpeg.exe";
        QStringList arg;

        arg.append("-i");
        arg.append("E:\\tech_practise\\DvrUI\\DvrUI\\video\\"+tempFileName);
        arg.append("-y");
        arg.append("-r");
        arg.append("1");
        arg.append("-f");
        arg.append("image2");
        arg.append("-ss");
        arg.append("00:00:10");
        arg.append("-t");
        arg.append("00:00:01");
        arg.append(tempFileName_NoSuffix+".jpg");
        qDebug()<<"arg:"<<arg;
        const QString now_dir="E:\\tech_practise\\DvrUI\\DvrUI\\";
        QProcess::startDetached(cmd,arg,now_dir);
        file_path="E:/tech_practise/DvrUI/DvrUI/"+tempFileName_NoSuffix+".jpg";
        qDebug()<<file_path;
    #endif
}
videoWidget::~videoWidget()
{
    delete ui;
}
void videoWidget::play_video(QModelIndex pos)
{
    QListWidgetItem* item=ui->listWidget_file->currentItem();
    qDebug()<<"filename"<<item->text();
    qDebug()<<"which file:"<<pos.row();
    QString file_name;
    #if defined(Q_OS_LINUX)
    file_name=linux_usb_path+item->text();
    #else
    file_name=win_path+item->text();
    #endif
    fileInfo_to_play=QFileInfo(file_name);
    int ret=0;
    ret=video_support_or_not(fileInfo_to_play);
    if(!ret){
        video_players=new Video_Player(this);
        emit hide_moviedesktop();
    //    pStaticMaindesktop->setHidden(true);
        emit main_desktop_disvisible();
        video_players->showNormal();
    }
}
void videoWidget::deal_picture_views_signal()
{
    emit on_unhide_moviedesktop();
}

//void videoWidget::on_change_viewmode_clicked()
//{
//    if(show_model){
//        ui->listWidget_file->setViewMode(QListView::ListMode);
//        show_file_by_listview(m_DirIterator);
//        ui->listWidget_file->setIconSize(QSize(22,22));
//        show_model=false;
//    }else{
//        ui->listWidget_file->setViewMode(QListView::IconMode);
//        show_file_by_iconview(m_DirIterator);
//        ui->listWidget_file->setIconSize(QSize(100,100));
//        show_model=true;
//    }
//}
