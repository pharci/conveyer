#include "pch.h"
#include "baseItem.h"
#include "conveyer.h"
#include "receiver.h"
#include "scene.h"

BaseItem::BaseItem(QGraphicsObject *parent) : QGraphicsObject(parent) {
    setZValue(10);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

QRectF BaseItem::boundingRect() const {
    return QRectF(-size, -size, size * 2, size * 2);
}

Scene* BaseItem::getScene() const {
    return qobject_cast<Scene*>(scene());
}

void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(isSelected() ? Qt::green : Qt::red);
    painter->drawEllipse(boundingRect());
}

void BaseItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        dragStartPos = pos();
        event->accept();
        return;
    }
}

void BaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        isDragging = true;
        setZValue(10);
        QPointF newScenePos = event->scenePos();
        QPointF delta = newScenePos - event->buttonDownScenePos(Qt::LeftButton);
        setPos(dragStartPos + delta);

        event->accept();
        return;
    }
}

void BaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        Scene* sc = getScene();

        if (isDragging) {
            QPointF pos = event->scenePos();
            QPropertyAnimation* animation = new QPropertyAnimation(this, "pos");
            animation->setDuration(200);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(this->pos());
            animation->setEndValue(pos);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
            isDragging = false;
            setZValue(0);
        }

        if (event->modifiers() & Qt::ControlModifier) {
            setSelected(!isSelected());
        } else {
            for (QGraphicsItem *item : sc->selectedItems())
                item->setSelected(false);
            setSelected(true);
            
        }
        scene()->update();
        event->accept();
        return;
    }
}