#pragma once
#include "baseObject.h"
#include "baseItem.h"
#include <QPainter>

class Conveyer : public BaseObject
{
    Q_OBJECT
private:
    double speed = 1.0;
    QList<BaseItem*> items;
    BaseObject* prev = nullptr;
    BaseObject* next = nullptr;
public:
    explicit Conveyer(QGraphicsObject *parent = nullptr);
    virtual ~Conveyer();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void connection(QList<BaseObject*> objects) override;
    void setNext(BaseObject* obj);
    void setPrev(BaseObject* obj);
    void addItem(BaseItem* item);
    QList<BaseItem*> getItems();
    void removeItem(BaseItem* item);
    double getSpeed();
    BaseObject* getNext();
    BaseObject* getPrev();
};