#include "pch.h"

#include "scene.h"
#include "receiver.h"
#include "generator.h"
#include "pusher.h"
#include "conveyer.h"
#include "context.h"
#include "baseItem.h"

Scene::Scene(Context *context, QGraphicsScene *parent) : QGraphicsScene(parent), context(context)
{
    setSceneRect(0, 0, 3000, 3000);

    hoverRect = addRect(0, 0, gridSize, gridSize, QPen(Qt::green), QBrush(QColor(100, 100, 255, 50)));
    hoverRect->setZValue(100);
    hoverRect->setVisible(false);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Scene::onTick);
    timer->start(16);

    connect(this, &QGraphicsScene::selectionChanged, this, [this]()
            { emit selectionCountChanged(selectedItems().size()); });
}

Scene::~Scene() {}

void Scene::onTick()
{
    for (auto *item : items())
    {
        if (auto *conv = dynamic_cast<Conveyer *>(item))
        {
            conv->moveItems();
        }
    }
    update();
}

int Scene::getGridSize() const { return gridSize; }

void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QPen gridPen(Qt::lightGray);
    gridPen.setWidth(1);
    painter->setPen(gridPen);

    qreal left   = std::floor(rect.left() / gridSize) * gridSize;
    qreal right  = std::ceil(rect.right() / gridSize) * gridSize;
    qreal top    = std::floor(rect.top() / gridSize) * gridSize;
    qreal bottom = std::ceil(rect.bottom() / gridSize) * gridSize;

    // Вертикальные линии
    for (qreal x = left; x <= right; x += gridSize) {
        painter->drawLine(QLineF(x, top, x, bottom));
    }

    // Горизонтальные линии
    for (qreal y = top; y <= bottom; y += gridSize) {
        painter->drawLine(QLineF(left, y, right, y));
    }
}

void Scene::craeteNewObject(QPointF pos, QString type) {
    if (checkLegal(snapToGrid(pos))) {
        BaseObject* obj;
        if (type == "Generator") {
            obj = new Generator;
        } else if (type == "Receiver") {
            obj = new Receiver;
        } else if (type == "Conveyer") {
            obj = new Conveyer;
        } else if (type == "Pusher") {
            obj = new Pusher;
        } else {
            obj = nullptr;
        }

        if (obj) {
            obj->setPos(snapToGrid(pos));
            connect(obj, &BaseObject::clicked, this, [this, obj]() { emit showObjectProperties(obj); });

            addItem(obj);
            updateHoverRect(pos);
            obj->setSelected(true);

            QList<BaseObject *> neighbors = findNeighbors(obj);
            obj->connection(neighbors);
            update();

            QTimer::singleShot(0, this, [this, obj]() {
                emit showObjectProperties(obj);
                emit context->setCurrentObjectType(static_cast<int>(ObjectType::None));
            });
        }
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->modifiers() & Qt::AltModifier))
    {
        updateHoverRect(event->scenePos());
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::updateHoverRect(QPointF pos)
{
    hoverRect->setRect(QRectF(snapToGrid(pos), snapToGrid(pos) + QPointF(gridSize, gridSize)));
    if (checkLegal(snapToGrid(pos)))
    {
        hoverRect->setPen(QPen(Qt::green));
    }
    else
    {
        hoverRect->setPen(QPen(Qt::red));
    }
    hoverRect->setVisible(true);
}

bool Scene::checkLegal(QPointF pos)
{
    for (QGraphicsItem *item : this->items())
    {
        if (item->pos() == pos)
        {
            return false;
        }
    }
    return true;
}

QList<BaseObject *> Scene::findNeighbors(BaseObject *objCenter)
{
    if (!objCenter)
        return {};

    QPointF centerPos = objCenter->pos();
    QList<QPointF> nPoses = {
        centerPos + QPointF(0, -gridSize),
        centerPos + QPointF(0, gridSize),
        centerPos + QPointF(-gridSize, 0),
        centerPos + QPointF(gridSize, 0)};

    QList<BaseObject *> neighbors;

    for (QPointF npos : nPoses)
    {
        for (QGraphicsItem *item : this->items())
        {
            if (auto *baseObj = dynamic_cast<BaseObject *>(item))
            {
                if (baseObj->pos() == npos)
                {
                    neighbors.append(baseObj);
                    break;
                }
            }
        }
    }
    return neighbors;
}

void Scene::deleteSelectedObjects()
{
    auto selected = selectedItems();
    for (auto *item : selected)
    {
        if (auto *obj = dynamic_cast<BaseObject *>(item))
            deleteObject(obj);
        else if (auto *baseItem = dynamic_cast<BaseItem *>(item))
            baseItem->deleteLater();
    }
    update();
}


void Scene::deleteObject(BaseObject* obj)
{
    if (!obj) return;

    if (auto *conveyer = qobject_cast<Conveyer *>(obj)) {
        conveyer->clearItems();
    }

    for (auto neighbor : findNeighbors(obj)) {
        if (auto *conv = qobject_cast<Conveyer *>(neighbor)) {
            if (conv->getNext() == obj) conv->setNext(nullptr);
            if (conv->getPrev() == obj) conv->setPrev(nullptr);
        }
        if (auto *gen = qobject_cast<Generator *>(neighbor)) {
            if (gen->getRelated() == obj) gen->setRelated(nullptr);
        }
        if (auto *recv = qobject_cast<Receiver *>(neighbor)) {
            if (recv->getRelated() == obj) recv->setRelated(nullptr);
        }
    }

    obj->deleteLater();
}



void Scene::cancelSelectionObjcts()
{
    for (QGraphicsItem *item : selectedItems())
    {
        item->setSelected(false);
    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        deleteSelectedObjects();
    }
    else if (event->key() == Qt::Key_Escape)
    {
        cancelSelectionObjcts();
        emit showObjectProperties(nullptr);
    }
}


QPointF Scene::snapToGrid(const QPointF& pos)
{
    int x = static_cast<int>(std::floor(pos.x() / gridSize));
    int y = static_cast<int>(std::floor(pos.y() / gridSize));
    return QPointF(x * gridSize, y * gridSize);
}