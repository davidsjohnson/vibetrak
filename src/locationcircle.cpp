#include "locationcircle.h"

LocationCircle::LocationCircle(int width, int height, QColor color) :
    m_width(width), m_height(height), m_boundingBox(0, 0, width, height), m_color(color)
{
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QRectF LocationCircle::boundingRect() const
{
    return m_boundingBox;
}


void LocationCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(m_color));
    painter->setBrush(QBrush(m_color));
    painter->drawEllipse(0,0, m_width, m_height);
}


void LocationCircle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    setCursor(Qt::ClosedHandCursor);
}


void LocationCircle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    setCursor(Qt::OpenHandCursor);
}


void LocationCircle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}
