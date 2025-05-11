#include "conveyer.h"

Conveyer::Conveyer(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Conveyer, QRect(0, 5, 50, 40), Qt::gray) {}

Conveyer::~Conveyer() {}

void Conveyer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    painter->setBrush(color);
    painter->drawRect(rect);

    if (highlighted) {
        QPen pen(Qt::green, 3);
        painter->setPen(pen);
        painter->drawRect(rect);
    }

    QPen linePen(Qt::lightGray, 2);
    painter->setPen(linePen);

    int lineCount = 3;
    qreal spacing = rect.width() / (lineCount + 1);

    for (int i = 1; i <= lineCount; ++i) {
        qreal x = rect.left() + i * spacing;
        painter->drawLine(QPointF(x, rect.top() + 5), QPointF(x, rect.bottom() - 5));
    }
}