#include "dss.h"
#include "mainwindow.h"
#include "view.h"

#include <QHBoxLayout>
#include <QtWidgets>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    view = new View("View", this);
    view2 = new View("Second Screen", this);

    PopulateScene();


    view->view()->setScene(scene);
    view2->view()->setScene(scene);

    layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);

    setWindowTitle(tr("Simple Interactive GUI Prototype"));

    listWidget = new QTableWidget;
    listWidget->insertColumn(0);
    listWidget->insertColumn(1);
    listWidget->insertColumn(2);

    QStringList list;
    list.append("Group");
    list.append("VolumePool");
    list.append("Load");

    listWidget->setHorizontalHeaderLabels(list);
    listWidget->setFixedSize(318, 400);
    listWidget->hide();
}

bool split = false;
void MainWindow::SplitScreen()
{
    split = true;
    layout->addWidget(view2);
}

void MainWindow::UnSplitScreen()
{
    split = false;
    layout->removeWidget(view2);

    delete view2;
    view2 = new View("Second Screen", this);
}


void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Space)
    {
        if(split)
            UnSplitScreen();
        else
        {
            SplitScreen();
            view2->view()->setScene(view->view()->scene());
        }
    }
}

void MainWindow::PopulateScene()
{
    scene = new QGraphicsScene;
    scene2 = new QGraphicsScene;
    scene3 = new QGraphicsScene;

    QGraphicsItem *item = new DSS(Qt::red, "DSS #1", -200, -200, view, this);
    scene->addItem(item);
    item = new DSS(Qt::green, "DSS #1", -200, -200, view, this);
    scene2->addItem(item);

    item = new DSS(Qt::green, "DSS #2", 200, 200, view, this);
    scene->addItem(item);
    item = new DSS(Qt::green, "DSS #2", 200, 200, view, this);
    scene2->addItem(item);

    item = new DSS(Qt::green, "DSS #3", -200, 200, view, this);
    scene->addItem(item);
    item = new DSS(Qt::green, "DSS #3", -200, 200, view, this);
    scene2->addItem(item);

    item = new DSS(Qt::green, "DSS #4", 200, -200, view, this);
    scene->addItem(item);
    item = new DSS(Qt::green, "DSS #4", 200, -200, view, this);
    scene2->addItem(item);

    scene->setSceneRect(-1500,-1000,3000,2000);
    scene2->setSceneRect(-1500,-1000,3000,2000);


    int count = 0;
    for(int i = -15000; i < 15000; i+=1000)
    {
        for(int j = -15000; j < 15000; j+=1000)
        {
            item = new DSS(Qt::green, "DSS #1", i+ -200, j+ -200, view, this);
            scene3->addItem(item);
            item = new DSS(Qt::green, "DSS #2", i+ 200, j+ 200, view, this);
            scene3->addItem(item);
            item = new DSS(Qt::green, "DSS #3", i+- 200, j+ 200, view, this);
            scene3->addItem(item);
            item = new DSS(Qt::green, "DSS #4", i+ 200, j+ -200, view, this);
            scene3->addItem(item);

            QGraphicsRectItem* t = new QGraphicsRectItem(i-300,j-300,800,800);
            scene3->addItem(t);

            QGraphicsTextItem* txt = new QGraphicsTextItem("Scenario " + QString::number(count));
            txt->setPos(i-300, j-300);
            txt->setScale(6);
            scene3->addItem(txt);

            count++;
        }
    }
    scene3->setSceneRect(-20000,-20000,40000,40000);
}
