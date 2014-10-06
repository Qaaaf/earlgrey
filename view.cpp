#include "view.h"

#include "mainwindow.h"

#include <QtWidgets>
#include <qmath.h>

#include <QTouchEvent>
#include <QScrollBar>

GraphicsView::GraphicsView(View *v) : QGraphicsView(), view(v)
{
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    setDragMode(ScrollHandDrag);
}

#ifndef QT_NO_WHEELEVENT
void GraphicsView::wheelEvent(QWheelEvent *e)
{

    if (e->delta() > 0)
        view->ZoomIn(12);
    else
        view->ZoomOut(12);

    e->accept();
}
#endif

View::View(const QString &name, MainWindow* window, QWidget *parent)
    : QFrame(parent)
{
    mainWindow = window;

    setFrameStyle(Sunken | StyledPanel);
    graphicsView = new GraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/zoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/zoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    QToolButton *rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QPixmap(":/rotateleft.png"));
    rotateLeftIcon->setIconSize(iconSize);
    QToolButton *rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QPixmap(":/rotateright.png"));
    rotateRightIcon->setIconSize(iconSize);
    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
    rotateSlider->setTickPosition(QSlider::TicksBelow);

    // Rotate slider layout
    QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);

    resetButton = new QToolButton;
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);

    QGridLayout *topLayout = new QGridLayout;
    topLayout->addWidget(graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addLayout(rotateSliderLayout, 2, 0);
    topLayout->addWidget(resetButton, 2, 1);
    setLayout(topLayout);

    connect(resetButton, SIGNAL(clicked()), this, SLOT(ResetView()));
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(SetupMatrix()));
    connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(SetupMatrix()));
    connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(SetResetButtonEnabled()));
    connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(SetResetButtonEnabled()));
    connect(rotateLeftIcon, SIGNAL(clicked()), this, SLOT(RotateLeft()));
    connect(rotateRightIcon, SIGNAL(clicked()), this, SLOT(RotateRight()));
    connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(ZoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(ZoomOut()));

    SetupMatrix();

    wantedZoom = 0.0;
    currentZoom = zoomSlider->value();

    timer = new QTimer;
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(Update()));
    timer->setInterval(10);
    //timer->start();
}

QGraphicsView *View::view()
{
    return static_cast<QGraphicsView *>(graphicsView);
}

void View::Update()
{
    float dt = wantedZoom * 0.15;

    wantedZoom -= dt;
    currentZoom += dt;

    zoomSlider->setValue(currentZoom);

    if(wantedZoom > -2 && wantedZoom < 2)
    {
        wantedZoom = 0;
        timer->stop();
    }
}

void View::ResetView()
{
    zoomSlider->setValue(250);
    rotateSlider->setValue(0);
    SetupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    resetButton->setEnabled(false);
}

void View::SetResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void View::SetupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;
    matrix.scale(scale, scale);
    //matrix.rotate(rotateSlider->value());

    graphicsView->setMatrix(matrix);
    SetResetButtonEnabled();
}

void View::ScrollTime()
{
    switch(rotateSlider->value())
    {
    case 0:
        view()->setScene(mainWindow->scene);
        break;
    case 40:
        view()->setScene(mainWindow->scene3);
        break;
    default:
        view()->setScene(mainWindow->scene2);
    }
}

void View::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_A)
    {
        ZoomIn(12);
    }
    else if(e->key() == Qt::Key_Z)
    {
        ZoomOut(12);
    }
    else
        QFrame::keyPressEvent(e);
}

void View::ZoomIn(int level)
{
    if(wantedZoom == 0)
    {
        currentZoom = zoomSlider->value();
        timer->start();
    }

    wantedZoom += level;
}

void View::ZoomOut(int level)
{
    if(wantedZoom == 0)
    {
        currentZoom = zoomSlider->value();
        timer->start();
    }

    wantedZoom -= level;
}

void View::RotateLeft()
{
    rotateSlider->setValue(rotateSlider->value() - 10);
    ScrollTime();
}

void View::RotateRight()
{
    rotateSlider->setValue(rotateSlider->value() + 10);
    ScrollTime();
}

void GraphicsView::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::MiddleButton)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
        setInteractive(false);

        QMouseEvent fake(e->type(), e->pos(), Qt::LeftButton, Qt::LeftButton, e->modifiers());
        QGraphicsView::mousePressEvent(&fake);
    }
    else
        QGraphicsView::mousePressEvent(e);

    update();
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::MiddleButton)
    {
        QMouseEvent fake(e->type(), e->pos(), Qt::LeftButton, Qt::LeftButton, e->modifiers());
        QGraphicsView::mouseReleaseEvent(&fake);

        setDragMode(QGraphicsView::RubberBandDrag);
        setInteractive(true);
    }
    else
        QGraphicsView::mouseReleaseEvent(e);

    update();
}



bool GraphicsView::viewportEvent(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
        if (touchPoints.count() == 2)
        {
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
            qreal currentScaleFactor =
                    QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                    / QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();

            qreal scalar = (currentScaleFactor-totalScaleFactor) * 200.0;
            if(scalar < 8.0 && scalar > -8.0)
                view->ZoomIn(scalar);


            qDebug() << "Zooming: " << QString::number(scalar) << endl;

            qDebug() << "wantedZoom: " << QString::number(view->wantedZoom) << endl;

            totalScaleFactor = currentScaleFactor;
        }
        else
            break;
        //return true;
    }
    default:
        break;
    }
    return QGraphicsView::viewportEvent(event);
}
