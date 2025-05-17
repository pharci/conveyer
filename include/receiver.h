#pragma once

#include "baseObject.h"

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
    void acceptItem();
    QWidget* createPropertiesWidget(QWidget* parent) override;
};