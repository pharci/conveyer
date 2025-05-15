#include "receiver.h"
#include "generator.h"
#include "scene.h"

Receiver::Receiver(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Receiver) {}

Receiver::~Receiver() {}

void Receiver::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    if (related != nullptr) { painter->setBrush(Qt::black); } 
    else { painter->setBrush(Qt::red); }
    if (highlighted) { painter->setPen(QPen(Qt::green, 3)); }
    painter->drawRect(rect);

    painter->setBrush(Qt::green);
    painter->drawEllipse(rect.center(), 15, 15);

    QString countText = QString::number(totalReceived);
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 12));
    painter->drawText(rect, Qt::AlignCenter, countText);
}

void Receiver::setRelated(BaseObject* obj) { related = obj; }

BaseObject* Receiver::getRelated() { return related; }

void Receiver::connection(QList<BaseObject*> objects) {
    if (related) {
        if (auto* conv = qobject_cast<Conveyer*>(related)) conv->setPrev(nullptr);
        related = nullptr;
    }

    for (BaseObject* neighbor : objects) {
        if (auto* conv = qobject_cast<Conveyer*>(neighbor)) {
            Direction dir = conv->getDirectionTo(conv->pos(), this->pos());
            if (conv->getOutDir() == dir) {
                setRelated(conv);
                conv->setNext(this);
            }
        } 
    }
}

void Receiver::addItem(BaseItem* item) {
    totalReceived++;

    auto* sc = dynamic_cast<Scene*>(scene());
    item->deleteLater();
}
