#pragma once

#include <QGraphicsObject>

class BaseItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit BaseItem(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
};