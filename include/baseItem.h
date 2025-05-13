#pragma once

#include <QGraphicsObject>

class Conveyer;

class BaseItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit BaseItem(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    void updatePos();

    void setConveyer(Conveyer* c);
    Conveyer* getConveyer() const;

private:
    Conveyer* currentConveyer = nullptr;
};