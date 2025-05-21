#include "scene/objects/baseObject.h"
#include "common/pch.h"
#include "scene/items/baseItem.h"
#include "scene/objects/conveyer.h"
#include "scene/scene.h"

BaseObject::BaseObject(QGraphicsObject *parent) : QGraphicsObject(parent) {
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

BaseObject::~BaseObject() {}

QRectF BaseObject::boundingRect() const {
    return QRect(0, 0, size, size);
}

Scene *BaseObject::getScene() const {
    return qobject_cast<Scene *>(scene());
}

void BaseObject::setConnection(ConnectionType type, BaseObject *object) {
    connections[type] = object;
}

BaseObject *BaseObject::getConnection(ConnectionType type) const {
    return connections.value(type, nullptr);
}

QMap<ConnectionType, BaseObject *> BaseObject::getConnections() const {
    return connections;
}

void BaseObject::clearAllConnections() {
    connections.clear();
}

void BaseObject::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        int x = std::round(pos().x() / size) * size;
        int y = std::round(pos().y() / size) * size;
        dragStartPos = QPointF(x, y);

        event->accept();
        return;
    }
}

void BaseObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
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

void BaseObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        Scene *sc = getScene();

        if (isDragging) {
            QPointF finalPos = sc->snapToGrid(event->scenePos());
            if (!sc->checkLegal(finalPos)) finalPos = sc->snapToGrid(dragStartPos);

            QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
            animation->setDuration(200);
            animation->setEasingCurve(QEasingCurve::OutCubic);
            animation->setStartValue(this->pos());
            animation->setEndValue(finalPos);
            animation->start(QAbstractAnimation::DeleteWhenStopped);

            if (dragStartPos != finalPos) {
                connect(animation, &QPropertyAnimation::finished, this, [this, sc]() {
                    connection(sc->findNeighbors(this));
                });
            }
            isDragging = false;
        }

        if (event->modifiers() & Qt::ControlModifier) {
            setSelected(!isSelected());
        } else {
            for (QGraphicsItem *item : sc->selectedItems())
                item->setSelected(false);
            setSelected(true);
        }

        setZValue(0);
        emit clicked(this);
        scene()->update();
        event->accept();
        return;
    }
}