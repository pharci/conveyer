#pragma once
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QRectF>

enum class Direction 
{
    Up,
    Down,
    Left,
    Right,
    None
}; 

enum class ObjectType
{
    None,
    Generator,
    Receiver,
    Conveyer,
    Pusher
};

class BaseObject : public QGraphicsObject 
{
    Q_OBJECT
protected:
    ObjectType type;
    QPainterPath shape = QPainterPath();
    bool highlighted = false;
public:
    explicit BaseObject(
        QGraphicsObject *parent = nullptr, 
        ObjectType type = ObjectType::None
    );
    virtual ~BaseObject();
    QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override = 0;

    ObjectType getObjectType();
    QString getObjectName();
    void setHighlighted(bool value);
    bool isHighlighted();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void connection(QList<BaseObject*> objects) = 0;

signals:
    void clicked(BaseObject *self);
};