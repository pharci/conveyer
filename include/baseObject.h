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
    int rotationAngle = 0;
    Direction direction = Direction::Right;
    QSet<QPoint> location;
    ObjectType type;
    QRectF shape;
    QColor color;
    bool highlighted = false;
public:
    explicit BaseObject(
        QGraphicsObject *parent = nullptr, 
        ObjectType type = ObjectType::None, 
        QRectF shape = QRectF(0, 0, 50, 50), 
        QColor color = Qt::gray
    );
    virtual ~BaseObject();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    ObjectType getObjectType();
    QString getObjectName();
    void setHighlighted(bool value);
    bool isHighlighted();
    void turn();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    int getRotationAngle();
    Direction getDirection() const;
    QPointF directionToOffset(Direction dir) const;

    virtual void connection(QList<BaseObject*> objects) = 0;

signals:
    void clicked(BaseObject *self);
};