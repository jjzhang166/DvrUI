#include "settings.h"
#include "ui_settings.h"
#include "setfirst.h"
#include "ui_setfirst.h"

#include <QDesktopWidget>
Settings* pStatic_settings;
Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

//    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->settingTab->setTabPosition(QTabWidget::West);
    ui->settingTab->setWindowFlags(Qt::FramelessWindowHint);
//    ui->settingTab->setTabShape(QTabWidget::Triangular);
    ui->settingTab->tabBar()->setStyle(new CustomTabStyle(130,80));

    ui->settingTab->clear();
    #if 0
    ui->settingTab->setStyleSheet("QTabWidget::pane{ \
            border-left: 0px solid #eeeeee;\
        }");
    #endif


    abouts=new About(this);
    ui->settingTab->addTab(abouts,tr("关于"));

    previews=new Preview(this);
    ui->settingTab->insertTab(0,previews,tr("预览设置"));

    adass=new ADAS(this);
    ui->settingTab->insertTab(1,adass,tr("ADAS"));

    reverselines=new ReverseLine(this);
    ui->settingTab->insertTab(2,reverselines,tr("倒车线设置"));

    timesettings=new TimeSetting(this);
    ui->settingTab->insertTab(3,timesettings,tr("时间设置"));

    ui->settingTab->setCurrentIndex(0);

    connect(ui->returnButton,SIGNAL(clicked()),this,SLOT(on_returnButton_clicked()));

    connect(previews,SIGNAL(hideSettings()),this,SLOT(on_hideSettings()));
    connect(adass,SIGNAL(hideSettings()),this,SLOT(on_hideSettings()));
    connect(reverselines,SIGNAL(hideSettings()),this,SLOT(on_hideSettings()));

    connect(previews,SIGNAL(unhideSettings_signal()),this,SLOT(on_unhideSettings()));
    connect(adass,SIGNAL(unhideSettings_signal()),this,SLOT(on_unhideSettings()));
    connect(reverselines,SIGNAL(unhideSettings_signal()),this,SLOT(on_unhideSettings()));

    pStatic_settings=this;
}


    void Settings::FormInCenter()
    {
        int frmX = this->width();
        int frmY = this->height();
        QDesktopWidget w;
        int deskWidth = w.width();
        int deskHeight = w.height();
        QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
        this->move(movePoint);
    }
Settings::~Settings()
{
    delete ui;
}
void Settings::on_hideSettings()
{
    this->hide();
}
void Settings::on_unhideSettings()
{
    this->setHidden(false);
}


void Settings::on_returnButton_clicked()
{
    this->close();

    SetFirst* psetFirst=static_cast<SetFirst*>(parentWidget());
    psetFirst->setHidden(false);
}
