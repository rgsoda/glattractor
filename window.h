
#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDoubleSpinBox>

QT_BEGIN_NAMESPACE
class QSlider;
QT_END_NAMESPACE
class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QSlider *createSlider();
    QDoubleSpinBox *createSpinBox();

    GLWidget *glWidget;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;
    QSlider *zoomSlider;

    QDoubleSpinBox *aSpin;
    QDoubleSpinBox *bSpin;
    QDoubleSpinBox *cSpin;
    QDoubleSpinBox *dSpin;

};

#endif
