#include "videowidget.h"
#include "ui_videowidget.h"
#include <QDesktopWidget>
// This is available in all editors.
QString which_filename_to_play;

videoWidget::videoWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::videoWidget)
{
    ui->setupUi(this);
//    ui->listWidget_file=new QListWidget(this);

    ui->listWidget_file->setObjectName(QString::fromUtf8("listWidget_file"));
    ui->listWidget_file->setGeometry(QRect(0,0,0,0));
    QDirIterator m_DirIterator(QString("/home/libo/桌面/ui_qt/DvrUI/video"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    qDebug()<<" 当前目录为："<<m_DirIterator.path();
    ui->listWidget_file->clear();
    while (m_DirIterator.hasNext()) {
        QString tempFile=m_DirIterator.next();
        qDebug()<<"当前文件信息为："<<tempFile;
        QString tempFileName=tempFile.remove(QString("/home/libo/桌面/ui_qt/DvrUI/video/"),Qt::CaseSensitive);
        QString tempFileName_NoSuffix=tempFileName;
        int suffix_index=tempFileName_NoSuffix.lastIndexOf(".");
        tempFileName_NoSuffix.truncate(suffix_index);
        qDebug()<<"去掉后缀后的文件名："<<tempFileName_NoSuffix;
        ui->listWidget_file->setIconSize(QSize(100,100));
        ui->listWidget_file->setResizeMode(QListView::Adjust);
        ui->listWidget_file->setViewMode(QListView::IconMode);
        ui->listWidget_file->setMovement(QListView::Static);
        //选择模式有:ExtendedSelection 按住ctrl多选,
        //SingleSelection 单选 MultiSelection 点击多选,
        //ContiguousSelection 鼠标拖拉多选
        ui->listWidget_file->setSelectionMode(QAbstractItemView::ExtendedSelection);
        //设置自动排序
        ui->listWidget_file->setSortingEnabled(true);
        //设置拖拉
        ui->listWidget_file->setDragEnabled(true);
        //设置全选
        //ui->listWidget_file->selectAll();
        ui->listWidget_file->setSpacing(12);
        //生成缩略图文件
        const QString cmd="E:\\tech_practise\\T3_linux\\T3_linux\\T3_linux\\shared\\bin\\ffmpeg.exe";
        QStringList arg;
        arg.append("-i");
        arg.append("E:\\tech_practise\\T3_linux\\T3_linux\\T3_linux\\video\\"+tempFileName);
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
        const QString now_dir="E:\\tech_practise\\T3_linux\\T3_linux\\T3_linux";
        QProcess::startDetached(cmd,arg,now_dir);
        QString file_path="/home/libo/桌面/ui_qt/DvrUI/"+tempFileName_NoSuffix+".jpg";
        qDebug()<<file_path;
        QPixmap objPixmap(file_path);
        //tempFile=tempFile.remove(QString("E:/tech_practise/T3_linux/T3_Linux/T3_Linux/image/"),Qt::CaseSensitive);
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(90,70))),tempFile);
        pItem->setSizeHint(QSize(90,90));            //设置单元项的宽度和高度

        ui->listWidget_file->addItem(pItem);              //添加QListWidgetItem
    }
    ui->listWidget_file->setGeometry(NULL,NULL,450,420);
    connect(ui->listWidget_file,SIGNAL(clicked(QModelIndex)),this,SLOT(play_video(QModelIndex)));
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
    video_players=new Video_Player();
    video_players->exec();
//        player_video=new Player_Video();
//    const QRect availableGeometry = QApplication::desktop()->availableGeometry(player_video);

//    player_video->resize(availableGeometry.width() / 6, availableGeometry.height() / 4);

//    player_video->exec();
}
