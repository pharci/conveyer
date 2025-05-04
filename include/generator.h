#pragma once
#include "baseObject.h"
#include "conveyer.h"
#include <QPainter>

class Generator : public BaseObject
{
private:
    QRectF rect = QRectF(0, 0, 50, 50);
    double frequency;

    Conveyer* related;
public:
    explicit Generator(QGraphicsObject *parent = nullptr);
    virtual ~Generator();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};