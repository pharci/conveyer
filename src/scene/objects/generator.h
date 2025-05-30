#pragma once

#include "baseObject.h"

class Generator : public BaseObject {
    Q_OBJECT
  private:
    qreal frequency = 1000;
    QTimer *timer = nullptr;
    QTimer *updateTimer = nullptr;
    BaseObject *related = nullptr;

  public:
    explicit Generator(QGraphicsObject *parent = nullptr);
    virtual ~Generator();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void connection(QList<BaseObject *> objects) override;
    void spawnItem();
    QWidget *createPropertiesWidget(QWidget *parent) override;
    void start();
    void stop();
};