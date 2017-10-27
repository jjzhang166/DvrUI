#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QDialog>
#include <QDebug>
#include <QListWidget>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QRect>
#include <QListWidgetItem>
#include <QModelIndex>
#include "picture_view.h"
#include "midwindow.h"
#include <QFileInfo>
#include <QDir>
namespace Ui {
class pictureWidget;
}

class pictureWidget : public QDialog
{
    Q_OBJECT
signals:
    void hide_moviedesktop();
    void on_unhide_moviedesktop();
    void main_desktop_disvisible();
public slots:
    void deal_picture_views_signal();
public:
    explicit pictureWidget(QWidget *parent = 0);
    ~pictureWidget();
    void show_file_by_iconview(QFileInfoList file_list);
    void show_file_by_listview(QFileInfoList file_list);
public slots:
    void show_big_picture(QModelIndex);
private:
    Ui::pictureWidget *ui;
    Picture_view *picture_views;
    bool show_model;
    void show_file();
    QFileInfoList GetFileList(QDir dir);
    QFileInfoList file_list;
    void on_usb_mount();
    void on_usb_umount();
};

#endif // PICTUREWIDGET_H
