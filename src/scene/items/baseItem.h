#pragma once

#include <QElapsedTimer>
#include <QGraphicsObject>

class BaseObject;
class Scene;
class BaseItem : public QGraphicsObject {
    Q_OBJECT
  private:
    const int size = 10;
    bool onConveyer = false;
    QPointF velocity;
    QElapsedTimer idleTimer;
    bool isIdleTimerRunning = false;

  public:
    explicit BaseItem(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void advance(int step) override;
    void checkCurrentSurface();
    void setVelocity(QPointF p);
    QPointF getVelocity();
};