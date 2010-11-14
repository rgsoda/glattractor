
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QSharedPointer>
#include <QGLWidget>
#include "pointbuffer.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    const static float MAX_ZOOM = 200.0f;

    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setZoom(int zoom);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void zoomChanged(int zoom);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void fillPointBuffer();

    int xRot;
    int yRot;
    int zRot;
    float zoom;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;
    QColor qtWhite;
    QColor qtBlack;
    QSharedPointer<PointBuffer> pointBuffer;
};

#endif
