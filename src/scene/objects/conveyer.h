#pragma once

#include "baseObject.h"

class BaseItem;
class Conveyer : public BaseObject {
    Q_OBJECT
  private:
    int rotationAngle = 0;
    int speed = 1;
    bool isCorner = false;
    Direction inDir = Direction::Left;
    Direction outDir = Direction::Right;
    QPointF startPoint, endPoint, centerPoint;
    int patternOffset = 0;
    QTimer *patternTimer;

  public:
    explicit Conveyer(QGraphicsObject *parent = nullptr);
    virtual ~Conveyer();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QPainterPath shape();
    void connection(QList<BaseObject *> objects) override;
    QWidget *createPropertiesWidget(QWidget *parent) override;

    Direction getInDir();
    Direction getOutDir();

    QPointF getStartPoint();
    QPointF getCenterPoint();

    void turn();
    void start();
    void stop();

    void advance(int step) override;
    void affect(BaseItem *item);
    QList<QGraphicsItem *> nearbyItems();
};