#pragma once

#include "enums.h"
#include <QGraphicsObject>

class Scene;
class BaseObject : public QGraphicsObject 
{
    Q_OBJECT
protected:
    ObjectType type;
    int size = 100;
    QPointF dragStartPos;
    bool isDragging = false;
public:
    explicit BaseObject(
        QGraphicsObject *parent = nullptr, 
        ObjectType type = ObjectType::None
    );
    virtual ~BaseObject();
    QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override = 0;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    ObjectType getObjectType();
    QString getObjectName();
    Scene* getScene() const;
    virtual void connection(QList<BaseObject*> objects) = 0;
    virtual QWidget* createPropertiesWidget(QWidget* parent) = 0;
signals:
    void clicked(BaseObject *self);
};