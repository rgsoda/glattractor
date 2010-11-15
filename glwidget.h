
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
    const static float INITIAL_A = -2.6605426906608045f;
    const static float INITIAL_B = -0.3278694022446871f;
    const static float INITIAL_C = 2.8367380360141397f;
    const static float INITIAL_D = -2.35758491512388f;

    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setZoom(int zoom);
    void setA(double value);
    void setB(double value);
    void setC(double value);
    void setD(double value);

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
    void redrawPoints();
    void positionColor(Point &newPoint) const;
    void speedColor(Point &newPoint, const Point &prevPoint,
                    const QColor &slowColor, const QColor &fastColor) const;
    void angleColor(Point &newPoint, const Point &prevPoint,
                    const QColor &smallAngleColor,
                    const QColor &largeAngleColor) const;

    int xRot;
    int yRot;
    int zRot;
    float zoom;
    float A;
    float B;
    float C;
    float D;
    QPoint lastPos;
    QSharedPointer<PointBuffer> pointBuffer;
};

#endif
