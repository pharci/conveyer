#include "generator.h"
#include "receiver.h"
#include "baseItem.h"
#include "scene.h"

Generator::Generator(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Generator, QRect(0, 0, 50, 50), Qt::blue) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Generator::spawnItem);
    timer->start(1000);
}

Generator::~Generator() {}

void Generator::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
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

    painter->setBrush(Qt::white);
    painter->drawEllipse(rect.center(), 15, 15);
}

void Generator::setRelated(BaseObject* obj) { 
    related = obj; 
}
BaseObject* Generator::getRelated() { return related; }

void Generator::connection(QList<BaseObject*> objects) {
    if (related) {
        if (auto* conv = qobject_cast<Conveyer*>(related)) conv->setPrev(nullptr);
        related = nullptr;
    }

    for (BaseObject* neighbor : objects) {
        if (auto* conv = qobject_cast<Conveyer*>(neighbor)) {
            bool directionMatches = false;
            switch (conv->getDirection()) {
                case Direction::Right:
                    directionMatches = this->pos().x() < conv->pos().x();
                    break;
                case Direction::Left:
                    directionMatches = this->pos().x() > conv->pos().x();
                    break;
                case Direction::Up:
                    directionMatches = this->pos().y() > conv->pos().y();
                    break;
                case Direction::Down:
                    directionMatches = this->pos().y() < conv->pos().y();
                    break;
                default:
                    return;
            }
            if (directionMatches) {
                setRelated(conv);
                conv->setPrev(this);
            }
        } 
    }
}

void Generator::spawnItem() {
    if (!related || related->getObjectType() != ObjectType::Conveyer)
        return;

    Conveyer* conveyer = static_cast<Conveyer*>(related);
    QPointF conveyerStart = conveyer->pos();
    auto* item = new BaseItem();

    switch (conveyer->getDirection()) {
        case Direction::Right:
            item->setPos(conveyerStart.x(), conveyerStart.y() + conveyer->boundingRect().height() / 2 + item->boundingRect().height() / 2);
            break;
        case Direction::Left:
            item->setPos(conveyerStart.x() + conveyer->boundingRect().width(), 
                         conveyerStart.y() + conveyer->boundingRect().height() / 2 + item->boundingRect().height() / 2);
            break;
        case Direction::Up:
            item->setPos(conveyerStart.x() + conveyer->boundingRect().width() / 2,
                         conveyerStart.y() + conveyer->boundingRect().height());
            break;
        case Direction::Down:
            item->setPos(conveyerStart.x() + conveyer->boundingRect().width() / 2, conveyerStart.y());
            break;
        default:
            return;
    }

    conveyer->addItem(item);
    item->setConveyer(conveyer);

    auto* sc = dynamic_cast<Scene*>(scene());
    sc->addItem(item);
    sc->update();
}