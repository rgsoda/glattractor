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
    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    qtBlack = QColor::fromCmykF(0.0, 0.0, 0.0, 1.0);
    qtWhite = QColor::fromRgb(0,0,0);

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
    zoom = _zoom;
    zoom = qMax(zoom, 0.0f);
    zoom = qMin(zoom, MAX_ZOOM);

    emit zoomChanged(zoom);
    updateGL();
}

void GLWidget::initializeGL()
{
    qglClearColor(qtBlack);

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
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    setZoom(zoom + event->delta() / 40.0f);
}

void GLWidget::fillPointBuffer()
{
    float x,y,z, x2, y2 = 0;

    if (!pointBuffer)
        return;
    for(int iter=0;iter<100000;iter++)
    {
        x2 = sin(A * y) - z * cos(B * x);
        y2 = z * sin(C * x) - cos(D * y);
        z = sin(x);
        x = x2;
        y = y2;
        Point p(x * 10, y * 10, z * 10,
                x, y, z, 0.4f);
        pointBuffer->addPoint(p);
    }
}

void GLWidget::setA(double value) {
    A = value;
    redrawPoints();
}
void GLWidget::setB(double value) {
    B = value;
    redrawPoints();
}
void GLWidget::setC(double value) {
    C = value;
    redrawPoints();
}
void GLWidget::setD(double value) {
    D = value;
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
