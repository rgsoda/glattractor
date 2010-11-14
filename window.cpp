
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

    aSpin = createSpinBox();
    aSpin->setValue(GLWidget::INITIAL_A);
    bSpin = createSpinBox();
    bSpin->setValue(GLWidget::INITIAL_B);
    cSpin = createSpinBox();
    cSpin->setValue(GLWidget::INITIAL_C);
    dSpin = createSpinBox();
    dSpin->setValue(GLWidget::INITIAL_D);

    connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
    connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
    connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
    connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));

    connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));
    connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));

    connect(zoomSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZoom(int)));
    connect(glWidget, SIGNAL(zoomChanged(int)), zoomSlider, SLOT(setValue(int)));

    connect(aSpin, SIGNAL(valueChanged(double)),glWidget,SLOT(setA(double)));
    connect(bSpin, SIGNAL(valueChanged(double)),glWidget,SLOT(setB(double)));
    connect(cSpin, SIGNAL(valueChanged(double)),glWidget,SLOT(setC(double)));
    connect(dSpin, SIGNAL(valueChanged(double)),glWidget,SLOT(setD(double)));

    QHBoxLayout *displayLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *spinBoxGroup = new QHBoxLayout;

    displayLayout->addWidget(glWidget);
//    displayLayout->addWidget(xSlider);
//    displayLayout->addWidget(ySlider);
    displayLayout->addWidget(zSlider);
    displayLayout->addWidget(zoomSlider);

    spinBoxGroup->addWidget(aSpin);
    spinBoxGroup->addWidget(bSpin);
    spinBoxGroup->addWidget(cSpin);
    spinBoxGroup->addWidget(dSpin);

    mainLayout->addLayout(spinBoxGroup);

    mainLayout->addLayout(displayLayout);

    setLayout(mainLayout);

    xSlider->setValue(15 * 16);
    ySlider->setValue(345 * 16);
    zSlider->setValue(0 * 16);
    zoomSlider->setTickInterval(1);
    zoomSlider->setRange(0, GLWidget::MAX_ZOOM);
    zoomSlider->setValue(40);
    setWindowTitle(tr("PickOver Attractor"));
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

QDoubleSpinBox *Window::createSpinBox() {
    QDoubleSpinBox *spinbox = new QDoubleSpinBox(this);
    spinbox->setDecimals(10);
    spinbox->setSingleStep(0.1);
    spinbox->setRange(-4,4);
    return spinbox;
}
void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
