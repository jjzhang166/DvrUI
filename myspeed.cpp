#include "myspeed.h"
#include <QPainter>
#include <QtMath>
mySpeed::mySpeed(QWidget *parent)
    : QMainWindow(parent)
{
    m_background = Qt::black;
    m_foreground = Qt::green;

    m_startAngle=60;
    m_endAngle=60;
    m_scaleMajor=10;
    m_minValue=0;
    m_maxValue=100;
    m_scaleMajor = 10;//分度
    m_scaleMinor = 10;
    m_units = "km/h";
    m_title = "Speed Meter";
    m_precision = 0;
    m_value=0;

    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(100);//间隔
    connect(m_updateTimer,SIGNAL(timeout()),this,SLOT(UpdateAngle()));
      m_updateTimer->start();//启动定时器

     setWindowFlags(Qt::FramelessWindowHint);//无窗体
     setAttribute(Qt::WA_TranslucentBackground);//背景透明
     resize(150,150);
}

mySpeed::~mySpeed()
{

}
void mySpeed::drawCrown(QPainter *painter)
{
    painter->save();
    int radius =50;
    QLinearGradient lg1(0,-radius,0,radius);
    lg1.setColorAt(0.2,Qt::white);
    lg1.setColorAt(1,Qt::black);
    painter->setBrush(lg1);
    painter->drawEllipse(-50, -50 , 100 ,100);
    painter->restore();
}
void mySpeed::drawBackground(QPainter *painter)
{
    painter->save();
    painter->setBrush(m_background);
    painter->drawEllipse(-90, -90, 180 ,180);
    painter->restore();
}
void mySpeed::drawScaleNum(QPainter *painter)
{
    painter->save();
    painter->setPen(m_foreground);
    double startRad = (360 - m_startAngle - 90) * (3.14 / 180);
    double deltaRad = (360 - m_startAngle - m_endAngle) * (3.14 / 180) / m_scaleMajor;
    double sina,cosa;
    int x, y;
    QFontMetricsF fm(this->font());
    double w, h, tmpVal;
    QString str;

    for (int i = 0; i <= m_scaleMajor; i++)
    {
        sina = sin(startRad - i * deltaRad);
        cosa = cos(startRad - i * deltaRad);

       tmpVal = 1.0 * i *((m_maxValue - m_minValue) / m_scaleMajor) + m_minValue;

        str = QString( "%1" ).arg(tmpVal);
        w = fm.size(Qt::TextSingleLine,str).width();
        h = fm.size(Qt::TextSingleLine,str).height();
        x = 82 * cosa - w / 2;
        y = -82 * sina + h / 4;
        painter->drawText(x, y, str);
    }
    painter->restore();
}

void mySpeed::drawScale(QPainter *painter)
{
    painter->save();
    painter->rotate(m_startAngle);
    int steps = (m_scaleMajor * m_scaleMinor); //相乘后的值是分的份数
    double angleStep = (360.0 - m_startAngle - m_endAngle) / steps;  //每一个份数的角度
    painter->setPen(m_foreground); //
    QPen pen = painter->pen();
    for (int i = 0; i <= steps; i++)
    {
        if (i % m_scaleMinor == 0)//整数刻度显示加粗
        {
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(0, 62, 0, 72); //两个参数应该是两个坐标值
        }
        else
        {
            pen.setWidth(0);
            painter->setPen(pen);
            painter->drawLine(0, 67, 0, 72);
        }
        painter->rotate(angleStep);
    }
    painter->restore();
}
 /*

QFontMetricsF这个类用于获取字体的像素信息，例如字体的高度，一个给定字符串的像素宽度等等。

*/
void mySpeed::drawTitle(QPainter *painter)
{
    painter->save();
    painter->setPen(m_foreground);
    //painter->setBrush(m_foreground);
    QString str(m_title); //显示仪表的功能
    QFontMetricsF fm(this->font());
    double w = fm.size(Qt::TextSingleLine,str).width();
    painter->drawText(-w / 2, -30, str);
    painter->restore();
}
void mySpeed::drawNumericValue(QPainter *painter)
{
    QString str  =  QString("%1 %2").arg(m_value, 0, 'f', m_precision).arg(m_units);
    QFontMetricsF fm(font());
    double w = fm.size(Qt::TextSingleLine,str).width();
    painter->setPen(m_foreground);
    painter->drawText(-w / 2, 42, str);
}

void mySpeed::drawIndicator(QPainter *painter)
{
    painter->save();
    QPolygon pts;
    pts.setPoints(3, -2,0, 2,0, 0,60);	/* (-2,0)/(2,0)/(0,60) *///第一个参数是 ，坐标的个数。后边的是坐标

    painter->rotate(m_startAngle);
    double degRotate =  (360.0 - m_startAngle - m_endAngle)/(m_maxValue - m_minValue)*(m_value - m_minValue);

    //画指针
    painter->rotate(degRotate);  //顺时针旋转坐标系统
    QRadialGradient haloGradient(0, 0, 60, 0, 0);  //辐射渐变
    haloGradient.setColorAt(0, QColor(60,60,60));
    haloGradient.setColorAt(1, QColor(160,160,160)); //灰
    painter->setPen(Qt::white); //定义线条文本颜色  设置线条的颜色
    painter->setBrush(haloGradient);//刷子定义形状如何填满 填充后的颜色
    painter->drawConvexPolygon(pts); //这是个重载函数，绘制多边形。
    painter->restore();

    //画中心点
    QColor niceBlue(150, 150, 200);
    QConicalGradient coneGradient(0, 0, -90.0);  //角度渐变
    coneGradient.setColorAt(0.0, Qt::darkGray);
    coneGradient.setColorAt(0.2, niceBlue);
    coneGradient.setColorAt(0.5, Qt::white);
    coneGradient.setColorAt(1.0, Qt::darkGray);
    painter->setPen(Qt::NoPen);  //没有线，填满没有边界
    painter->setBrush(coneGradient);
    painter->drawEllipse(-5, -5, 10, 10);
}


void mySpeed::paintEvent(QPaintEvent *)
{
    QPainter painter (this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width()/2,height()/2);
    int side = qMin(width(),height());
    painter.scale(side /200.0,side /200.0);
    drawCrown(&painter);
    drawBackground(&painter);
    drawScaleNum(&painter);

    drawScale(&painter);							/* 画刻度线 */
    drawTitle(&painter);							/* 画单位 */

    drawNumericValue(&painter);				    	/* 画数字显示 */
    drawIndicator(&painter);

}

void mySpeed::UpdateAngle()
{
  m_value += 1;
  if(m_value > 100)
  {
     m_value = 0;
  }

  //m_angle = ((m_angle + 1) % 360);与上面几行功能一样
  update();//刷新控件，会调用paintEvent函数
}

