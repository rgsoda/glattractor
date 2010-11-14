
#include <QtGui>

#include "glwidget.h"
#include "window.h"

Window::Window()
{
    glWidget = new GLWidget;

    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();
    zoomSlider = createSlider();

    aSlider = createSlider();
    bSlider = createSlider();
    cSlider = createSlider();
    dSlider = createSlider();

    aSlider->setRange(-10000,10000);
    bSlider->setRange(-2,2);
    cSlider->setRange(-2,2);
    dSlider->setRange(-2,2);


    connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
    connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
    connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
    connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));

    connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));
    connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));

    connect(zoomSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZoom(int)));
    connect(glWidget, SIGNAL(zoomChanged(int)), zoomSlider, SLOT(setValue(int)));

    connect(aSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setA(int)));
    //connect(glWidget, SIGNAL(aChanged(int)), aSlider, SLOT(setValue(int)));


    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
//    mainLayout->addWidget(xSlider);
//    mainLayout->addWidget(ySlider);
    mainLayout->addWidget(zSlider);
    mainLayout->addWidget(zoomSlider);
    mainLayout->addWidget(aSlider);
    setLayout(mainLayout);

    xSlider->setValue(15 * 16);
    ySlider->setValue(345 * 16);
    zSlider->setValue(0 * 16);
    zoomSlider->setTickInterval(1);
    zoomSlider->setRange(-100,100);
    zoomSlider->setValue(40);
    setWindowTitle(tr("Attractor"));
}


QSlider *Window::createSlider()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(1);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(1 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
