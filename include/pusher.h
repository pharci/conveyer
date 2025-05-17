#pragma once

#include "baseObject.h"

class Pusher : public BaseObject
{
    Q_OBJECT
private:
    double frequency;
    BaseObject* related;
public:
    explicit Pusher(QGraphicsObject *parent = nullptr);
    virtual ~Pusher();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void connection(QList<BaseObject*> objects) override;
    QWidget* createPropertiesWidget(QWidget* parent) override;
};