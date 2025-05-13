#pragma once
#include "baseObject.h"
#include "conveyer.h"
#include <QPainter>

class Receiver : public BaseObject
{
    Q_OBJECT
private:
    int totalReceived = 0; 

    BaseObject* related = nullptr;
public:
    explicit Receiver(QGraphicsObject *parent = nullptr);
    virtual ~Receiver();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void setRelated(BaseObject* obj);
    BaseObject* getRelated();
    void connection(QList<BaseObject*> objects) override;
    void addItem(BaseItem* item);
};