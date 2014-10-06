#pragma once

#include <QFrame>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class View;
class MainWindow;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(View *v);

protected:
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *);
#endif

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    bool viewportEvent(QEvent *event);

private:
    View *view;

    qreal totalScaleFactor;
};

class View : public QFrame
{
    Q_OBJECT
public:
    explicit View(const QString &name, MainWindow* window, QWidget *parent = 0);

    QGraphicsView *view();

    void keyPressEvent(QKeyEvent *e);

    void ScrollTime();



public slots:
    void ZoomIn(int level = 1);
    void ZoomOut(int level = 1);

    void Update();

public slots:
    void ResetView();
    void SetResetButtonEnabled();
    void SetupMatrix();
    void RotateLeft();
    void RotateRight();

public:
    GraphicsView *graphicsView;
    QLabel *label;
    QLabel *label2;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;

    float wantedZoom;
    float currentZoom;

    MainWindow* mainWindow;

private:
    QTimer* timer;
};
