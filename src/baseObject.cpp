#include "baseObject.h"
#include <QPainter>

BaseObject::BaseObject(QGraphicsObject *parent) : QGraphicsObject(parent)
{
}

BaseObject::~BaseObject()
{
}

QRectF BaseObject::boundingRect() const {
    return rect;
}

void BaseObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::gray);
    painter->drawRect(boundingRect());
}