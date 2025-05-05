#pragma once
#include "baseObject.h"
#include "conveyer.h"
#include <QPainter>

class Pusher : public BaseObject
{
private:
    double frequency;

    Conveyer* related;
public:
    explicit Pusher(QGraphicsObject *parent = nullptr);
    virtual ~Pusher();
};