#ifndef MIDWINDOW_H
#define MIDWINDOW_H
#include <QObject>

class MidWindow:public QObject
{
    Q_OBJECT
public:
    MidWindow();
    void mount_usb();
    void umount_usb();
    void mount_sdcard();
    void umount_sdcard();
signals:
    void usb_insert();
    void usb_delete();
    void sdcard_insert();
    void sdcard_delete();

};

#endif // MIDWINDOW_H
