#include "scene.h"
#include "common/pch.h"
#include "interface/view.h"
#include "items/baseItem.h"
#include "objects/conveyer.h"
#include "objects/generator.h"
#include "objects/pusher.h"
#include "objects/receiver.h"

Scene::Scene(QGraphicsScene *parent) : QGraphicsScene(parent) {
    setSceneRect(0, 0, 3000, 3000);

    hoverRect = addRect(0, 0, gridSize, gridSize, QPen(Qt::green), QBrush(QColor(100, 100, 255, 50)));
    hoverRect->setZValue(100);
    hoverRect->setVisible(false);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Scene::onTick);
    timer->start(16);
}

void Scene::onTick() {
    advance();
}

Scene::~Scene() {}

int Scene::getGridSize() const { return gridSize; }

void Scene::drawBackground(QPainter *painter, const QRectF &rect) {
    QPen gridPen(Qt::lightGray);
    gridPen.setWidth(1);
    painter->setPen(gridPen);

    qreal left = std::floor(rect.left() / gridSize) * gridSize;
    qreal right = std::ceil(rect.right() / gridSize) * gridSize;
    qreal top = std::floor(rect.top() / gridSize) * gridSize;
    qreal bottom = std::ceil(rect.bottom() / gridSize) * gridSize;
    for (qreal x = left; x <= right; x += gridSize) {
        painter->drawLine(QLineF(x, top, x, bottom));
    }
    for (qreal y = top; y <= bottom; y += gridSize) {
        painter->drawLine(QLineF(left, y, right, y));
    }
}

void Scene::craeteNewObject(QPointF pos, QString type) {
    if (checkLegal(snapToGrid(pos))) {
        BaseObject *obj;
        if (type == "Generator")
            obj = new Generator;
        else if (type == "Receiver")
            obj = new Receiver;
        else if (type == "Conveyer")
            obj = new Conveyer;
        else if (type == "Pusher")
            obj = new Pusher;
        else
            obj = nullptr;

        if (obj) {
            obj->setPos(snapToGrid(pos));
            connect(obj, &BaseObject::clicked, this, [this, obj]() { emit showObjectProperties(obj); });

            addItem(obj);
            updateHoverRect(pos);
            obj->setSelected(true);

            QList<BaseObject *> neighbors = findNeighbors(obj);
            obj->connection(neighbors);
            update();

            QTimer::singleShot(0, this, [this, obj]() { emit showObjectProperties(obj); });
        }
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (!views().isEmpty()) {
        if (auto view = qobject_cast<View *>(views().first())) {
            if (!view->isPanningActive()) {
                updateHoverRect(event->scenePos());
            }
        }
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (!(event->button() == Qt::RightButton)) emit showObjectProperties(nullptr);
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {};
    QGraphicsScene::mousePressEvent(event);
}

void Scene::updateHoverRect(QPointF pos) {
    hoverRect->setRect(QRectF(snapToGrid(pos), snapToGrid(pos) + QPointF(gridSize, gridSize)));
    hoverRect->setPen(QPen(checkLegal(snapToGrid(pos)) ? Qt::green : Qt::red));

    hoverRect->setVisible(true);
}

bool Scene::checkLegal(QPointF pos) {
    for (QGraphicsItem *item : this->items()) {
        if (item->pos() == pos) return false;
    }
    return true;
}

BaseObject *Scene::getObjFromPos(QPointF pos) {
    for (QGraphicsItem *item : this->items()) {
        if (item->pos() == pos) {
            if (auto *obj = dynamic_cast<BaseObject *>(item)) return obj;
        }
    }
    return nullptr;
}

QList<BaseObject *> Scene::findNeighbors(BaseObject *obj) {
    if (!obj) return {};
    QPointF pos = obj->pos();
    QList<QPointF> poses = {
        pos + QPointF(0, -gridSize),
        pos + QPointF(0, gridSize),
        pos + QPointF(-gridSize, 0),
        pos + QPointF(gridSize, 0)};

    QList<BaseObject *> neighbors;
    neighbors.reserve(4);

    for (const QPointF &neighborPos : poses) {
        BaseObject *neighbor = getObjFromPos(neighborPos);
        if (neighbor) neighbors.append(neighbor);
    }
    return neighbors;
}

void Scene::deleteObject(BaseObject *obj) {
    if (!obj) return;
    for (auto [type, neighbor] : obj->getConnections().toStdMap()) {
        if (!neighbor) continue;
        for (auto [otherType, otherObj] : neighbor->getConnections().toStdMap()) {
            if (otherObj == obj) {
                neighbor->setConnection(otherType, nullptr);
                break;
            }
        }
    }
    obj->deleteLater();
}

void Scene::deleteSelectedObjects() {
    auto selected = selectedItems();
    for (auto *item : selected)
        if (auto *obj = dynamic_cast<BaseObject *>(item)) deleteObject(obj);
    emit showObjectProperties(nullptr);
}

void Scene::cancelSelectionObjcts() {
    for (QGraphicsItem *item : selectedItems())
        item->setSelected(false);
}

void Scene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete)
        deleteSelectedObjects();
    else if (event->key() == Qt::Key_Escape)
        cancelSelectionObjcts();
    emit showObjectProperties(nullptr);
}

QPointF Scene::snapToGrid(const QPointF &pos) {
    int x = static_cast<int>(std::floor(pos.x() / gridSize));
    int y = static_cast<int>(std::floor(pos.y() / gridSize));
    return QPointF(x * gridSize, y * gridSize);
}