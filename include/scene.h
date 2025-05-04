#pragma once
#include "context.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(Context* context, QGraphicsScene *parent = nullptr);
    ~Scene();
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    bool checkLegal(QRectF rect);
private:
    Context* context;
    QGraphicsRectItem *hoverRect = nullptr;
    const int gridSize = 50;
};