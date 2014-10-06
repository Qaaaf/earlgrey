#include "dss.h"

#include <QtWidgets>

#include "view.h"
#include "mainwindow.h"

#include "QGraphicsScene"


#include "QGraphicsWidget"

#include "QTableWidget"
#include "QListWidgetItem"


DSS::DSS(const QColor &color, QString name, int x, int y, View* view, MainWindow *window)
{
    this->health = color;
    this->name = name;
    mainWindow = window;
    setPos(x, y);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

    this->view = view;
}

QRectF DSS::boundingRect() const
{
    return QRectF(0, 0, 140, 200);
}

QPainterPath DSS::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 140, 200);
    return path;
}

void DSS::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor fillColor = (option->state & QStyle::State_Selected) ? health.dark(150) : health;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if (lod < 2)
    {
        painter->fillRect(QRectF(0, 0, 140, 200), fillColor);


        if(lod > 0.5)
        {
            QFont font("Times", 10);
            font.setStyleStrategy(QFont::ForceOutline);
            painter->setFont(font);
            painter->save();
            painter->drawText(40, 180, name);
            painter->restore();
        }


        return;
    }

    if ( lod >= 2)
    {
        (health == Qt::red)? painter->fillRect(QRectF(0, 0, 70, 100),Qt::red) : painter->drawRect(QRectF(0, 0, 70, 100));
        painter->drawRect(QRectF(70, 0, 70, 100));
        painter->drawRect(QRectF(0, 100, 70, 100));
        painter->drawRect(QRectF(70, 100, 70, 100));


        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->drawText(144, 16, name + " Type: XP7");

        painter->drawText(18, 55,  " HDD");
        painter->drawText(88, 55, " HDD");
        painter->drawText(18, 155, " FMD");
        painter->drawText(88, 155,  " SSD");

        painter->restore();
    }
}

void DSS::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);

    update();
}

void DSS::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene* scene = new QGraphicsScene;

    for(int i = 0; i < 25; i++)
        for(int j = 0; j < 25; j++)
        {
            QString id = "HDD #" + QString::number(i + i*j);
            HDDRank* item = new HDDRank(id, i*120, j*120, (health == Qt::red)? i*4 : 30, view, mainWindow);
            scene->addItem(item);
        }

    if(name == "DSS #1")
    {
        QGraphicsRectItem* t = new QGraphicsRectItem(120*16-20,-20,120*9,120*25+20);
        scene->addItem(t);
        t = new QGraphicsRectItem(-20,-20,120*16,120*25+20);
        scene->addItem(t);

        QGraphicsTextItem* txt = new QGraphicsTextItem("VolumePool 0");
        txt->setPos(120*16-30, -35*5);
        txt->setScale(8);
        scene->addItem(txt);
        txt = new QGraphicsTextItem("VolumePool 1");
        txt->setPos(-30, -35*5);
        txt->setScale(8);
        scene->addItem(txt);
    }
    else
    {
        QGraphicsTextItem* txt = new QGraphicsTextItem("VolumePool 1");
        txt->setPos(-30, -35*5);
        txt->setScale(8);
        scene->addItem(txt);

        QGraphicsRectItem* t = new QGraphicsRectItem(-20,-20,120*25+20,120*25+20);
        scene->addItem(t);
    }


    scene->setSceneRect(-1500,-1500,8000,8000);

    mainWindow->view2->view()->setScene(scene);
    mainWindow->SplitScreen();

    update();
}

void DSS::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
    update();
}

void DSS::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
        view->view()->centerOn(this);
    else
        QGraphicsItem::mouseReleaseEvent(event);

    update();
}



HDDRank::HDDRank(QString name, int x, int y, int load, View *view, MainWindow* window)
{
    this->name = name;
    mainWindow = window;
    setPos(x, y);

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);

    this->view = view;

    this->load = load;
}

QRectF HDDRank::boundingRect() const
{
    return QRectF(0, 0, 60, 100);
}

QPainterPath HDDRank::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, 60, 100);
    return path;
}

void HDDRank::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
}

QVariant HDDRank::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
    {
        if (value == true)
        {
            int row = mainWindow->listWidget->rowCount();
            mainWindow->listWidget->insertRow(row);

            item = new QTableWidgetItem(name);
            mainWindow->listWidget->setItem(row, 0, item);
            QTableWidgetItem* item2 = new QTableWidgetItem((load >= 60)?"VolumePool 0" : "VolumePool 1");
            mainWindow->listWidget->setItem(row, 1, item2);
            QTableWidgetItem* item3 = new QTableWidgetItem(QString::number(load));
            mainWindow->listWidget->setItem(row, 2, item3);

            mainWindow->listWidget->show();
            mainWindow->listWidget->activateWindow();
            // do stuff if selected
        }
        else
        {
            mainWindow->listWidget->removeRow(mainWindow->listWidget->row(item));
            if(mainWindow->listWidget->rowCount() == 0)
                mainWindow->listWidget->hide();
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void HDDRank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QColor c = (load > 60)? Qt::red : Qt::green;

    c = (option->state & QStyle::State_Selected) ? c.dark(150) : c;
    if (option->state & QStyle::State_MouseOver)
        c = c.light(125);

    painter->drawRect(QRectF(0, 0, 60, 100));
    painter->fillRect(QRectF(0, 100-load, 60, load), c);
}

