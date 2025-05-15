#pragma once
#include "context.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QSet>
#include <QHash>
#include <QList>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(Context* context, QGraphicsScene *parent = nullptr);
    ~Scene();
    QList<BaseObject*> findNeighbors(BaseObject* objCenter);
    int getGridSize() const;
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    bool checkLegal(QRectF rect);
    BaseObject* getObjectFromPos(QPointF pos);
    void onObjectClicked(BaseObject *obj);
    void updateHoverRect(QPointF pos);
private:
    QTimer* timer = nullptr;
    const int gridSize = 100;
    Context* context;
    QGraphicsRectItem *hoverRect = nullptr;
    void onTick();
signals:
    void objectSelected(BaseObject *obj);
};