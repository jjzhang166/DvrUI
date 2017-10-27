#include "picturewidget.h"
#include "ui_picturewidget.h"
#include "main_desktop.h"
#include "midwindow.h"
#include "frmmessagebox.h"
extern main_desktop* pStaticMaindesktop;
// This is available in all editors.
extern QFileInfo fileInfo_to_show;
const QString win_path="E:/tech_practise/DvrUI/DvrUI/";
//const QString linux_path="/mnt/sdcard/mmcblk1p1/frontVideo/";//sdcard
extern QString linux_usb_path;//U
extern QString linux_sdcard_path;//sd

extern MidWindow* midwindow;
const int W_ICONSIZE = 100;
const int H_ICONSIZE = 100;
const int H_ITEMSIZE = 120;
pictureWidget::pictureWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pictureWidget)
{
    ui->setupUi(this);

    show_model=true;
    ui->listWidget_file->clear();

    show_file();

    connect(this,SIGNAL(main_desktop_disvisible()),pStaticMaindesktop,SLOT(on_main_desktop_disvisible()));

    connect(midwindow,SIGNAL(usb_insert()),this,SLOT(on_usb_mount()));
    connect(midwindow,SIGNAL(usb_delete()),this,SLOT(on_usb_umount()));


    connect(ui->listWidget_file,SIGNAL(clicked(QModelIndex)),this,SLOT(show_big_picture(QModelIndex)));

}
void pictureWidget::show_file()
{
    qDebug()<<"pic widget is showing file";
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
pictureWidget::~pictureWidget()
{
    delete ui;
}
void pictureWidget::on_usb_mount()
{
    qDebug()<<"videowidget get the signal usb insert";
    show_file();
}
void pictureWidget::on_usb_umount()
{
    qDebug()<<"videowidget get the signal usb delete";
    ui->listWidget_file->clear();
    frmMessageBox *msg=new frmMessageBox;
    msg->SetMessage(QString(tr("usb is out!")),0);
    QTimer::singleShot(2000, msg, SLOT(close()));
    msg->exec();
}
QFileInfoList pictureWidget::GetFileList(QDir dir)
{
    qDebug()<<"GetFileList:"<<"now path is :"<<dir.path();
    qDebug()<<"get all file name";
    QStringList filters;
     filters << "*.bmp"<<"*.jpg"<<"*.jpeg"<<"*.png"<<"*.BMP";
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
void pictureWidget::show_file_by_iconview(QFileInfoList file_list)
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

        QPixmap objPixmap(tempFilePath);
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(W_ICONSIZE,H_ICONSIZE))),tempFileName_NoSuffix);
        pItem->setSizeHint(QSize(W_ICONSIZE,H_ITEMSIZE));
        ui->listWidget_file->addItem(pItem);
    }
}
void pictureWidget::show_file_by_listview(QFileInfoList file_list)
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

        QPixmap objPixmap(tempFilePath);
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(20,20))),tempFileName);
        pItem->setSizeHint(QSize(W_ICONSIZE,H_ITEMSIZE));
        ui->listWidget_file->addItem(pItem);
    }
}
void pictureWidget::show_big_picture(QModelIndex pos)
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
    fileInfo_to_show=QFileInfo(file_name);

    picture_views=new Picture_view(this);
    emit hide_moviedesktop();
    emit main_desktop_disvisible();
    picture_views->showNormal();
}
void pictureWidget::deal_picture_views_signal()
{
    emit on_unhide_moviedesktop();
}
