#pragma once
#include "baseObject.h"
#include "conveyer.h"
#include <QPainter>

class Receiver : public BaseObject
{
private:
    QRectF rect = QRectF(0, 0, 50, 50);
    int totalReceived; 

    Conveyer* related;
public:
    explicit Receiver(QGraphicsObject *parent = nullptr);
    virtual ~Receiver();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};