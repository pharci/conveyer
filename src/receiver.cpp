#include "receiver.h"

Receiver::Receiver(QGraphicsObject *parent) : BaseObject(parent)
{
}

Receiver::~Receiver()
{
}

QRectF Receiver::boundingRect() const {
    return rect;
}

void Receiver::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::black);
    painter->drawRect(boundingRect());
}