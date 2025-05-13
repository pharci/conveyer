#include "baseObject.h"
#include "conveyer.h"
#include "scene.h"
#include <QPainter>

BaseObject::BaseObject(QGraphicsObject *parent, ObjectType type, QRectF shape, QColor color) : 
    QGraphicsObject(parent), 
    type(type), 
    shape(shape),
    color(color)
{
}

BaseObject::~BaseObject()
{
}

QRectF BaseObject::boundingRect() const {
    return shape;
}

void BaseObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(color);
    painter->drawRect(boundingRect());

    if (highlighted) {
        QPen pen(Qt::green, 3);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
    }
}

ObjectType BaseObject::getObjectType() {
    return type;
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

void BaseObject::turn() {
    rotationAngle = (rotationAngle + 90) % 360;
    setTransformOriginPoint(boundingRect().center());
    setRotation(rotationAngle);

    if (auto* sc = dynamic_cast<Scene*>(scene())) {
        QList<BaseObject*> neighbors = sc->findNeighbors(this);
        this->connection(neighbors);
        sc->update();
    }
}

int BaseObject::getRotationAngle() {
    return rotationAngle;
}

Direction BaseObject::getDirection() const {
    switch (rotationAngle % 360) {
        case 0: return Direction::Right;
        case 90: return Direction::Down;
        case 180: return Direction::Left;
        case 270: return Direction::Up;
        default: return Direction::None;
    }
}

QPointF BaseObject::directionToOffset(Direction dir) const {
    switch (dir) {
        case Direction::Right: return QPointF(50, 0);
        case Direction::Left:  return QPointF(-50, 0);
        case Direction::Down:  return QPointF(0, 50);
        case Direction::Up:    return QPointF(0, -50);
        default: return QPointF(0, 0);
    }
}