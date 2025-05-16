#include "pch.h"
#include "generator.h"
#include "conveyer.h"
#include "baseItem.h"

Generator::Generator(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Generator) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Generator::spawnItem);
    timer->start(1000);
}

Generator::~Generator() {}

void Generator::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    if ( related != nullptr) { painter->setBrush(Qt::blue); } 
    else { painter->setBrush(Qt::red); }
    if (isSelected()) { painter->setPen(QPen(Qt::green, 3));}
    painter->drawRect(rect);

    painter->setBrush(Qt::white);
    painter->drawEllipse(rect.center(), rect.width() / 2 - 10, rect.width() / 2 - 10);
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
    item->setPos(conveyer->getStartPoint());
    conveyer->addItem(item);
    item->setParentItem(conveyer);
}