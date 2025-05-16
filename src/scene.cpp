#include "pch.h"

#include "scene.h"
#include "receiver.h"
#include "generator.h"
#include "conveyer.h"
#include "context.h"

Scene::Scene(Context* context, QGraphicsScene *parent) : QGraphicsScene(parent), context(context)
{
    hoverRect = addRect(0, 0, gridSize, gridSize, QPen(Qt::green), QBrush(QColor(100, 100, 255, 50)));
    hoverRect->setZValue(100);
    hoverRect->setVisible(false);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Scene::onTick);
    timer->start(16);

    connect(this, &QGraphicsScene::selectionChanged, this, [this]() {
        emit selectionCountChanged(selectedItems().size());
    });
}

Scene::~Scene() {}

void Scene::onTick() {
    for (auto* item : items()) {
        if (auto* conv = dynamic_cast<Conveyer *>(item)) {
            conv->moveItems();
        }
    }
}

int Scene::getGridSize() const {
    return gridSize;
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect) {
    QPen pen(QColor(200, 200, 200));
    pen.setWidth(1);
    painter->setPen(pen);

    for (qreal x = std::floor(rect.left() / gridSize) * gridSize; x < rect.right(); x += gridSize)
        painter->drawLine(QLineF(x, rect.top(), x, rect.bottom()));

    for (qreal y = std::floor(rect.top() / gridSize) * gridSize; y < rect.bottom(); y += gridSize)
        painter->drawLine(QLineF(rect.left(), y, rect.right(), y));
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (context->getCurrentObjectType() != ObjectType::None) {
            QPointF pos = event->scenePos();
            double x = std::floor(pos.x() / gridSize) * gridSize;
            double y = std::floor(pos.y() / gridSize) * gridSize;    
            if (checkLegal(QPointF(x, y))) {
                auto *obj = context->createCurrentObject();
                obj->setPos(QPointF(x, y));
                connect(obj, &BaseObject::clicked, this, &Scene::onObjectClicked);
                obj->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable); //
                addItem(obj);
                updateHoverRect(pos);

                QList<BaseObject*> neighbors = findNeighbors(obj);
                obj->connection(neighbors);
                update();
                return;
            }
        } 
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::onObjectClicked(BaseObject *obj) {
    emit objectClicked(obj);
    update();
}


void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    updateHoverRect(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::updateHoverRect(QPointF pos) {
    qreal x = std::floor(pos.x() / gridSize) * gridSize;
    qreal y = std::floor(pos.y() / gridSize) * gridSize;
    hoverRect->setRect(x, y, gridSize, gridSize);
    if (checkLegal(QPointF(x, y))) { hoverRect->setPen(QPen(Qt::green)); } 
    else { hoverRect->setPen(QPen(Qt::red)); }
    hoverRect->setVisible(true);
}

bool Scene::checkLegal(QPointF pos) {
    for (QGraphicsItem *item : this->items()) {
        if (item->pos() == pos) {
            return false;
        }
    }
    return true;
}

BaseObject* Scene::getObjectFromPos(QPointF pos) {
    for (QGraphicsItem *item : this->items()) {
        if (BaseObject *obj = dynamic_cast<BaseObject *>(item)) {
            if (obj->pos() == pos) {
                return obj;
            }
        }
    }
    return nullptr;
}

QList<BaseObject*> Scene::findNeighbors(BaseObject* objCenter) {
    QList<QPointF> poses = {QPointF(0, -gridSize), QPointF(0, gridSize), QPointF(-gridSize, 0), QPointF(gridSize, 0)};
    QList<BaseObject*> neighbors;
    for (QPointF pos : poses) {
        BaseObject *obj = getObjectFromPos(objCenter->pos() + pos);
        if (obj) {
            neighbors.append(obj);
        }
    }
    return neighbors;
}



void Scene::deleteObjects() {
    for (auto* item : selectedItems()) {
        BaseObject* obj = dynamic_cast<BaseObject*>(item);
        
        QList<BaseObject*> neighbors = findNeighbors(obj);
        for (auto neighbor : neighbors) {
            if (auto* conv = qobject_cast<Conveyer*>(neighbor)) {
                if (conv->getNext() == obj) conv->setNext(nullptr);
                if (conv->getPrev() == obj) conv->setPrev(nullptr);
            }
            if (auto* gen = qobject_cast<Generator*>(neighbor)) {
                if (gen->getRelated() == obj) gen->setRelated(nullptr);
            }
            if (auto* recv = qobject_cast<Receiver*>(neighbor)) {
                if (recv->getRelated() == obj) recv->setRelated(nullptr);
            }
        }

        obj->deleteLater();
        update();
    }
}