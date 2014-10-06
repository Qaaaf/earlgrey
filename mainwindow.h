#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QHBoxLayout;
class View;
class QTableWidget;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *e);

public:
    void PopulateScene();
    void SplitScreen();
    void UnSplitScreen();

    QGraphicsScene *scene;
    QGraphicsScene *scene2;
    QGraphicsScene *scene3;
    View* view;
    View* view2;

    QHBoxLayout* layout;

    QTableWidget* listWidget;
};
