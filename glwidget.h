
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QSharedPointer>
#include <QGLWidget>
#include "pointbuffer.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setZoom(int zoom);
    void setA(int value);
    void setB(int value);
    void setC(int value);
    void setD(int value);

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

private:
    void fillPointBuffer();

    int xRot;
    int yRot;
    int zRot;
    float zoom;
    float A;
    float B;
    float C;
    float D;
    QPoint lastPos;
    QColor qtGreen;
    QColor qtPurple;
    QColor qtWhite;
    QColor qtBlack;
    QSharedPointer<PointBuffer> pointBuffer;
};

#endif
