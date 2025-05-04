#include "scene.h"
#include "receiver.h"
#include "generator.h"
#include "conveyer.h"
#include <QGraphicsView>

Scene::Scene(Context* context, QGraphicsScene *parent) : QGraphicsScene(parent), context(context)
{
    Generator *gen = new Generator();
    gen->setPos(QPoint(50, 50));
    this->addItem(gen);

    Conveyer *conv = new Conveyer();
    conv->setPos(QPoint(500, 50));
    this->addItem(conv);

    Receiver *recv = new Receiver();
    recv->setPos(QPoint(50, 250));
    this->addItem(recv);

    hoverRect = addRect(0, 0, gridSize, gridSize, QPen(Qt::green), QBrush(QColor(100, 100, 255, 50)));
    hoverRect->setZValue(100);
    hoverRect->setVisible(false);
}

Scene::~Scene() {}

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

        auto *object = context->createCurrentObject();

        double x = std::floor(pos.x() / gridSize) * gridSize;
        double y = std::floor(pos.y() / gridSize) * gridSize;
        object->setPos(x, y);

        if (checkLegal(object->sceneBoundingRect())) {
            addItem(object);
        };
    }

    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos();

    double x = std::floor(pos.x() / gridSize) * gridSize;
    double y = std::floor(pos.y() / gridSize) * gridSize;

    hoverRect->setRect(x, y, gridSize, gridSize);

    if (checkLegal(hoverRect->sceneBoundingRect().adjusted(+0.5, +0.5, -0.5, -0.5))) {
        hoverRect->setPen(QPen(Qt::green));
    } else {
        hoverRect->setPen(QPen(Qt::red));
    }
    hoverRect->setVisible(true);

    QGraphicsScene::mouseMoveEvent(event);
}

bool Scene::checkLegal(QRectF rect) {
    for (QGraphicsItem *item : this->items()) {
        if (auto *base = dynamic_cast<BaseObject *>(item)) {
            QRectF item_rect = item->sceneBoundingRect();
            if (item->sceneBoundingRect().intersects(rect)) {
                return false;
            }
        }
    }
    return true;
}