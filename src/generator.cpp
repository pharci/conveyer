#include "generator.h"

Generator::Generator(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Generator, QRect(0, 0, 50, 50), Qt::blue) {}

Generator::~Generator() {}

void Generator::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    painter->setBrush(color);
    painter->drawRect(rect);

    if (highlighted) {
        QPen pen(Qt::green, 3);
        painter->setPen(pen);
        painter->drawRect(rect);
    }

    painter->setBrush(Qt::white);
    painter->drawEllipse(rect.center(), 15, 15);
}