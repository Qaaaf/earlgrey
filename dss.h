#pragma once

#include <QColor>
#include <QGraphicsItem>

class View;
class MainWindow;
class QTableWidgetItem;

class DSS : public QGraphicsItem
{
public:

    View *view;
    MainWindow* mainWindow;


    DSS(const QColor &color, QString name, int x, int y, View *view, MainWindow* window);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    QColor health;

    QString name;


};

class HDDRank : public QGraphicsItem
{
public:
    HDDRank(QString name, int x, int y, int load, View *view, MainWindow* window);

    QRectF boundingRect() const;
    QPainterPath shape() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);


    int load;
    View *view;
    MainWindow* mainWindow;

    QTableWidgetItem* item;

    QString name;
};
