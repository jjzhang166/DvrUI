#include "midwindow.h"
#include <QDebug>
MidWindow::MidWindow()
{

}
void MidWindow::mount_usb()
{
    qDebug()<<"call midwindow";
    emit usb_insert();
}
void MidWindow::umount_usb()
{
    qDebug()<<"call midwindow";
    emit usb_delete();
}
void MidWindow::mount_sdcard()
{
    emit sdcard_insert();
}
void MidWindow::umount_sdcard()
{
    emit sdcard_delete();
}
