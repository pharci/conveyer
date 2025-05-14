#include "pusher.h"

Pusher::Pusher(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Pusher) {
    shape.addRect(QRect(0, 0, 50, 50));
}

Pusher::~Pusher() {}

void Pusher::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    painter->drawRect(rect);

    if (highlighted) {
        QPen pen(Qt::green, 3);
        painter->setPen(pen);
        painter->drawRect(rect);
    }

    QPen linePen(Qt::red, 2);
    painter->setPen(linePen);

    painter->setBrush(Qt::black);
    painter->drawRect(QRect(0, 0, 50, 10));
    painter->drawRect(QRect(15, 10, 20, 40));
}

void Pusher::connection(QList<BaseObject*> objects) {
}