#include "slave_picturewidget.h"
#include "ui_slave_picturewidget.h"

slave_PictureWidget::slave_PictureWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::slave_PictureWidget)
{
    ui->setupUi(this);
}

slave_PictureWidget::~slave_PictureWidget()
{
    delete ui;
}
