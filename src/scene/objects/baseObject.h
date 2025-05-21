#pragma once

#include "common/enums.h"
#include <QGraphicsObject>

class Scene;
class BaseObject : public QGraphicsObject {
    Q_OBJECT
  protected:
    int size = 100;
    QPointF dragStartPos;
    bool isDragging = false;
    QMap<ConnectionType, BaseObject *> connections;

  public:
    explicit BaseObject(QGraphicsObject *parent = nullptr);
    virtual ~BaseObject();
    QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override = 0;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void setConnection(ConnectionType type, BaseObject *object);
    BaseObject *getConnection(ConnectionType type) const;
    QMap<ConnectionType, BaseObject *> getConnections() const;
    void clearAllConnections();

    Scene *getScene() const;
    virtual void connection(QList<BaseObject *> objects) = 0;
    virtual QWidget *createPropertiesWidget(QWidget *parent) = 0;
  signals:
    void clicked(BaseObject *self);
};