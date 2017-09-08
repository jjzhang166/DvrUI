#include "videowidget.h"
#include "ui_videowidget.h"
#include <QDesktopWidget>
#include "main_desktop.h"
extern main_desktop* pStaticMaindesktop;
// This is available in all editors.
QString which_filename_to_play;
const QString win_path="E:/tech_practise/DvrUI/DvrUI/video/";
const QString linux_path="/mnt/sdcard/mmcblk1p1/frontVideo/";
videoWidget::videoWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::videoWidget)
{
    ui->setupUi(this);
//    ui->listWidget_file=new QListWidget(this);
    show_model=true;//true为显示icon模式，false为显示list模式
    ui->listWidget_file->clear();
    #if defined(Q_OS_LINUX)
        m_DirIterator=new QDirIterator(linux_path,QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    #else
        m_DirIterator=new QDirIterator(win_path,QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    #endif
    ui->listWidget_file->setObjectName(QString::fromUtf8("listWidget_file"));
//    ui->listWidget_file->setGeometry(QRect(0,0,0,0));
    ui->listWidget_file->setMovement(QListView::Static);
    //选择模式有:ExtendedSelection 按住ctrl多选,
    //SingleSelection 单选 MultiSelection 点击多选,
    //ContiguousSelection 鼠标拖拉多选
    ui->listWidget_file->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //设置自动排序
    ui->listWidget_file->setSortingEnabled(true);

    //设置全选
    //ui->listWidget_file->selectAll();
    ui->listWidget_file->setSpacing(12);
    ui->listWidget_file->setResizeMode(QListView::Adjust);
    if(show_model){
        //缩略图显示
        ui->listWidget_file->setViewMode(QListView::IconMode);
        ui->listWidget_file->setIconSize(QSize(100,100));
        //设置拖拉
        ui->listWidget_file->setDragEnabled(true);
        show_file_by_iconview(m_DirIterator);
    }else{
        //列表显示
        ui->listWidget_file->setViewMode(QListView::ListMode);
        ui->listWidget_file->setDragEnabled(false);
        ui->listWidget_file->setIconSize(QSize(22,22));
        show_file_by_listview(m_DirIterator);
    }
    qDebug()<<" 当前目录为："<<m_DirIterator->path();
//    ui->listWidget_file->setGeometry(NULL,NULL,450,420);
    connect(ui->listWidget_file,SIGNAL(clicked(QModelIndex)),this,SLOT(play_video(QModelIndex)));
    connect(this,SIGNAL(main_desktop_disvisible()),pStaticMaindesktop,SLOT(on_main_desktop_disvisible()));
}
void videoWidget::show_file_by_iconview(QDirIterator* m_DirIterator)
{
    ui->listWidget_file->setIconSize(QSize(100,100));
    while (m_DirIterator->hasNext()) {
        QString tempFile=m_DirIterator->next();
        qDebug()<<"当前文件信息为："<<tempFile;
        #if defined(Q_OS_LINUX)
            QString tempFileName=tempFile.remove(linux_path,Qt::CaseSensitive);
        #else
            QString tempFileName=tempFile.remove(win_path,Qt::CaseSensitive);
        #endif
        QString tempFileName_NoSuffix=tempFileName;
        int suffix_index=tempFileName_NoSuffix.lastIndexOf(".");
        tempFileName_NoSuffix.truncate(suffix_index);
        qDebug()<<"去掉后缀后的文件名："<<tempFileName_NoSuffix;
        QString file_path="";
        gen_shot_picture(tempFileName_NoSuffix,file_path,tempFileName);
//        file_path="";
        QListWidgetItem *pItem;
//        tempFile=tempFile.remove(win_path,Qt::CaseSensitive);
        if(file_path==""){//如果没有得到缩略图
            pItem=new QListWidgetItem(QIcon(":/icon/no_shotvideo.png"),tempFileName);
        }else{
            //生成了缩略图
            QPixmap objPixmap(file_path);
            pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(90,70))),tempFileName);
        }
        pItem->setSizeHint(QSize(90,90));            //设置单元项的宽度和高度
        ui->listWidget_file->addItem(pItem);              //添加QListWidgetItem
    }
}
void videoWidget::show_file_by_listview(QDirIterator* m_DirIterator)
{
    ui->listWidget_file->setIconSize(QSize(22,22));
    while (m_DirIterator->hasNext()) {
        QString tempFile=m_DirIterator->next();
        qDebug()<<"当前文件信息为："<<tempFile;
        #if defined(Q_OS_LINUX)
            QString tempFileName=tempFile.remove(linux_path,Qt::CaseSensitive);
        #else
            QString tempFileName=tempFile.remove(win_path,Qt::CaseSensitive);
        #endif
        QString tempFileName_NoSuffix=tempFileName;
        int suffix_index=tempFileName_NoSuffix.lastIndexOf(".");
        tempFileName_NoSuffix.truncate(suffix_index);
        qDebug()<<"去掉后缀后的文件名："<<tempFileName_NoSuffix;
        QString file_path="";
        gen_shot_picture(tempFileName_NoSuffix,file_path,tempFileName);
        QPixmap objPixmap(file_path);
        tempFile=tempFile.remove(win_path,Qt::CaseSensitive);
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(20,20))),tempFile);
        pItem->setSizeHint(QSize(22,22));            //设置单元项的宽度和高度
        ui->listWidget_file->addItem(pItem);              //添加QListWidgetItem
    }
}
//windows下生成缩略图的方法
void videoWidget::gen_shot_picture(QString tempFileName_NoSuffix,QString& file_path,QString tempFileName)
{
    #if defined(Q_OS_LINUX)
    //linux下生成缩略图的方法
        file_path="";
    #else
    //win下生成缩略图的方法（使用ffmepeg）
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
    qDebug()<<"点击了"<<pos.row();
    which_filename_to_play=item->text();
//    which_pic_show_big=pos.row();
    video_players=new Video_Player(this);
//    this->hide();
    emit hide_moviedesktop();
    connect(video_players,SIGNAL(p_unhide_moviedesktop()),this,SLOT(deal_picture_views_signal()));
    video_players->showNormal();
//        player_video=new Player_Video();
//    const QRect availableGeometry = QApplication::desktop()->availableGeometry(player_video);
//    player_video->resize(availableGeometry.width() / 6, availableGeometry.height() / 4);
//    player_video->exec();

    emit main_desktop_disvisible();
}
void videoWidget::deal_picture_views_signal()
{
    emit on_unhide_moviedesktop();
}
//列表和icon模式的切换，还未调试好
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
