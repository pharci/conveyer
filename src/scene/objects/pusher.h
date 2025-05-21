#pragma once

#include "baseObject.h"

class Pusher : public BaseObject {
    Q_OBJECT
  private:
    double frequency;
    int rotationAngle = 0;
    QTimer *timer;
    qreal pistonOffset = 0;
    qreal speed = 1;

  public:
    explicit Pusher(QGraphicsObject *parent = nullptr);
    virtual ~Pusher();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void push();

    int getRotationAngle();
    QWidget *createPropertiesWidget(QWidget *parent) override;
    void connection(QList<BaseObject *> objects) override;
    void turn();
};