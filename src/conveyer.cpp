#include "conveyer.h"

Conveyer::Conveyer(QGraphicsObject *parent) : BaseObject(parent)
{
}

Conveyer::~Conveyer()
{
}

QRectF Conveyer::boundingRect() const {
    return rect;
}

void Conveyer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::gray);
    painter->drawRect(boundingRect());
}