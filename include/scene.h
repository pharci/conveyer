#pragma once

#include <QGraphicsScene>

class Context;
class BaseObject;
class Scene : public QGraphicsScene
{
    Q_OBJECT
private:
    QTimer* timer = nullptr;
    const int gridSize = 100;
    Context* context;
    QGraphicsRectItem *hoverRect = nullptr;
    void onTick();
public:
    explicit Scene(Context* context, QGraphicsScene *parent = nullptr);
    ~Scene();
    QVector<BaseObject*> findNeighbors(BaseObject* objCenter);
    int getGridSize() const;
    bool checkLegal(QPointF pos);
    QPointF snapToGrid(const QPointF& pos);
    void craeteNewObject(QPointF pos, QString type);
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void updateHoverRect(QPointF pos);
signals:
    void showObjectProperties(BaseObject *obj);
    void selectionCountChanged(int count);
public slots:
    void deleteSelectedObjects();
    void deleteObject(BaseObject* obj);
    void cancelSelectionObjcts();
};