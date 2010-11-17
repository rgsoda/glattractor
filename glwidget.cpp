#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"
#include "pointbuffer.h"


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

const float GLWidget::MAX_ZOOM;

const float GLWidget::INITIAL_A;
const float GLWidget::INITIAL_B;
const float GLWidget::INITIAL_C;
const float GLWidget::INITIAL_D;

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    zoom = 40;

    A = INITIAL_A;
    B = INITIAL_B;
    C = INITIAL_C;
    D = INITIAL_D;
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const

{
    return QSize(800, 800);
}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZoom(int _zoom) {
    zoom = qBound(0.0f, (float)_zoom, MAX_ZOOM);

    emit zoomChanged(zoom);
    updateGL();
}

void GLWidget::initializeGL()
{
    qglClearColor(QColor("black"));

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);

    glEnable(GL_POINT_SMOOTH);

    //glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_MULTISAMPLE);

    pointBuffer = QSharedPointer<PointBuffer>(
                new PointBuffer(context()));
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the camera location
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glTranslated(0.0, 0.0, -zoom);

    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);


    gluLookAt(zoom, 0., zoom,    // eye x,y,z
              0., 0., 0.,       // center x,y,z
              0., 2., 0.);      // Up direction

    glPointSize(1.0f);

    if (pointBuffer)
    {
        if (pointBuffer->size() == 0)
            fillPointBuffer();
        pointBuffer->render();
    }

    glFlush();
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble aspectratio = GLdouble(width) / GLdouble(height);

    // Set the camera parameters
    gluPerspective(90.,         // Vertical FOV degrees.
                   aspectratio, // The aspect ratio.
                   0.1,         // Near clipping 40/130
                   MAX_ZOOM + 200.0f);      // Far clipping

    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        // change attractor parameters
        float horizPos = ((float)event->x() / width()) * 8.0f - 4.0f;
        float vertPos = ((float)event->y() / height()) * 8.0f - 4.0f;

        if (event->buttons() & Qt::LeftButton) {
            setA(horizPos);
            setB(vertPos);
        }
        if (event->buttons() & Qt::RightButton) {
            setC(horizPos);
            setD(vertPos);
        }
    } else {
        // rotate attractor
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();

        if (event->buttons() & Qt::LeftButton) {
            setXRotation(xRot + 8 * dy);
            setYRotation(yRot + 8 * dx);
        } else if (event->buttons() & Qt::RightButton) {
            setXRotation(xRot + 8 * dy);
            setZRotation(zRot + 8 * dx);
        }
    }
    lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    setZoom(zoom + event->delta() / 40.0f);
}

void GLWidget::fillPointBuffer()
{
    Point newPoint, prevPoint;
    QColor staticColor = QColor::fromRgbF(1.0, 1.0, 1.0, 0.01);
    QColor slowColor = QColor::fromRgbF(0.0, 0.0, 1.0, 0.4);
    QColor fastColor = QColor::fromRgbF(1.0, 0.0, 0.0, 0.4);

    if (!pointBuffer)
        return;
    for(int iter=0;iter<100000;iter++)
    {
        newPoint.z = sin(prevPoint.x);
        newPoint.x = sin(A * prevPoint.y) - prevPoint.z * cos(B * prevPoint.x);
        newPoint.y = prevPoint.z * sin(C * prevPoint.x) - cos(D * prevPoint.y);

        constColor(newPoint, staticColor);
        //positionColor(newPoint);
        //speedColor(newPoint, prevPoint, slowColor, fastColor);
        //angleColor(newPoint, prevPoint, slowColor, fastColor);
        pointBuffer->addPoint(newPoint);

        prevPoint = newPoint;
    }
}

void GLWidget::setA(double value) {
    A = qBound(-4.0, value, 4.0);
    redrawPoints();
}
void GLWidget::setB(double value) {
    B = qBound(-4.0, value, 4.0);
    redrawPoints();
}
void GLWidget::setC(double value) {
    C = qBound(-4.0, value, 4.0);
    redrawPoints();
}
void GLWidget::setD(double value) {
    D = qBound(-4.0, value, 4.0);
    redrawPoints();
}

void GLWidget::redrawPoints() {
    if (pointBuffer)
    {
        pointBuffer->clear();
        if (pointBuffer->size() == 0) {
            fillPointBuffer();
        }
        pointBuffer->render();
    }
    updateGL();
}

void GLWidget::constColor(Point &newPoint, const QColor &staticColor) const
{
    newPoint.r = staticColor.redF();
    newPoint.g = staticColor.greenF();
    newPoint.b = staticColor.blueF();
    newPoint.a = staticColor.alphaF();
}

void GLWidget::positionColor(Point &newPoint) const
{
    newPoint.r = newPoint.x;
    newPoint.g = newPoint.y;
    newPoint.b = newPoint.z;
    newPoint.a = 0.4;
}

void GLWidget::speedColor(Point &newPoint, const Point &prevPoint,
                          const QColor &slowColor, const QColor &fastColor) const
{
    const float maxSpeed = 10.0f;

    float sub[3] = {
        newPoint.x - prevPoint.x,
        newPoint.y - prevPoint.y,
        newPoint.z - prevPoint.z
    };

    float squaredDistance = sub[0] * sub[0] + sub[1] * sub[1] + sub[2] * sub[2];
    float factor = qMin(squaredDistance, maxSpeed) / maxSpeed;

    newPoint.r = slowColor.redF() * (1 - factor) + fastColor.redF() * factor;
    newPoint.g = slowColor.greenF() * (1 - factor) + fastColor.greenF() * factor;
    newPoint.b = slowColor.blueF() * (1 - factor) + fastColor.blueF() * factor;
    newPoint.a = slowColor.alphaF() * (1 - factor) + fastColor.alphaF() * factor;
}

void GLWidget::angleColor(Point &newPoint, const Point &prevPoint,
                          const QColor &smallAngleColor,
                          const QColor &largeAngleColor) const
{
    float dotProduct =
            newPoint.x * prevPoint.x +
            newPoint.y * prevPoint.y +
            newPoint.z * prevPoint.z;
    float factor =  (dotProduct - 1.0f) / 2.0f;

    newPoint.r = smallAngleColor.redF() * (1 - factor) + largeAngleColor.redF() * factor;
    newPoint.g = smallAngleColor.greenF() * (1 - factor) + largeAngleColor.greenF() * factor;
    newPoint.b = smallAngleColor.blueF() * (1 - factor) + largeAngleColor.blueF() * factor;
    newPoint.a = smallAngleColor.alphaF() * (1 - factor) + largeAngleColor.alphaF() * factor;
}
