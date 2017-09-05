#include "picturewidget.h"
#include "ui_picturewidget.h"

// This is available in all editors.
int which_masterpic_show_big;
QString which_masterfilename_show_big;
extern bool pic_slave_or_master;

const int W_ICONSIZE = 100; //图片预览的宽
const int H_ICONSIZE = 100; //高
const int H_ITEMSIZE = 120; //每个图片加名称的大小
pictureWidget::pictureWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pictureWidget)
{
    ui->setupUi(this);
    ///home/libo/桌面/ui_qt/DvrUI/image/

    pic_slave_or_master=true;

    ui->listWidget_file->setObjectName(QString::fromUtf8("listWidget_file"));
    ui->listWidget_file->setGeometry(QRect(0,0,0,0));

    #if defined(Q_OS_LINUX)
    QDirIterator m_DirIterator(QString("/mnt/sdcard/mmcblk1p1/frontPicture/"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    #else
    QDirIterator m_DirIterator(QString("../DvrUI/image/"),QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    #endif

    qDebug()<<" 当前目录为："<<m_DirIterator.path();
    qDebug()<<"当前文件信息为："<<m_DirIterator.fileName();
    qDebug()<<"program file path"<<QDir::currentPath();
    ui->listWidget_file->clear();
    while (m_DirIterator.hasNext()) {
        QString tempFile=m_DirIterator.next();
        qDebug()<<"当前文件信息为："<<tempFile;
        ui->listWidget_file->setIconSize(QSize(W_ICONSIZE,H_ICONSIZE));
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
        QPixmap objPixmap(tempFile);

        #if defined(Q_OS_LINUX)
        tempFile=tempFile.remove(QString("/mnt/sdcard/mmcblk1p1/frontPicture/"),Qt::CaseSensitive);
        #else
        tempFile=tempFile.remove(QString("../DvrUI/image/"),Qt::CaseSensitive);
        #endif

        QListWidgetItem *pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(W_ICONSIZE,H_ICONSIZE))),tempFile);
        pItem->setSizeHint(QSize(W_ICONSIZE,H_ITEMSIZE));            //设置单元项的宽度和高度
        ui->listWidget_file->addItem(pItem);              //添加QListWidgetItem项
    }
    ui->listWidget_file->setGeometry(NULL,NULL,450,420);
    connect(ui->listWidget_file,SIGNAL(clicked(QModelIndex)),this,SLOT(show_big_picture(QModelIndex)));

}

pictureWidget::~pictureWidget()
{
    delete ui;
}
void pictureWidget::show_big_picture(QModelIndex pos)
{
    QListWidgetItem* item=ui->listWidget_file->currentItem();
//    qDebug()<<"filename"<<item->text();
//    qDebug()<<"点击了"<<pos.row();
    which_masterfilename_show_big=item->text();
    which_masterpic_show_big=pos.row();
    picture_views=new Picture_view(this);
//    this->hide();
    picture_views->showNormal();
}
