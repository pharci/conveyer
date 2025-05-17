#include "pch.h"
#include "scene.h"
#include "conveyer.h"
#include "generator.h"
#include "receiver.h"
#include "baseItem.h"

Conveyer::Conveyer(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Conveyer) {}

Conveyer::~Conveyer() {}

void Conveyer::setNext(BaseObject* obj) { next = obj; }
void Conveyer::setPrev(BaseObject* obj) { prev = obj; }
BaseObject* Conveyer::getNext() { return next; }
BaseObject* Conveyer::getPrev() { return prev; }
void Conveyer::addItem(BaseItem* item) { items.push_back(item); }
QList<BaseItem*> Conveyer::getItems() { return items; }
double Conveyer::getSpeed() { return speed; }
Direction Conveyer::getInDir() { return inDir; }
Direction Conveyer::getOutDir() { return outDir; }

QPointF Conveyer::getStartPoint() { 
    return mapToScene(startPoint); 
};
QPointF Conveyer::getEndPoint() { 
    return mapToScene(endPoint); 
};
QPointF Conveyer::getCenterPoint() { 
    return mapToScene(centerPoint); 
};

void Conveyer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    prepareGeometryChange();
    QRectF r = boundingRect();
    qreal t = 10;
    QPainterPath path;

    startPoint = QPointF();
    endPoint = QPointF(r.right(), r.center().y());

    qreal pW = 3.0;

    if (inDir == Direction::Left && outDir == Direction::Down || 
        inDir == Direction::Right && outDir == Direction::Up ||
        inDir == Direction::Up && outDir == Direction::Left ||
        inDir == Direction::Down && outDir == Direction::Right) {
        path.addRect(QRectF(t, t, r.width() - 2*t, r.height() - t).adjusted(pW, pW, -pW, -pW));
        path.addRect(QRectF(r.width() - t - pW, t, t + pW, r.height() - 2*t).adjusted(0, pW, -pW, -pW));
        startPoint.setX(r.center().x());
        startPoint.setY(r.bottom() - t);
        centerPoint = QPoint(r.right(), r.bottom());
        isCorner = true;
    } 
    else if (
        inDir == Direction::Left && outDir == Direction::Up ||
        inDir == Direction::Right && outDir == Direction::Down ||
        inDir == Direction::Up && outDir == Direction::Right ||
        inDir == Direction::Down && outDir == Direction::Left) {
        path.addRect(QRectF(t, 0, r.width() - 2*t, r.height() - t).adjusted(pW, pW, -pW, -pW));
        path.addRect(QRectF(r.width() - t - pW, t, t + pW, r.height() - 2*t).adjusted(0, pW, -pW, -pW));
        startPoint.setX(r.center().x());
        startPoint.setY(r.top() + t);
        centerPoint = QPoint(r.right(), r.top());
        isCorner = true;
    }
    else {
        path.addRect(QRect(0, t, r.width(), r.height() - 2*t).adjusted(pW, pW, -pW, -pW));
        startPoint.setX(r.left() + t);
        startPoint.setY(r.center().y());
        isCorner = false;
    }

    painter->setPen(QPen(isSelected() ? Qt::green : Qt::black, pW + 1));
    if (prev != nullptr && next != nullptr) painter->fillPath(path, Qt::gray);
    else painter->fillPath(path, Qt::red);
    painter->drawPath(path.simplified());

    //arrow
    painter->setPen(QPen(Qt::white, 5));
    QPainterPath arrowPath;
    arrowPath.moveTo(startPoint);
    arrowPath.lineTo(r.center());
    arrowPath.lineTo(endPoint + QPointF(-t, 0));
    arrowPath.lineTo(endPoint + QPointF(-2 * t, -t));
    arrowPath.moveTo(endPoint + QPointF(-t, 0));
    arrowPath.lineTo(endPoint + QPointF(-2 *t, t));
    painter->drawPath(arrowPath);
}

void Conveyer::clearItems() {
    for (auto *item : items)
    item->deleteLater();
    items.clear();
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
        if (auto* recv = qobject_cast<Receiver*>(next)) recv->setRelated(nullptr);
        next = nullptr;
    }
    if (prev) {
        if (auto* conv = qobject_cast<Conveyer*>(prev)) conv->setNext(nullptr);
        if (auto* gen = qobject_cast<Generator*>(prev)) {
            gen->setRelated(nullptr);
            gen->stop();
        }
        prev = nullptr;
    }

    for (BaseObject* neighbor : objects) {
        Direction dirToNeighbor = getDirectionTo(this->pos(), neighbor->pos());
        Direction dirFromNeighbor = getDirectionTo(neighbor->pos(), this->pos());
        if (auto* conv = qobject_cast<Conveyer*>(neighbor)) {
            if (inDir == dirToNeighbor && conv->outDir == dirFromNeighbor) {
                setPrev(conv);
                conv->setNext(this);
            }
            else if (outDir == dirToNeighbor && conv->inDir == dirFromNeighbor) {
                setNext(conv);
                conv->setPrev(this);
            }
        }

        else if (auto* gen = qobject_cast<Generator*>(neighbor)) {
            if (inDir == dirToNeighbor && !gen->getRelated()) {
                setPrev(gen);
                gen->setRelated(this);
                gen->start();
            }
        }

        else if (auto* recv = qobject_cast<Receiver*>(neighbor)) {
            if (outDir == dirToNeighbor && !recv->getRelated()) {
                setNext(recv);
                recv->setRelated(this);
            }
        }

    }

    scene()->update();
}

void Conveyer::turn() {
    if (!prev && !next) {
        rotationAngle = (rotationAngle + 90) % 360;
        inDir = angleInDir(rotationAngle);
        outDir = angleOutDir(rotationAngle);
        setTransformOriginPoint(boundingRect().center());
        setRotation(rotationAngle);
        for (auto* item : items) { item->setRotation(rotationAngle); }
    }
    else if (prev && !next || prev && next) { 
        rotationAngle = (rotationAngle + 90) % 360;
        if (angleOutDir(rotationAngle) == inDir) rotationAngle = (rotationAngle + 90) % 360;
        outDir = angleOutDir(rotationAngle); 
        setTransformOriginPoint(boundingRect().center());
        setRotation(rotationAngle);
        for (auto* item : items) { item->setRotation(rotationAngle); }
    }
    else if (next && !prev) { 
        rotationAngle = (rotationAngle + 90) % 360;
        if (angleInDir(rotationAngle) == outDir) rotationAngle = (rotationAngle + 90) % 360;
        inDir = angleInDir(rotationAngle);
    }

    Scene* sc = getScene();
    QList<BaseObject*> neighbors = sc->findNeighbors(this);
    this->connection(neighbors);
    sc->update();
}

Direction Conveyer::angleOutDir(int angle) {
    switch (angle % 360) {
        case 0:   return Direction::Right;
        case 90:  return Direction::Down;
        case 180: return Direction::Left;
        case 270: return Direction::Up;
        default:  return Direction::Right;
    }
}

Direction Conveyer::angleInDir(int angle) {
    switch (angle % 360) {
        case 0:   return Direction::Left;
        case 90:  return Direction::Up;
        case 180: return Direction::Right;
        case 270: return Direction::Down;
        default:  return Direction::Right;
    }
}

void Conveyer::moveItems() {
    if (items.size()) {
        for (auto* item : items) {
            if (isCorner) {
                if (!itemProgress.contains(item)) itemProgress[item] = 0.0;

                qreal& t = itemProgress[item];
                t += speed / 50.0;

                if (t >= 1.0) {
                    if (auto* conv = qobject_cast<Conveyer*>(next)) {
                        conv->addItem(item);
                        items.removeOne(item);
                        itemProgress.remove(item);
                    } else if (auto* recv = qobject_cast<Receiver*>(next)) {
                        recv->acceptItem();
                        items.removeOne(item);
                        itemProgress.remove(item);
                        item->deleteLater();
                    }
                    continue;
                }

                qreal radius = QLineF(getCenterPoint(), item->pos()).length();
                qreal angleStart = std::atan2(getStartPoint().y() - getCenterPoint().y(), getStartPoint().x() - getCenterPoint().x());
                qreal angleEnd   = std::atan2(getEndPoint().y() - getCenterPoint().y(), getEndPoint().x() - getCenterPoint().x());
                qreal deltaAngle = angleEnd - angleStart;
                qreal angle = angleStart + t * deltaAngle;

                QPointF pos = QPointF(
                    getCenterPoint().x() + radius * std::cos(angle),
                    getCenterPoint().y() + radius * std::sin(angle)
                );
                item->setPos(pos);
            }
            else {
                QPointF dir = getEndPoint() - getStartPoint();
                QPointF vecToItem = item->pos() - getStartPoint();
                qreal dot = QPointF::dotProduct(vecToItem, dir);
                qreal len2 = QPointF::dotProduct(dir, dir);
                if (dot < len2) {
                    switch (angleOutDir(rotationAngle)) {
                        case Direction::Left:  item->moveBy(-speed, 0); break;
                        case Direction::Right: item->moveBy(speed, 0);  break;
                        case Direction::Up:    item->moveBy(0, -speed); break;
                        case Direction::Down:  item->moveBy(0, speed);  break;
                        default: break;
                    }
                }
                else { 
                    if (auto* conv = qobject_cast<Conveyer*>(next)) {
                        conv->addItem(item);
                        items.removeOne(item);
                    }
                    if (auto* recv = qobject_cast<Receiver*>(next)) {
                        recv->acceptItem();
                        items.removeOne(item);
                        item->deleteLater();
                    }
                }

            }
        }
    }
}


QWidget* Conveyer::createPropertiesWidget(QWidget* parent) {
    QWidget* container = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(container);

    QLabel *nameLabel = new QLabel("Конвейер", container);
    layout->addWidget(nameLabel);

    QLabel* speedLabel = new QLabel("Скорость: ", container);
    QSpinBox* speedSpin = new QSpinBox(container);
    speedSpin->setRange(1, 10);
    speedSpin->setValue(speed);
    layout->addWidget(speedLabel);
    layout->addWidget(speedSpin);
    connect(speedSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
        speed = value;
    });

    QLabel* label = new QLabel("Действия: ", container);
    layout->addWidget(label);

    QPushButton *orientationBtn = new QPushButton("Повернуть", container);
    connect(orientationBtn, &QPushButton::clicked, this, &Conveyer::turn);
    layout->addWidget(orientationBtn);

    QPushButton *clearBtn = new QPushButton("Очистить", container);
    connect(clearBtn, &QPushButton::clicked, this, &Conveyer::clearItems);
    layout->addWidget(clearBtn);

    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QPushButton *deleteBtn = new QPushButton("Удалить", container);
    layout->addWidget(deleteBtn);
    connect(deleteBtn, &QPushButton::clicked, this, [this]() {
        getScene()->deleteObject(this);
    });

    container->setLayout(layout);
    return container;
}