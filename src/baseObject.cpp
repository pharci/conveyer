#include "pch.h"
#include "baseObject.h"
#include "scene.h"
#include "conveyer.h"

BaseObject::BaseObject(QGraphicsObject *parent, ObjectType type) : QGraphicsObject(parent), type(type) {
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

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
        Scene* sc = getScene();

        if (isDragging) {
            QPointF finalPos = sc->snapToGrid(event->scenePos());
            if (!sc->checkLegal(finalPos)) finalPos = sc->snapToGrid(dragStartPos);

            QPropertyAnimation* animation = new QPropertyAnimation(this, "pos");
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
            setZValue(0);
        }

        if (event->modifiers() & Qt::ControlModifier) {
            setSelected(!isSelected());
        } else {
            for (QGraphicsItem *item : sc->selectedItems())
                item->setSelected(false);
            setSelected(true);
            
        }

        emit clicked(this);
        scene()->update();
        event->accept();
        return;
    }
}