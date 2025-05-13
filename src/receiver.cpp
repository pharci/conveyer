#include "receiver.h"
#include "generator.h"
#include "scene.h"

Receiver::Receiver(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Receiver, QRect(0, 0, 50, 50), Qt::black) {}

Receiver::~Receiver() {}

void Receiver::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    if (related != nullptr) {
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
            bool directionMatches = false;
            switch (conv->getDirection()) {
                case Direction::Right:
                    directionMatches = this->pos().x() > conv->pos().x();
                    break;
                case Direction::Left:
                    directionMatches = this->pos().x() < conv->pos().x();
                    break;
                case Direction::Up:
                    directionMatches = this->pos().y() < conv->pos().y();
                    break;
                case Direction::Down:
                    directionMatches = this->pos().y() > conv->pos().y();
                    break;
                default:
                    return;
            }
            if (directionMatches) {
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
