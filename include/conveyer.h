#pragma once
#include "baseObject.h"
#include <QPainter>

class Conveyer : public BaseObject
{
private:
    QRectF rect = QRectF(0, 0, 50, 50);
    double speed;

    Conveyer* prev;
    Conveyer* next;
public:
    explicit Conveyer(QGraphicsObject *parent = nullptr);
    virtual ~Conveyer();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};