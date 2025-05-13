#include "baseItem.h"
#include "conveyer.h"
#include "receiver.h"

BaseItem::BaseItem(QGraphicsObject *parent)
    : QGraphicsObject(parent), currentConveyer(nullptr)
{
    setZValue(1);
}

QRectF BaseItem::boundingRect() const {
    return QRectF(-5, -5, 10, 10);
}

void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(Qt::red);
    painter->drawEllipse(boundingRect());
}

void BaseItem::updatePos() {
    if (!currentConveyer) return;

    double speed = currentConveyer->getSpeed();
    QPointF offset = QPointF(0, 0);

    bool moveToNext = false;

    switch (currentConveyer->getDirection()) {
        case Direction::Right:
            if (pos().x() - boundingRect().x() < currentConveyer->pos().x() + currentConveyer->boundingRect().width()) {
                offset = QPointF(speed, 0);
            }
            else { moveToNext = true; }
            break;
        case Direction::Left:
            if (pos().x() + boundingRect().x() > currentConveyer->pos().x()) {
                offset = QPointF(-speed, 0);
            }
            else { moveToNext = true; }
            break;
        case Direction::Up:
            if (pos().y() + boundingRect().y() > currentConveyer->pos().y()) {
                offset = QPointF(0, -speed);
            }
            else { moveToNext = true; }
            break;
        case Direction::Down:
            if (pos().y() + boundingRect().y() < currentConveyer->pos().y() + currentConveyer->boundingRect().height()) {
                offset = QPointF(0, speed);
            }
            else { moveToNext = true; }
            break;
        default:
            return;
    }

    if (moveToNext) {
        BaseObject* obj = currentConveyer->getNext();
        if (auto* conv = qobject_cast<Conveyer*>(obj)) {
            currentConveyer->removeItem(this);
            currentConveyer = conv;
            conv->addItem(this);
        }
        if (auto* recv = qobject_cast<Receiver*>(obj)) {
            currentConveyer->removeItem(this);
            recv->addItem(this);
        }
    }

    moveBy(offset.x(), offset.y());
}

void BaseItem::setConveyer(Conveyer* c) {
    currentConveyer = c;
}

Conveyer* BaseItem::getConveyer() const {
    return currentConveyer;
}
