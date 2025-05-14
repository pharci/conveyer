#include "baseItem.h"
#include "conveyer.h"
#include "receiver.h"

BaseItem::BaseItem(QGraphicsObject *parent) : QGraphicsObject(parent) {}

QRectF BaseItem::boundingRect() const {
    return QRectF(-10, -10, 20, 20);
}

void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::red);
    painter->drawEllipse(boundingRect());
}