#pragma once
#include "baseObject.h"
#include "conveyer.h"
#include <QPainter>

class Pusher : public BaseObject
{
    Q_OBJECT
private:
    double frequency;

    Conveyer* related;
public:
    explicit Pusher(QGraphicsObject *parent = nullptr);
    virtual ~Pusher();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void connection(QList<BaseObject*> objects) override;
};