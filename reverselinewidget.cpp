#include "reverselinewidget.h"
#include <QPainter>
reverseLineWidget::reverseLineWidget(QWidget *parent) : QWidget(parent)
{
    this->setMaximumSize(1000,430);
    this->setMinimumSize(1000,430);
}
void reverseLineWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
//    painter.rotate(20);
    //反走样，防锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    //设置颜色和宽度
    painter.setPen(QPen(QColor(231,255,15),5));

    static const QPointF point[4]={
        QPointF(350,10),
        QPointF(50,430),
        QPointF(950,430),
        QPointF(650,10)
    };
    painter.drawPolygon(point,4);
    painter.setPen(QPen(QColor(255,12,48),5));
    painter.drawLine(QPointF(250,150),QPointF(750,150));


    painter.setPen(QPen(QColor(255,12,48),5));
    painter.drawLine(QPointF(150,290),QPointF(850,290));

    painter.setPen(QPen(QColor(231,255,15),3));
    painter.drawText(QPoint(660,10),QString("3m"));

    painter.setPen(QPen(QColor(231,255,15),3));
    painter.drawText(QPoint(760,150),QString("1.5m"));

    painter.setPen(QPen(QColor(231,255,15),3));
    painter.drawText(QPoint(860,290),QString("0.5m"));



}
