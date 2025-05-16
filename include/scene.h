#pragma once

#include <QGraphicsScene>

class Context;
class BaseObject;
class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(Context* context, QGraphicsScene *parent = nullptr);
    ~Scene();
    QVector<BaseObject*> findNeighbors(BaseObject* objCenter);
    int getGridSize() const;
    bool checkLegal(QPointF pos);
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
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
    void objectClicked(BaseObject *obj);
    void selectionCountChanged(int count);
public slots:
    void deleteObjects();
};