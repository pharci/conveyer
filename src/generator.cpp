#include "generator.h"

Generator::Generator(QGraphicsObject *parent) : BaseObject(parent)
{
}

Generator::~Generator()
{
}

QRectF Generator::boundingRect() const {
    return rect;
}

void Generator::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::red);
    painter->drawRect(boundingRect());
}