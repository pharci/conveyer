#pragma once

#include <QGraphicsScene>

class BaseObject;
class Scene : public QGraphicsScene {
    Q_OBJECT
  private:
    QTimer *timer = nullptr;
    const int gridSize = 100;
    QGraphicsRectItem *hoverRect = nullptr;
    void onTick();
    bool isRightButtonDown = false;

  public:
    explicit Scene(QGraphicsScene *parent = nullptr);
    ~Scene();
    QVector<BaseObject *> findNeighbors(BaseObject *objCenter);
    int getGridSize() const;
    bool checkLegal(QPointF pos);
    BaseObject *getObjFromPos(QPointF pos);
    QPointF snapToGrid(const QPointF &pos);
    void craeteNewObject(QPointF pos, QString type);

  protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void updateHoverRect(QPointF pos);
  signals:
    void showObjectProperties(BaseObject *obj);
  public slots:
    void deleteSelectedObjects();
    void deleteObject(BaseObject *obj);
    void cancelSelectionObjcts();
};