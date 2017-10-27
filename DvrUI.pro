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
    slave_videowidget.cpp \
    cprojectionpicture.cpp \
    frmmessagebox.cpp \
    reverselinewidget.cpp \
    reverseline_setting.cpp \
    midwindow.cpp \
    moviedesk1.cpp

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
    slave_videowidget.h \
    cprojectionpicture.h \
    frmmessagebox.h \
    reverselinewidget.h \
    reverseline_setting.h \
    midwindow.h \
    moviedesk1.h

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
    slave_videowidget.ui \
    frmmessagebox.ui \
    reverseline_setting.ui \
    moviedesk1.ui

RESOURCES += \
    image/image.qrc \
    image/font.qrc

#unix:!macx: LIBS += -L$$PWD/../sdk/ -lsdk -L$$PWD/../sdk/libs/ -lsdk_func -lmdas \
unix:!macx: LIBS += -L$$PWD/../sdk/ -lsdk -L$$PWD/../sdk/libs/ -lmdas -lsdk_compose  -lsdk_disp  -lsdk_dvr   -lsdk_player  -lsdk_tr \
                                        -L$$PWD/../sdk/cedarx/lib/ -Wl,--no-undefined  -lcdx_playback -lcdx_common -laencoder -lawrecorder -lxplayer   -lcdx_muxer \
             -lcdx_parser -lcdc_vdecoder -lcdx_stream   -lcdc_vencoder -lcdc_ve  -lcdc_sdecoder -lcdc_base -lcdc_memory -ladecoder -laencoder -lxmetadataretriever -lcdx_base -lrt -lpthread -lz -lcrypto -lssl -ltinyalsa

INCLUDEPATH += $$PWD/../sdk $$PWD/../sdk/include $$PWD/../sdk/include/disp2 $$PWD/../sdk/media $$PWD/../sdk/cedarx/include $$PWD/../sdk/camera $$PWD/../sdk/include/utils . ./src/ $$PWD/../../../external-packages/gpu/mali400/fbdev/header_files/include/  $$PWD/../sdk/cedarx/include $$PWD/../sdk/cedarx/include/libcedarc/include $$PWD/../sdk/cedarx/include/DEMUX/MUXER/include/include/ \
                  $$PWD/../sdk/cedarx/include/DEMUX/BASE/include/include $$PWD/../sdk/cedarx/include/DEMUX/PARSER/include/include/ $$PWD/../sdk/cedarx/include/DEMUX/STREAM/include/include $$PWD/../sdk/cedarx/include/PLAYER/include/ \
                   $$PWD/../../../../tools/pack/chips/sun8iw11p1/hal/gpu/fbdev/include  $$PWD/Commons $$PWD/../sdk/storage/ $$PWD/../sdk/sound/ $$PWD/../sdk/misc/ \
                                   $$PWD/../sdk/camera/allwinnertech/include $$PWD/../sdk/camera/allwinnertech/water_mark/ $$PWD/../sdk/camera/adas/ $$PWD/../sdk/storage/ $$PWD/../sdk/sound/ \
                                  $$PWD/../sdk/cedarx/include/libcore/muxer/include \
                                 $$PWD/../sdk/cedarx/include/libcore/base/include/ \
                                  $$PWD/../sdk/cedarx/include/libcore/parser/include \
                                  $$PWD/../sdk/cedarx/include/libcore/stream/include \
                                 $$PWD/../sdk/cedarx/include/libcore/playback/include/ \
                                $$PWD/../sdk/cedarx/include/libcore/common/iniparser \
                                $$PWD/../sdk/cedarx/include/libcore/
#DEFINES += QT_ROTATE_APP

DEPENDPATH += $$PWD/../sdk $$PWD/../sdk/include $$PWD/../sdk/include/disp2 $$PWD/../sdk/media $$PWD/../sdk/cedarx/include $$PWD/../sdk/camera $$PWD/../sdk/include/utils . ./src/ $$PWD/../../../external-packages/gpu/mali400/fbdev/header_files/include/ $$PWD/../sdk/cedarx/include $$PWD/../sdk/cedarx/include/libcedarc/include $$PWD/../sdk/cedarx/include/DEMUX/MUXER/include/include/ \
                  $$PWD/../sdk/cedarx/include/DEMUX/BASE/include/include $$PWD/../sdk/cedarx/include/DEMUX/PARSER/include/include/ $$PWD/../sdk/cedarx/include/DEMUX/STREAM/include/include $$PWD/../sdk/cedarx/include/PLAYER/include/ \
                $$PWD/../../../../tools/pack/chips/sun8iw11p1/hal/gpu/fbdev/include $$PWD/../sdk/storage/ $$PWD/../sdk/sound/  $$PWD/../sdk/misc/ \
                                  $$PWD/../sdk/cedarx/include/libcore/muxer/include \
                                 $$PWD/../sdk/cedarx/include/libcore/base/include/ \
                                  $$PWD/../sdk/cedarx/include/libcore/parser/include \
                                  $$PWD/../sdk/cedarx/include/libcore/stream/include \
                                 $$PWD/../sdk/cedarx/include/libcore/playback/include/ \
                                $$PWD/../sdk/cedarx/include/libcore/common/iniparser \
                                $$PWD/../sdk/cedarx/include/libcore/
#DEFINES += QT_ROTATE_APP
DEFINES += CDX_V27 HAVE_PTHREADS SUPPORT_NEW_DRIVER HAVE_SYS_UIO_H ANDROID_SMP=1 __ARM_HAVE_DMB __ARM_HAVE_LDREX_STREX __ARM_HAVE_LDREXD \
                                __ARM_HAVE_VFP __ARM_HAVE_NEON OPTION_CC_GNUEABIHF=1 OPTION_CC_GNUEABI=2 OPTION_CC_UCGNUEABI=3 \
           _GNU_SOURCE OPTION_CC_GNUEABIHF=1 OPTION_CC_GNUEABI=2 OPTION_CC_UCGNUEABI=3 OPTION_CC_LINUX_UCGNUEABI=4 OPTION_CC_LINUX_MUSLGNUEABI=5 OPTION_CC_LINUX_MUSLGNUEABI64=6 OPTION_OS_ANDROID=1 OPTION_OS_LINUX=2 OPTION_OS_VERSION_ANDROID_4_2=1 OPTION_OS_VERSION_ANDROID_4_4=2 OPTION_OS_VERSION_ANDROID_5_0=3 OPTION_OS_VERSION_ANDROID_6_0=4 OPTION_MEMORY_DRIVER_SUNXIMEM=1 OPTION_MEMORY_DRIVER_ION=2 OPTION_PRODUCT_PAD=1 OPTION_PRODUCT_TVBOX=2 OPTION_PRODUCT_OTT_CMCC=3 OPTION_PRODUCT_IPTV=4 OPTION_PRODUCT_DVB=5 OPTION_PRODUCT_LOUDSPEAKER=6 OPTION_CHIP_R8=1 OPTION_CHIP_R16=2 OPTION_CHIP_C500=3 OPTION_CHIP_R58=4 OPTION_CHIP_R18=5 OPTION_CHIP_T2=6 OPTION_CHIP_T3=7 OPTION_DRAM_INTERFACE_DDR1_16BITS=1 OPTION_DRAM_INTERFACE_DDR1_32BITS=2 OPTION_DRAM_INTERFACE_DDR2_16BITS=3 OPTION_DRAM_INTERFACE_DDR2_32BITS=4 OPTION_DRAM_INTERFACE_DDR3_16BITS=5 OPTION_DRAM_INTERFACE_DDR3_32BITS=6 OPTION_DRAM_INTERFACE_DDR3_64BITS=7 OPTION_CMCC_NO=0 OPTION_CMCC_YES=1 OPTION_DTV_NO=0 OPTION_DTV_YES=1 OPTION_ALI_YUNOS_NO=0 OPTION_ALI_YUNOS_YES=1 OPTION_IS_CAMERA_DECODER_NO=0 OPTION_IS_CAMERA_DECODER_YES=1 OPTION_VE_IPC_DISABLE=1 OPTION_VE_IPC_ENABLE=2 OPTION_NO_DEINTERLACE=0 OPTION_HW_DEINTERLACE=1 OPTION_SW_DEINTERLACE=2 LINUX_VERSION_3_4=1 LINUX_VERSION_3_10=2 DROP_DELAY_FRAME_NONE=0 DROP_DELAY_FRAME_720P=1 DROP_DELAY_FRAME_4K=2 ZEROCOPY_PIXEL_FORMAT_NONE=0 ZEROCOPY_PIXEL_FORMAT_YUV_MB32_420=1 ZEROCOPY_PIXEL_FORMAT_YV12=2 ZEROCOPY_PIXEL_FORMAT_NV21=3 GPU_Y16_C16_ALIGN=1 GPU_Y32_C16_ALIGN=2 GPU_Y16_C8_ALIGN=3 ZEROCOPY_HAL_PIXEL_FORMAT_AW_YUV_PLANNER420=1 ZEROCOPY_HAL_PIXEL_FORMAT_AW_YVU_PLANNER420=2 DEINTERLACE_FORMAT_MB32_12=1 DEINTERLACE_FORMAT_NV=2 DEINTERLACE_FORMAT_NV21=3 DEINTERLACE_FORMAT_NV12=4 OUTPUT_PIXEL_FORMAT_NV21=1 OUTPUT_PIXEL_FORMAT_YV12=2 OUTPUT_PIXEL_FORMAT_MB32=3 GPU_ALIGN_STRIDE_16=1 GPU_ALIGN_STRIDE_32=2 OPTION_HAVE_ZLIB=1 OPTION_NO_ZLIB=2 OPTION_HAVE_SSL=1 OPTION_NO_SSL=2 OPTION_HAVE_LIVE555=1 OPTION_NO_LIVE555=2 CONFIG_CC=4 CONFIG_OS=2 CONFIG_CHIP=7 CONFIG_ALI_YUNOS=0 CONFIG_OS_VERSION=2 CONFIG_TARGET_PRODUCT= CONFIG_PRODUCT=2 CONFIG_HAVE_ZLIB=1 CONFIG_HAVE_SSL=1 CONFIG_HAVE_LIVE555=2 CONFIG_MEMORY_DRIVER=2 CONFIG_DRAM_INTERFACE=6 CONFIG_VE_IPC=1 CONFIG_CMCC=0 CONFIG_DTV=0 CONFIG_IS_CAMERA_DECODER=0 CONFIG_DEINTERLACE=0 GPU_TYPE_MALI=1 CONFIG_ARM_VERSION= ENABLE_SUBTITLE_DISPLAY_IN_CEDARX=0 DTV=no LINUX_VERSION=2 USE_NEW_BDMV_STREAM=0 PLAYREADY_DRM_INVOKE=0 H265_4K_CHECK_SCALE_DOWN=1 NON_H265_4K_NOT_SCALE_DOWN=0 SUPPORT_H265=0 ANTUTU_NOT_SUPPORT=1 ENABLE_MEDIA_BOOST=1 DROP_DELAY_FRAME=2 ROTATE_PIC_HW=0 VE_PHY_OFFSET=0x40000000 ZEROCOPY_PIXEL_FORMAT=3 GPU_Y_C_ALIGN=1 SEND_3_BLACK_FRAME_TO_GPU=0 ZEROCOPY_DYNAMIC_CHECK=1 GRALLOC_PRIV=0 VIDEO_DIRECT_ACCESS_DE=0 KEY_PARAMETER_GET=0 DISPLAY_CMD_SETVIDEOSIZE_POSITION=0 DEINTERLACE_IOWR=0 DEINTERLACE_FORMAT=4 NEW_DISPLAY_DOUBLE_STREAM_NEED_NV21=1 OUTPUT_PIXEL_FORMAT=1 NOT_DROP_FRAME=1 SOUND_DEVICE_SET_RAW_FLAG=0 NATIVE_WIN_DISPLAY_CMD_GETDISPFPS=0 IMG_NV21_4K_ALIGN=0 DEINTERLACE_ADDR_64=0

DEFINES +=  WATERMARK_ENABLE HAVE_PRCTL WATERMARK ADAS_ENABLE

TRANSLATIONS +=filemanager_zh.ts

OTHER_FILES += \
    filemanager_zh.qm \
    filemanager_zh.ts
