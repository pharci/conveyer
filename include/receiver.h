#pragma once
#include "baseObject.h"
#include "conveyer.h"
#include <QPainter>

class Receiver : public BaseObject
{
private:
    int totalReceived; 

    Conveyer* related;
public:
    explicit Receiver(QGraphicsObject *parent = nullptr);
    virtual ~Receiver();
};