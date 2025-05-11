#include "receiver.h"

Receiver::Receiver(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Receiver, QRect(0, 0, 50, 50), Qt::black) {}

Receiver::~Receiver() {}

void Receiver::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    painter->setBrush(color);
    painter->drawRect(rect);

    if (highlighted) {
        QPen pen(Qt::green, 3);
        painter->setPen(pen);
        painter->drawRect(rect);
    }

    painter->setBrush(Qt::green);
    painter->drawEllipse(rect.center(), 15, 15);
}