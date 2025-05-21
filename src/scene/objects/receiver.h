#pragma once

#include "baseObject.h"

class Receiver : public BaseObject {
    Q_OBJECT
  private:
    int totalReceived = 0;

  public:
    explicit Receiver(QGraphicsObject *parent = nullptr);
    virtual ~Receiver();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void connection(QList<BaseObject *> objects) override;
    void advance(int step) override;
    QWidget *createPropertiesWidget(QWidget *parent) override;
};