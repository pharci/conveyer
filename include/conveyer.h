#pragma once
#include "baseObject.h"
#include <QPainter>

class Conveyer : public BaseObject
{
private:
    double speed;

    Conveyer* prev;
    Conveyer* next;
public:
    explicit Conveyer(QGraphicsObject *parent = nullptr);
    virtual ~Conveyer();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};