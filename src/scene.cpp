#include "scene.h"
#include "receiver.h"
#include "generator.h"
#include "conveyer.h"
#include <QGraphicsView>
#include <QMessageBox>
#include <QTimer>

Scene::Scene(Context* context, QGraphicsScene *parent) : QGraphicsScene(parent), context(context)
{
    hoverRect = addRect(0, 0, gridSize, gridSize, QPen(Qt::green), QBrush(QColor(100, 100, 255, 50)));
    hoverRect->setZValue(100);
    hoverRect->setVisible(false);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Scene::onTick);
    timer->start(30);
}

Scene::~Scene() {}

void Scene::onTick() {
    for (auto* item : items()) {
        if (auto* conv = dynamic_cast<Conveyer *>(item)) {
            conv->moveItems();
        }
    }
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
        QPointF pos = event->scenePos();
        double x = std::floor(pos.x() / gridSize) * gridSize;
        double y = std::floor(pos.y() / gridSize) * gridSize;
        QPointF location = QPointF(x, y);

        if (context->getCurrentObjectType() != ObjectType::None) {
            auto *obj = context->createCurrentObject();
            obj->setPos(location);
    
            if (checkLegal(obj->sceneBoundingRect())) {
                connect(obj, &BaseObject::clicked, this, &Scene::onObjectClicked);
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
    if (!obj->isHighlighted()) {
        obj->setHighlighted(true);
        context->addSelected(obj);
    } else {
        obj->setHighlighted(false);
        context->removeSelected(obj);
    }
    emit objectSelected(obj);
    update();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos();
    updateHoverRect(pos);
    QGraphicsScene::mouseMoveEvent(event);
}


void Scene::updateHoverRect(QPointF pos) {
    double x = std::floor(pos.x() / gridSize) * gridSize;
    double y = std::floor(pos.y() / gridSize) * gridSize;

    hoverRect->setRect(x, y, gridSize, gridSize);

    if (checkLegal(hoverRect->sceneBoundingRect().adjusted(+0.5, +0.5, -0.5, -0.5))) {
        hoverRect->setPen(QPen(Qt::green));
    } else {
        hoverRect->setPen(QPen(Qt::red));
    }
    hoverRect->setVisible(true);
}

bool Scene::checkLegal(QRectF rect) {
    for (QGraphicsItem *item : this->items()) {
        if (auto *base = dynamic_cast<BaseObject *>(item)) {
            if (item->sceneBoundingRect().intersects(rect)) {
                return false;
            }
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