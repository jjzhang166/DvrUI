#include "cprojectionpicture.h"
#include <QEvent>
#include <QPainter>
#include <QGesture>
#include <QPointF>
#include <QPinchGesture>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
CProjectionPicture::CProjectionPicture(QWidget *parent)
    : QWidget(parent),
    horizontalOffset(0),
    verticalOffset(0),
    scaleFactor(1),
    currentStepScaleFactor(1),
    m_translateButton(Qt::LeftButton),
    m_bMouseTranslate(false),
    m_zoomDelta(0.2)
{
    this->setFocusPolicy(Qt::ClickFocus);
    grabGesture(Qt::PanGesture);//平移手势
    grabGesture(Qt::PinchGesture);//缩放手势
    grabGesture(Qt::SwipeGesture);//滑动手势
}
void CProjectionPicture::setPicture(QImage &image)
{
    currentImage = image.convertToFormat(QImage::Format_RGB888);
    update();
}

bool CProjectionPicture::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

void CProjectionPicture::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QImage image = currentImage;


    if(!image.isNull()){
        image = image.scaled(this->width()*currentStepScaleFactor * scaleFactor,
                             this->height()*currentStepScaleFactor * scaleFactor,
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
    }
    const qreal iw = image.width();
    const qreal ih = image.height();
    const qreal wh = height();
    const qreal ww = width();

    painter.translate(ww/2, wh/2);
    painter.translate(horizontalOffset, verticalOffset);
    //painter.scale(currentStepScaleFactor * scaleFactor, currentStepScaleFactor * scaleFactor);
    painter.translate(-iw/2, -ih/2);
    painter.drawImage(0,0,image);
}
//还原
//void CProjectionPicture::mouseDoubleClickEvent(QMouseEvent *)
//{
//    scaleFactor = 1;
//    currentStepScaleFactor = 1;
//    verticalOffset = 0;
//    horizontalOffset = 0;
//    update();
//}

bool CProjectionPicture::gestureEvent(QGestureEvent *event)
{
    if (QGesture *pan = event->gesture(Qt::PanGesture))
        panTriggered(static_cast<QPanGesture *>(pan));
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void CProjectionPicture::panTriggered(QPanGesture *gesture)
{
#ifndef QT_NO_CURSOR
    switch (gesture->state()) {
        case Qt::GestureStarted:
        case Qt::GestureUpdated:
            setCursor(Qt::SizeAllCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
    }
#endif
    QPointF delta = gesture->delta();
    horizontalOffset += delta.x();
    verticalOffset += delta.y();
    update();
}

void CProjectionPicture::pinchTriggered(QPinchGesture *gesture)
{
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::ScaleFactorChanged) {
        currentStepScaleFactor = gesture->totalScaleFactor();
    }
    if (gesture->state() == Qt::GestureFinished) {
        scaleFactor *= currentStepScaleFactor;
        currentStepScaleFactor = 1;
    }
    update();
}

void CProjectionPicture::resizeEvent(QResizeEvent*e)
{
    update();
    QWidget::resizeEvent(e);
}

//// 上/下/左/右键向各个方向移动、加/减键进行缩放
//void CProjectionPicture::keyPressEvent(QKeyEvent *event)
//{
//    switch (event->key()) {
//    qDebug() << event->key();
//    case Qt::Key_Up:
//        translate(QPointF(0, -5));  // 上移
//        break;
//    case Qt::Key_Down:
//        translate(QPointF(0, 5));  // 下移
//        break;
//    case Qt::Key_Left:
//        translate(QPointF(-5, 0));  // 左移
//        break;
//    case Qt::Key_Right:
//        translate(QPointF(5, 0));  // 右移
//        break;
//    case Qt::Key_Plus:  // 放大
//        zoomIn();
//        break;
//    case Qt::Key_Minus:  // 缩小
//        zoomOut();
//        break;
//    default:
//        QWidget::keyPressEvent(event);
//    }
//    QWidget::keyPressEvent(event);
//}

//// 平移
//void CProjectionPicture::mouseMoveEvent(QMouseEvent *event)
//{
//    if (m_bMouseTranslate){
//        QPointF mouseDelta = event->pos() - m_lastMousePos;
//        translate(mouseDelta);
//    }

//    m_lastMousePos = event->pos();

//    QWidget::mouseMoveEvent(event);
//}

//void CProjectionPicture::mousePressEvent(QMouseEvent *event)
//{
//    qDebug() << "CProjectionPicture::mousePressEvent";
//    if (event->button() == m_translateButton) {
//        m_bMouseTranslate = true;
//        m_lastMousePos = event->pos();
//        setCursor(Qt::OpenHandCursor);
//    }

//    QWidget::mousePressEvent(event);
//}

//void CProjectionPicture::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (event->button() == m_translateButton)
//    {
//        m_bMouseTranslate = false;
//        setCursor(Qt::ArrowCursor);
//    }

//    QWidget::mouseReleaseEvent(event);
//}

//// 放大/缩小
//void CProjectionPicture::wheelEvent(QWheelEvent *event)
//{
//    qDebug() << "CProjectionPicture::wheelEvent";
////    QPoint numPixels = event->pixelDelta();
//    QPoint scrallAmount = event->angleDelta();
//    if(scrallAmount.y() > 0){
//        zoomIn();
//    }
//    else if(scrallAmount.y() < 0){
//        zoomOut();
//    }
//    QWidget::wheelEvent(event);
//}

// 放大
void CProjectionPicture::zoomIn()
{
    zoom(1 + m_zoomDelta);
}

// 缩小
void CProjectionPicture::zoomOut()
{
    zoom(1 - m_zoomDelta);
}

// 缩放 - scaleFactor：缩放的比例因子
void CProjectionPicture::zoom(float scale)
{
    scaleFactor *= scale;
    update();
}

// 平移
void CProjectionPicture::translate(QPointF delta)
{
    horizontalOffset += delta.x();
    verticalOffset += delta.y();
    update();
}
