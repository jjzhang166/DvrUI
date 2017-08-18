#ifndef SLAVE_PICTUREWIDGET_H
#define SLAVE_PICTUREWIDGET_H

#include <QDialog>

namespace Ui {
class slave_PictureWidget;
}

class slave_PictureWidget : public QDialog
{
    Q_OBJECT

public:
    explicit slave_PictureWidget(QWidget *parent = 0);
    ~slave_PictureWidget();

private:
    Ui::slave_PictureWidget *ui;
};

#endif // SLAVE_PICTUREWIDGET_H
