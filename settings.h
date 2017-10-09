#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QPainter>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QDate>
#include "about.h"
#include "preview.h"
#include "adas.h"
#include "reverseline.h"
#include "timesetting.h"
namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

public slots:
    void on_hideSettings();

    void on_unhideSettings();
private slots:
    void on_returnButton_clicked();

private://tabbar管理
    About *abouts;
    Preview *previews;
    ADAS *adass;
    ReverseLine *reverselines;
    TimeSetting *timesettings;
private:
    Ui::Settings *ui;
    void FormInCenter();
};


//将tabWidget设置为横向的
class CustomTabStyle : public QProxyStyle
{
public:
    explicit CustomTabStyle(int x,int y):width(x),height(y){}
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = this->width; // 设置每个tabBar中item的大小
            s.rheight() = this->height;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if(const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
                QRect allRect = tab->rect;

                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                }
                else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
private:
    int width;
    int height;
};

#endif // SETTINGS_H
