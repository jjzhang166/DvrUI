#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    //将Tab设置为横向
    ui->settingTab->setTabPosition(QTabWidget::West);
//    ui->settingTab->setTabShape(QTabWidget::Triangular);
    ui->settingTab->tabBar()->setStyle(new CustomTabStyle(130,86));
    //设置Tab标签的名称
//    ui->settingTab->setTabText(0,tr("预览设置"));
//    ui->settingTab->setTabText(1,tr("ADAS"));
//    ui->settingTab->setTabText(2,tr("倒车线设置"));
//    ui->settingTab->setTabText(3,tr("时间设置"));
//    ui->settingTab->setTabText(4,tr("关于"));
//    ui->settingTab->setCurrentIndex(4);
    ui->settingTab->clear();
    #if 0
    ui->settingTab->setStyleSheet("QTabWidget::pane{ \
            border-left: 1px solid #eeeeee;\
        }");
    #endif
    //隐藏窗口的边框
    //this->setWindowFlags(Qt::FramelessWindowHint);
    //设置窗口为透明的
    //this->setAttribute(Qt::WA_TranslucentBackground);
    //设置按钮的ICON

    //为tabbar加入widget
    abouts=new About();
    ui->settingTab->addTab(abouts,tr("关于"));
    previews=new Preview();
    ui->settingTab->insertTab(0,previews,tr("预览设置"));
    adass=new ADAS();
    ui->settingTab->insertTab(1,adass,tr("ADAS"));
    reverselines=new ReverseLine();
    ui->settingTab->insertTab(2,reverselines,tr("倒车线设置"));
    timesettings=new TimeSetting();
    ui->settingTab->insertTab(3,timesettings,tr("时间设置"));
    ui->settingTab->setCurrentIndex(0);

    ui->returnButton->setStyleSheet(tr("background-image: url(:/image/image/return.png);"));
    connect(ui->returnButton,SIGNAL(clicked(bool)),this,SLOT(on_click_returnButton(bool)));

}

Settings::~Settings()
{
    delete ui;
}
void Settings::on_click_returnButton(bool)
{

    this->close();
}
