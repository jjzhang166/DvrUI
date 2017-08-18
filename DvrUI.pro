#-------------------------------------------------
#
# Project created by QtCreator 2017-08-15T11:20:17
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DvrUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        main_desktop.cpp \
    setfirst.cpp \
    settings.cpp \
    about.cpp \
    front_adas_setting.cpp \
    front_camera_setting.cpp \
    rear_adas_setting.cpp \
    rear_camera_setting.cpp \
    reverseline.cpp \
    timesetting.cpp \
    preview.cpp \
    adas.cpp \
    mylabel.cpp \
    editwidget.cpp \
    moviedesk.cpp \
    picture_view.cpp \
    picturewidget.cpp \
    video_player.cpp \
    videowidget.cpp \
    video_widgets.cpp \
    dashboard.cpp \
    myspeed.cpp \
    slave_picturewidget.cpp \
    slave_videowidget.cpp

HEADERS += \
        main_desktop.h \
    setfirst.h \
    settings.h \
    about.h \
    front_adas_setting.h \
    front_camera_setting.h \
    rear_adas_setting.h \
    rear_camera_setting.h \
    reverseline.h \
    timesetting.h \
    preview.h \
    adas.h \
    mylabel.h \
    editwidget.h \
    moviedesk.h \
    picture_view.h \
    video_player.h \
    videowidget.h \
    video_widgets.h \
    picturewidget.h \
    dashboard.h \
    myspeed.h \
    slave_picturewidget.h \
    slave_videowidget.h

FORMS += \
        main_desktop.ui \
    setfirst.ui \
    settings.ui \
    about.ui \
    front_adas_setting.ui \
    front_camera_setting.ui \
    rear_adas_setting.ui \
    rear_camera_setting.ui \
    reverseline.ui \
    timesetting.ui \
    preview.ui \
    adas.ui \
    editwidget.ui \
    moviedesk.ui \
    picture_view.ui \
    video_player.ui \
    videowidget.ui \
    picturewidget.ui \
    dashboard.ui \
    slave_picturewidget.ui \
    slave_videowidget.ui

RESOURCES += \
    image/image.qrc

TRANSLATIONS +=filemanager_zh.ts

OTHER_FILES += \
    filemanager_zh.qm \
    filemanager_zh.ts
