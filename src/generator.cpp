#include "generator.h"
#include "receiver.h"
#include "baseItem.h"
#include "scene.h"

Generator::Generator(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Generator) {
    shape.addRect(QRect(0, 0, 50, 50));
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Generator::spawnItem);
    timer->start(1000);
}

Generator::~Generator() {}

void Generator::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    if (related != nullptr) {
        painter->setBrush(Qt::blue);
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
            Direction dir = conv->getDirectionTo(conv->pos(), this->pos());
            if (conv->getInDir() == dir) {
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
    auto* item = new BaseItem();

    item->setPos(QPointF(0, conveyer->boundingRect().width() / 2));

    conveyer->addItem(item);

    auto* sc = dynamic_cast<Scene*>(scene());
    item->setParentItem(conveyer);
    sc->update();
}