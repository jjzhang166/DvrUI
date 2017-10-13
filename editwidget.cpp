#include "editwidget.h"
#include "ui_editwidget.h"
#include <QDebug>
#include <QMessageBox>
#define LINUX_PATH "/"
#define WINd_PATH "E:"

editWidget::editWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editWidget)
{
    ui->setupUi(this);
//    fileDialog=new QFileDialog(ui->scrollArea);
//    #if defined(Q_OS_LINUX)
//    fileDialog->setDirectory(LINUX_PATH);
//    #else
//    fileDialog->setDirectory(WINd_PATH);
//    #endif
//    fileDialog->setViewMode(QFileDialog::Detail);
//    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
//    fileDialog->setFileMode(QFileDialog::AnyFile);
//    open_file();
    fileDialog =new QFileDialog(this);
            fileDialog->setWindowTitle(tr("Open Image"));
            fileDialog->setDirectory("E:");
//            fileDialog->setFilter(tr("Image Files(*.*)"));
            if(fileDialog->exec() == QDialog::Accepted) {
                    QString path = fileDialog->selectedFiles()[0];
                    QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
            }else {
                    QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
            }


}
void editWidget::open_file()
{

    if(fileDialog->exec() == QDialog::Accepted) {
            QString path = fileDialog->selectedFiles()[0];
            QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    }else {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    }
}
editWidget::~editWidget()
{
    delete ui;
}
