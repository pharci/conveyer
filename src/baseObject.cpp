#include "pch.h"
#include "baseObject.h"
#include "scene.h"
#include "conveyer.h"

BaseObject::BaseObject(QGraphicsObject *parent, ObjectType type) : QGraphicsObject(parent), type(type) {}

BaseObject::~BaseObject() {}

QRectF BaseObject::boundingRect() const { 
    return QRect(0, 0, size, size); 
}

Scene* BaseObject::getScene() const {
    return qobject_cast<Scene*>(scene());
}

QString BaseObject::getObjectName() {
    switch (type) {
        case ObjectType::Generator: return "Generator";
        case ObjectType::Receiver: return "Receiver";
        case ObjectType::Conveyer: return "Conveyer";
        case ObjectType::Pusher: return "Pusher";
        case ObjectType::None: return "None";
        default: return "Unknown";
    }
}

ObjectType BaseObject::getObjectType() {
    return type;
}

void BaseObject::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragStartPos = pos();
        event->accept();
        return;
    }
}

void BaseObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        drugAndDrop = true;
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
        Scene* sc = getScene();

        if (drugAndDrop) {
            QPointF pos = event->scenePos();
            qreal x = std::floor(pos.x() / size) * size;
            qreal y = std::floor(pos.y() / size) * size;    

            if (sc->checkLegal(QPointF(x, y))) { setPos(QPointF(x, y));} 
            else {
                qreal x = std::floor(dragStartPos.x() / size) * size;
                qreal y = std::floor(dragStartPos.y() / size) * size;      
                setPos(QPointF(x, y));
            }

            drugAndDrop = false;
            setZValue(0);
            connection(sc->findNeighbors(this));
        }

        if (event->modifiers() & Qt::ControlModifier) {
            setSelected(!isSelected());
        } else {
            for (QGraphicsItem *item : sc->selectedItems())
                item->setSelected(false);
            setSelected(true);
            
        }

        emit clicked(this);
        event->accept();
        return;
    }
}

void BaseObject::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    qreal x = std::floor(dragStartPos.x() / size) * size;
    qreal y = std::floor(dragStartPos.y() / size) * size;      
    setPos(QPointF(x, y));
    QGraphicsObject::hoverLeaveEvent(event);
}