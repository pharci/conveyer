#include "baseObject.h"
#include "conveyer.h"
#include "scene.h"
#include <QPainter>

BaseObject::BaseObject(QGraphicsObject *parent, ObjectType type) : QGraphicsObject(parent), type(type) {}

BaseObject::~BaseObject() {}

QRectF BaseObject::boundingRect() const { 
    return QRect(0, 0, 100, 100); 
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

void BaseObject::setHighlighted(bool value) {
    highlighted = value;
}

bool BaseObject::isHighlighted() { return highlighted; }

void BaseObject::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked(this);
    }
    QGraphicsObject::mousePressEvent(event);
}