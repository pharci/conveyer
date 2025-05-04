#pragma once
#include <QGraphicsObject>
#include <QPoint>
#include <QRectF>

enum class Orientation
{
    Up,
    Down,
    Left,
    Right
};

class BaseObject : public QGraphicsObject 
{
    Q_OBJECT
private:
    QRectF rect = QRectF(0, 0, 100, 100);
    Orientation orientation;

    QSet<QPoint> location;
public:
    explicit BaseObject(QGraphicsObject *parent = nullptr);
    virtual ~BaseObject();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};