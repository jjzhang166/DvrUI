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


namespace Ui {
class pictureWidget;
}

class pictureWidget : public QDialog
{
    Q_OBJECT

public:
    explicit pictureWidget(QWidget *parent = 0);
    ~pictureWidget();
public slots:
    void show_big_picture(QModelIndex);
private:
    Ui::pictureWidget *ui;
    Picture_view *picture_views;
};

#endif // PICTUREWIDGET_H
