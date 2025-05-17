#pragma once

#include <QGraphicsObject>

class Scene;
class BaseItem : public QGraphicsObject
{
    Q_OBJECT
private:
    QPointF dragStartPos;
    bool isDragging = false;
    Scene* getScene() const;
    const int size = 10;
public:
    explicit BaseItem(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};