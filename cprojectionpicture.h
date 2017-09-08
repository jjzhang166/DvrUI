#ifndef CPROJECTIONPICTURE_H
#define CPROJECTIONPICTURE_H

#include <QWidget>

class QGestureEvent;
class QPanGesture;
class QPinchGesture;
class QSwipeGesture;
class CProjectionPicture;

class CProjectionPicture : public QWidget
{
    Q_OBJECT
public:
    CProjectionPicture(QWidget *parent = 0);

    void setPicture(QImage & image);

protected:
    // 放大/缩小
//    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
//    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;


//    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
//    // 平移
//    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
//    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
//    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;


public Q_SLOTS:
    void zoomIn();  // 放大
    void zoomOut();  // 缩小
    void zoom(float scale); // 缩放 - scaleFactor：缩放的比例因子
    void translate(QPointF delta);  // 平移


private:
    bool gestureEvent(QGestureEvent *event);
    void panTriggered(QPanGesture*);
    void pinchTriggered(QPinchGesture*);

    QImage loadImage(const QString &fileName);

    QImage currentImage;

    qreal horizontalOffset;
    qreal verticalOffset;

    qreal scaleFactor;
    qreal currentStepScaleFactor;
    Qt::MouseButton m_translateButton;  // 平移按钮
    bool m_bMouseTranslate;
    qreal m_zoomDelta;  // 缩放的增量
    QPoint m_lastMousePos;  // 鼠标最后按下的位置


};

#endif // CPROJECTIONPICTURE_H
