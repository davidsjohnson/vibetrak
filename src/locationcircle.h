#ifndef LOCATIONCIRCLE_H
#define LOCATIONCIRCLE_H

#include <iostream>

#include <QGraphicsItem>
#include <Qt>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QStyleOptionGraphicsItem>

class LocationCircle : public QGraphicsItem
{
public:
    LocationCircle(int width = 10, int height =10, QColor color = QColor(255, 0, 0));

    QRectF boundingRect() const;

    qreal width() {return m_boundingBox.width();}
    qreal height() {return m_boundingBox.height();}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


private:

    int m_width;
    int m_height;
    QRectF m_boundingBox;
    QColor m_color;

};

#endif // LOCATIONCIRCLE_H
