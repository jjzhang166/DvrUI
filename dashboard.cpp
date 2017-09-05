#include "dashboard.h"
#include "ui_dashboard.h"

dashBoard::dashBoard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dashBoard)
{
    ui->setupUi(this);
    int w,h;
    w=(1024-this->width())/2;
    h=(600-this->height())/2;
    this->mapToParent(QPoint(w,h));
    myspeed=new mySpeed();
    ui->scrollArea->setWidget(myspeed);
    connect(ui->pushButton,SIGNAL(clicked(bool)),SLOT(on_returnButton_clicked()));
}

dashBoard::~dashBoard()
{
    delete ui;
}
void dashBoard::on_returnButton_clicked()
{
    this->close();
}
