#ifndef PICTURE_VIEW_H
#define PICTURE_VIEW_H

#include <QDialog>
#include <QDebug>
#include <QModelIndex>
#include <QDirIterator>
#include <QPixmap>
#include <QString>
#include <QLabel>
#include <QScrollArea>
#include <QBoxLayout>
namespace Ui {
class Picture_view;
}

class Picture_view : public QDialog
{
    Q_OBJECT

public:
    explicit Picture_view(QWidget *parent = 0);
    ~Picture_view();

private slots:
    void on_closeButton_clicked();

private:
    Ui::Picture_view *ui;
    int number_to_show;
    QLabel *pictureLabel;
    QScrollArea *scrollArea;
};

#endif // PICTURE_VIEW_H
