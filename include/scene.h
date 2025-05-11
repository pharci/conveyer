#pragma once
#include "context.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QSet>

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
    BaseObject* getObjectFromPos(QPointF pos);
    void onObjectClicked(BaseObject *obj);
    void updateHoverRect(QPointF pos);

private:
    const int gridSize = 50;
    Context* context;
    QGraphicsRectItem *hoverRect = nullptr;

signals:
    void objectSelected(BaseObject *obj);
};