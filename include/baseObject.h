#pragma once
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QRectF>


enum class Orientation
{
    Up,
    Down,
    Left,
    Right
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
private:
    Orientation orientation;
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
    QRectF boundingRect() const;
    ObjectType getObjectType();
    QString getObjectName();
    void setHighlighted(bool value);
    bool isHighlighted();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    
signals:
    void clicked(BaseObject *self);
};