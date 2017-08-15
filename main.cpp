#include "main_desktop.h"
#include <QApplication>
#include <QTextCodec>
#include <QFont>
#include <QTranslator>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //中文支持
    QTranslator translator(0);
    translator.load("filemanager_zh",".");
    a.installTranslator(&translator);

    QTranslator qttranslator(0);
    qttranslator.load("qt_zh_CN",".");
    a.installTranslator(&qttranslator);

    QTranslator fileinfotranslator(0);
    fileinfotranslator.load("qfile_zh",".");
    a.installTranslator(&fileinfotranslator);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    //QT5版本以上默认就是采用UTF-8编码
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    a.setFont(QFont("WenQuanYi Zen Hei", 12));
    main_desktop w;
    w.show();

    return a.exec();
}
