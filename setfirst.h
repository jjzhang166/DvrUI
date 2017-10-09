#ifndef SETFIRST_H
#define SETFIRST_H

#include <QDialog>
#include <QDebug>
#include "settings.h"
#include <QListView>
#if defined(Q_OS_LINUX)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vencoder.h"
#include "CdxMuxer.h"
#include <time.h>

#include "Rtc.h"
#include "StorageManager.h"
#include "DvrFactory.h"
#include "CameraFileCfg.h"
#include <sys/socket.h>
#include <sys/un.h>
#include "Fat.h"
#include "DebugLevelThread.h"
#include "DvrRecordManager.h"
using namespace android;
#endif
namespace Ui {
class SetFirst;
}

class SetFirst : public QDialog
{
    Q_OBJECT

public:
    explicit SetFirst(QWidget *parent = 0);
    ~SetFirst();
public slots://界面切换
//    void on_click_setttingsButton();
//    void on_click_returnButton();
    void on_audioButton_clicked();
    void on_ADASButton_clicked();
    void on_movieButton_clicked();
    void on_reverseButton_clicked();
public slots:
    void on_slider_valuechanged(int);
private slots:
    void on_voiceButton_clicked();

    void on_movieTimeSetting_valueChanged(int value);

    void on_settingsButton_clicked();

    void on_returnButton_clicked();

private:
    Ui::SetFirst *ui;
    Settings* setSecond_Desk;
    void setWindowStyleSheet();
    void FormInCenter();
private:
    bool voiceButtonState;
};

#endif // SETFIRST_H
