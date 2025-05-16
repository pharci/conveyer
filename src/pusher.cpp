#include "pch.h"
#include "pusher.h"

Pusher::Pusher(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Pusher) {}

Pusher::~Pusher() {}

void Pusher::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    if (isSelected()) { painter->setPen(QPen(Qt::green, 3)); }
    painter->drawRect(rect);

    QPen linePen(Qt::black, 2);
    painter->setPen(linePen);

    if (related != nullptr) { painter->setBrush(Qt::cyan); } 
    else { painter->setBrush(Qt::red); }
    painter->drawRect(QRect(0, 0, rect.width(), 10));
    painter->drawRect(QRect(15, 10, 20, 40));
}

void Pusher::connection(QList<BaseObject*> objects) {
}