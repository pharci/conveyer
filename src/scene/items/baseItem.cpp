#include "baseItem.h"
#include "common/pch.h"
#include "scene/objects/conveyer.h"
#include "scene/objects/receiver.h"
#include "scene/scene.h"

BaseItem::BaseItem(QGraphicsObject *parent) : QGraphicsObject(parent) {
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    setZValue(10);
}

QRectF BaseItem::boundingRect() const {
    return QRectF(-size, -size, size * 2, size * 2);
}

void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(isSelected() ? Qt::green : Qt::red);
    painter->drawEllipse(boundingRect());
}

void BaseItem::advance(int step) {
    if (!step) return;
    moveBy(velocity.x(), velocity.y());
    checkCurrentSurface();

    if (velocity.isNull() && !onConveyer) {
        if (!isIdleTimerRunning) {
            idleTimer.start();
            isIdleTimerRunning = true;
        } else {
            if (idleTimer.elapsed() > 5000) {
                if (scene()) scene()->removeItem(this);
                deleteLater();
            }
        }
    } else {
        isIdleTimerRunning = false;
    }
}

void BaseItem::checkCurrentSurface() {
    QList<QGraphicsItem *> colliding = collidingItems();

    onConveyer = false;
    for (auto *item : colliding) {
        Conveyer *conv = dynamic_cast<Conveyer *>(item);
        if (conv) {
            onConveyer = true;
            break;
        }
    }

    if (!onConveyer) {
        setVelocity(QPointF(0, 0));
    }
}

void BaseItem::setVelocity(QPointF p) {
    velocity = p;
}

QPointF BaseItem::getVelocity() {
    return velocity;
}