#include "editwidget.h"
#include "ui_editwidget.h"
#include <QDebug>
editWidget::editWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editWidget)
{
    ui->setupUi(this);
    qDebug()<<"编辑状态";
}

editWidget::~editWidget()
{
    delete ui;
}
