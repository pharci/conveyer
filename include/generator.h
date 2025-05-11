#pragma once
#include "baseObject.h"
#include "conveyer.h"
#include <QPainter>

class Generator : public BaseObject
{
private:
    double frequency;

    Conveyer* related;
public:
    explicit Generator(QGraphicsObject *parent = nullptr);
    virtual ~Generator();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};