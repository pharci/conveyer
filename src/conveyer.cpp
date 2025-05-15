#include "conveyer.h"
#include "generator.h"
#include "receiver.h"
#include "scene.h"
#include <QDebug>

Conveyer::Conveyer(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Conveyer) {
    shape.addRect(QRect(0, 5, 50, 40));
}

Conveyer::~Conveyer() {}

void Conveyer::updateShape(Direction inDir, Direction outDir) {

    shape = QPainterPath();

    const qreal thickness = 20;
    const qreal length = 50;

    QRectF hor, vert;

    if ((inDir == Direction::Left && outDir == Direction::Down) ||
        (inDir == Direction::Down && outDir == Direction::Left)) {
        hor = QRectF(0, 0, length, thickness);
        vert = QRectF(length - thickness, 0, thickness, length);
    }
    else if ((inDir == Direction::Left && outDir == Direction::Up) ||
             (inDir == Direction::Up && outDir == Direction::Left)) {
        hor = QRectF(0, thickness, length, thickness);
        vert = QRectF(length - thickness, 0, thickness, thickness);
    }
    else if ((inDir == Direction::Right && outDir == Direction::Down) ||
             (inDir == Direction::Down && outDir == Direction::Right)) {
        hor = QRectF(0, 0, length, thickness);
        vert = QRectF(0, 0, thickness, length);
    }
    else if ((inDir == Direction::Right && outDir == Direction::Up) ||
             (inDir == Direction::Up && outDir == Direction::Right)) {
        hor = QRectF(0, thickness, length, thickness);
        vert = QRectF(0, 0, thickness, thickness);
    }
    else {
        shape.addRect(QRectF(0, thickness, length, thickness));
        return;
    }

    shape.addRect(hor);
    shape.addRect(vert);
}

QDebug operator<<(QDebug debug, const Direction& dir) {
    switch (dir) {
        case Direction::Left:  return debug << "Left";
        case Direction::Right: return debug << "Right";
        case Direction::Up:    return debug << "Up";
        case Direction::Down:  return debug << "Down";
        case Direction::None:  return debug << "None";
    }
    return debug << "Unknown";
}

void Conveyer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    qreal offset = 5;

    shape = QPainterPath();
    shape.addRect(QRect(0, 5, 50, 40));

    QPointF start(rect.left() + offset, rect.center().y());
    prepareGeometryChange();
    if (inDir == Direction::Left && outDir == Direction::Down) {
        start = QPointF(rect.center().x(),rect.bottom() - offset);
        shape = QPainterPath();
        shape.addRect(QRectF(5, 5, 40, 45));
        shape.addRect(QRectF(45, 5, 5, 40));
    }
    else if (inDir == Direction::Left && outDir == Direction::Up) {
        start = QPointF(rect.center().x(),rect.top() + offset);
    }
    else if (inDir == Direction::Right && outDir == Direction::Down) {
        start = QPointF(rect.center().x(),rect.top() + offset);
    }
    else if (inDir == Direction::Right && outDir == Direction::Up) {
        start = QPointF(rect.center().x(),rect.bottom() - offset);
    }
    else if (inDir == Direction::Up && outDir == Direction::Right) {
        start = QPointF(rect.center().x(), rect.top() + offset);
    }
    else if (inDir == Direction::Up && outDir == Direction::Left) {
        start = QPointF(rect.center().x(), rect.bottom() - offset);
    }
    else if (inDir == Direction::Down && outDir == Direction::Right) {
        start = QPointF(rect.center().x(), rect.bottom() - offset);
    }
    else if (inDir == Direction::Down && outDir == Direction::Left) {
        start = QPointF(rect.center().x(), rect.top() + offset);
    }

    QPointF end(rect.right() - offset, rect.center().y());

    if (highlighted) { painter->setPen(QPen(Qt::green, 3)); }
    painter->fillPath(shape, QBrush(Qt::red));
    if (prev != nullptr && next != nullptr) { painter->fillPath(shape, QBrush(Qt::gray)); }
    painter->drawPath(shape);

    painter->setPen(QPen(Qt::white, 2));
    QPainterPath path;
    path.moveTo(start);
    path.lineTo(rect.center());
    path.lineTo(end);
    path.lineTo(end + QPointF(-offset, -offset));
    path.moveTo(end);
    path.lineTo(end + QPointF(-offset, offset));
    painter->drawPath(path);
}

Direction Conveyer::getDirectionTo(const QPointF& from, const QPointF& to) {
    QPointF delta = to - from;

    if (qAbs(delta.x()) > qAbs(delta.y())) {
        return delta.x() > 0 ? Direction::Right : Direction::Left;
    } else {
        return delta.y() > 0 ? Direction::Down : Direction::Up;
    }
}

void Conveyer::connection(QList<BaseObject*> objects) {
    if (next) {
        if (auto* conv = qobject_cast<Conveyer*>(next)) conv->setPrev(nullptr);
        if (auto* conv = qobject_cast<Receiver*>(next)) conv->setRelated(nullptr);
        next = nullptr;
    }
    if (prev) {
        if (auto* conv = qobject_cast<Conveyer*>(prev)) conv->setNext(nullptr);
        if (auto* conv = qobject_cast<Generator*>(prev)) conv->setRelated(nullptr);
        prev = nullptr;
    }

    for (BaseObject* neighbor : objects) {
        Direction dirToNeighbor = getDirectionTo(this->pos(), neighbor->pos());
        Direction dirFromNeighbor = getDirectionTo(neighbor->pos(), this->pos());
        if (auto* conv = qobject_cast<Conveyer*>(neighbor)) {
            qDebug() << "dirToNeighbor:" << dirToNeighbor << "dirFromNeighbor:" << dirFromNeighbor;
            qDebug() << "inDir:" << inDir << "outDir:" << outDir;
            qDebug() << "conv->inDir:" << conv->inDir << "conv->outDir:" << conv->outDir;
            if (inDir == dirToNeighbor && conv->outDir == dirFromNeighbor) {
                setPrev(conv);
                conv->setNext(this);
                qDebug() << "\nConnect prev!\n";
            }
            else if (outDir == dirToNeighbor && conv->inDir == dirFromNeighbor) {
                setNext(conv);
                conv->setPrev(this);
                qDebug() << "\nConnect next!\n";
            }
        }

        else if (auto* gen = qobject_cast<Generator*>(neighbor)) {
            if (inDir == dirToNeighbor && !gen->getRelated()) {
                setPrev(gen);
                gen->setRelated(this);
            }
        }

        else if (auto* recv = qobject_cast<Receiver*>(neighbor)) {
            if (outDir == dirToNeighbor && !recv->getRelated()) {
                setNext(recv);
                recv->setRelated(this);
            }
        }
    }
}

void Conveyer::setNext(BaseObject* obj) { next = obj; }
void Conveyer::setPrev(BaseObject* obj) { prev = obj; }
BaseObject* Conveyer::getNext() { return next; }
BaseObject* Conveyer::getPrev() { return prev; }
void Conveyer::addItem(BaseItem* item) { items.push_back(item); }
QList<BaseItem*> Conveyer::getItems() { return items; }
double Conveyer::getSpeed() { return speed; }
Direction Conveyer::getInDir() { return inDir; }
Direction Conveyer::getOutDir() { return outDir; }

void Conveyer::turn() {
    if (!prev && !next || prev && next) {
        rotationAngle = (rotationAngle + 90) % 360;
        switch (rotationAngle) {
            case 90:
                inDir = Direction::Up;
                outDir = Direction::Down;
                break;
            case 180:
                inDir = Direction::Right;
                outDir = Direction::Left;
                break;
            case 270:
                inDir = Direction::Down;
                outDir = Direction::Up;
                break;
            default: // 0 град
                inDir = Direction::Left;
                outDir = Direction::Right;
                break;
        }
        setTransformOriginPoint(boundingRect().center());
        setRotation(rotationAngle);
        for (auto* item : items) { item->setRotation(rotationAngle); }
    }
    else if (prev && !next) { 
        rotationAngle = (rotationAngle + 90) % 360;
        if (angleInDir(rotationAngle) == inDir) rotationAngle = (rotationAngle + 90) % 360;
        outDir = angleInDir(rotationAngle); 
        setTransformOriginPoint(boundingRect().center());
        setRotation(rotationAngle);
        for (auto* item : items) { item->setRotation(rotationAngle); }
    }
    else if (next && !prev) { 
        rotationAngle = (rotationAngle + 90) % 360;
        if (angleOutDir(rotationAngle) == outDir) rotationAngle = (rotationAngle + 90) % 360;
        inDir = angleOutDir(rotationAngle);
    }

    if (auto* sc = dynamic_cast<Scene*>(scene())) {
        QList<BaseObject*> neighbors = sc->findNeighbors(this);
        this->connection(neighbors);
        sc->update();
    }
}

Direction Conveyer::angleInDir(int angle) {
    switch (angle % 360) {
        case 0:   return Direction::Right;
        case 90:  return Direction::Down;
        case 180: return Direction::Left;
        case 270: return Direction::Up;
        default:  return Direction::Right; // безопасное значение по умолчанию
    }
}

Direction Conveyer::angleOutDir(int angle) {
    switch (angle % 360) {
        case 0:   return Direction::Left;
        case 90:  return Direction::Up;
        case 180: return Direction::Right;
        case 270: return Direction::Down;
        default:  return Direction::Right; // безопасное значение по умолчанию
    }
}

void Conveyer::moveItems() {
    if (items.size()) {
        for (auto* item : items) {
            if (item->pos().x() - boundingRect().x() < boundingRect().width()) {
                item->moveBy(speed, 0);
            }
            else { 
                if (auto* conv = qobject_cast<Conveyer*>(next)) {
                    conv->addItem(item);
                    item->setParentItem(conv);
                    item->setPos(QPointF(0, boundingRect().width() / 2));
                    items.removeOne(item);
                }
                if (auto* recv = qobject_cast<Receiver*>(next)) {
                    recv->addItem(item);
                    items.removeOne(item);
                }
            }
        }
    }
}