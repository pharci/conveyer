#pragma once
#include "baseObject.h"
#include "baseItem.h"
#include <QPainter>

class Conveyer : public BaseObject
{
    Q_OBJECT
private:
    int rotationAngle = 0;
    double speed = 1.0;
    QList<BaseItem*> items;
    BaseObject* prev = nullptr;
    BaseObject* next = nullptr;
    bool isCorner = false;
    Direction inDir = Direction::Left;
    Direction outDir = Direction::Right;
public:
    explicit Conveyer(QGraphicsObject *parent = nullptr);
    virtual ~Conveyer();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void connection(QList<BaseObject*> objects) override;

    void addItem(BaseItem* item);
    QList<BaseItem*> getItems();
    double getSpeed();

    void setNext(BaseObject* obj);
    void setPrev(BaseObject* obj);
    BaseObject* getNext();
    BaseObject* getPrev();

    Direction getInDir();
    Direction getOutDir();

    void turn();
    void moveItems();
    Direction getDirectionTo(const QPointF& from, const QPointF& to);
    void updateShape(Direction inDir, Direction outDir);
    Direction angleInDir(int angle);
    Direction angleOutDir(int angle);
};