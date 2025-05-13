#include "conveyer.h"
#include "generator.h"
#include "receiver.h"
#include "scene.h"

Conveyer::Conveyer(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Conveyer, QRect(0, 5, 50, 40), Qt::gray) {}

Conveyer::~Conveyer() {}

void Conveyer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    if (prev != nullptr && next != nullptr) {
        painter->setBrush(color);
    } else {
        painter->setBrush(Qt::red);
    }

    painter->drawRect(rect);

    if (highlighted) {
        QPen pen(Qt::green, 3);
        painter->setPen(pen);
        painter->drawRect(rect);
    }

    QPen linePen(Qt::white, 2);
    painter->setPen(linePen);

    QPointF start(rect.left() + 10, rect.center().y());
    QPointF end(rect.right() - 10, rect.center().y());

    QPainterPath path;
    path.moveTo(start);
    path.lineTo(end);
    path.lineTo(end + QPointF(-10, -10 / 2));
    path.moveTo(end);
    path.lineTo(end + QPointF(-10, 10 / 2));

    painter->drawPath(path);
}

void Conveyer::connection(QList<BaseObject*> objects) {
    if (next) {
        if (auto* conv = qobject_cast<Conveyer*>(next)) conv->setPrev(nullptr);
        if (auto* conv = qobject_cast<Receiver*>(next)) conv->setRelated(nullptr);
        next = nullptr;
    }
    if (prev) {
        if (auto* conv = qobject_cast<Conveyer*>(prev)) conv->setNext(nullptr);
        if (auto* conv = qobject_cast<Generator*>(prev)) conv->setRelated(nullptr);
        prev = nullptr;
    }

    Direction myDir = getDirection();
    QPointF nextPos = pos() + directionToOffset(myDir);
    QPointF prevPos = pos() - directionToOffset(myDir);

    for (BaseObject* neighbor : objects) {
        if (auto* conv = qobject_cast<Conveyer*>(neighbor)) {
            if (conv->pos() == nextPos && conv->getDirection() == myDir) {
                setNext(conv);
                conv->setPrev(this);
            } else if (conv->pos() == prevPos && conv->getDirection() == myDir) {
                setPrev(conv);
                conv->setNext(this);
            }
        } else if (auto* gen = qobject_cast<Generator*>(neighbor)) {
            if (gen->pos() == prevPos && gen->getRelated() == nullptr) {
                gen->setRelated(this);
                setPrev(gen);
            }
        } else if (auto* recv = qobject_cast<Receiver*>(neighbor)) {
            if (recv->pos() == nextPos && recv->getRelated() == nullptr) {
                recv->setRelated(this);
                setNext(recv);
            }
        }
    }
}

void Conveyer::setNext(BaseObject* obj) { next = obj; }
void Conveyer::setPrev(BaseObject* obj) { prev = obj; }
BaseObject* Conveyer::getNext() { return next; }
BaseObject* Conveyer::getPrev() { return prev; }
void Conveyer::removeItem(BaseItem* item) { items.removeOne(item); }
void Conveyer::addItem(BaseItem* item) { items.push_back(item); }
QList<BaseItem*> Conveyer::getItems() { return items; }
double Conveyer::getSpeed() { return speed; }