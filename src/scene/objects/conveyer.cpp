#include "scene/objects/conveyer.h"
#include "common/enums.h"
#include "common/pch.h"
#include "scene/items/baseItem.h"
#include "scene/objects/generator.h"
#include "scene/objects/pusher.h"
#include "scene/objects/receiver.h"
#include "scene/scene.h"

Conveyer::Conveyer(QGraphicsObject *parent) : BaseObject(parent) {
    patternTimer = new QTimer(this);
    connect(patternTimer, &QTimer::timeout, this, [this]() {
        patternOffset = (patternOffset - speed) % 20;
        update();
    });
    patternTimer->start(16);
}

Conveyer::~Conveyer() {}

QPainterPath Conveyer::shape() {
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
        path.addRect(QRectF(t, t, r.width() - 2 * t, r.height() - t).adjusted(pW, pW, -pW, -pW));
        path.addRect(QRectF(r.width() - t - pW, t, t + pW, r.height() - 2 * t).adjusted(0, pW, -pW, -pW));
        startPoint.setX(r.center().x());
        startPoint.setY(r.bottom() - t);
        centerPoint = QPoint(r.right(), r.bottom());
        isCorner = true;

    } else if (
        inDir == Direction::Left && outDir == Direction::Up ||
        inDir == Direction::Right && outDir == Direction::Down ||
        inDir == Direction::Up && outDir == Direction::Right ||
        inDir == Direction::Down && outDir == Direction::Left) {
        path.addRect(QRectF(t, 0, r.width() - 2 * t, r.height() - t).adjusted(pW, pW, -pW, -pW));
        path.addRect(QRectF(r.width() - t - pW, t, t + pW, r.height() - 2 * t).adjusted(0, pW, -pW, -pW));
        startPoint.setX(r.center().x());
        startPoint.setY(r.top() + t);
        centerPoint = QPoint(r.right(), r.top());
        isCorner = true;
    } else {
        path.addRect(QRect(0, t, r.width(), r.height() - 2 * t).adjusted(pW, pW, -pW, -pW));
        startPoint.setX(r.left() + t);
        startPoint.setY(r.center().y());
        isCorner = false;
    }

    return path;
}

void Conveyer::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    prepareGeometryChange();
    QRectF r = boundingRect();
    qreal t = 10;
    QPainterPath path = shape();
    qreal pW = 3;

    painter->setPen(QPen(isSelected() ? Qt::green : Qt::black, pW + 1));

    if (getConnection(ConnectionType::Input) != nullptr && getConnection(ConnectionType::Output) != nullptr)
        painter->fillPath(path, Qt::gray);
    else
        painter->fillPath(path, Qt::red);
    painter->drawPath(path.simplified());

    if (!isCorner) {
        painter->setPen(QPen(Qt::white, pW));
        for (int x = -patternOffset; x < r.width() - t - pW; x += 20) {
            QPointF p1(x + t - pW, r.top() + 2 * t);
            QPointF p2(x + t - pW, r.bottom() - 2 * t);
            painter->drawLine(p1, p2);
        }
    } else {
        // //arrow
        painter->setPen(QPen(Qt::white, 5));
        QPainterPath arrowPath;
        arrowPath.moveTo(startPoint);
        arrowPath.lineTo(r.center());
        arrowPath.lineTo(endPoint + QPointF(-t, 0));
        arrowPath.lineTo(endPoint + QPointF(-2 * t, -t));
        arrowPath.moveTo(endPoint + QPointF(-t, 0));
        arrowPath.lineTo(endPoint + QPointF(-2 * t, t));
        painter->drawPath(arrowPath);
    }
}

Direction Conveyer::getInDir() { return inDir; }
Direction Conveyer::getOutDir() { return outDir; }

void Conveyer::stop() {
    patternTimer->stop();
}
void Conveyer::start() {
    patternTimer->start();
}

QPointF Conveyer::getStartPoint() {
    return mapToScene(startPoint);
};
QPointF Conveyer::getCenterPoint() {
    return mapToScene(centerPoint);
};

void Conveyer::advance(int step) {
    if (!step) return;
    for (auto *item : nearbyItems()) {
        if (collidesWithItem(item)) {
            if (auto *baseItem = dynamic_cast<BaseItem *>(item))
                affect(baseItem);
        }
    }
}

void Conveyer::affect(BaseItem *item) {
    if (!item) return;

    QPointF p;

    if (isCorner) {
        QPointF center = getCenterPoint();
        QPointF pos = item->pos();
        qreal radius = QLineF(center, pos).length();
        qreal angle = std::atan2(pos.y() - center.y(), pos.x() - center.x());
        qreal tangentAngle = angle;

        if (inDir == Direction::Left && outDir == Direction::Down ||
            inDir == Direction::Right && outDir == Direction::Up ||
            inDir == Direction::Up && outDir == Direction::Left ||
            inDir == Direction::Down && outDir == Direction::Right) {
            tangentAngle += M_PI_2;

        } else if (
            inDir == Direction::Left && outDir == Direction::Up ||
            inDir == Direction::Right && outDir == Direction::Down ||
            inDir == Direction::Up && outDir == Direction::Right ||
            inDir == Direction::Down && outDir == Direction::Left) {
            tangentAngle -= M_PI_2;
        }

        p = QPointF(std::cos(tangentAngle), std::sin(tangentAngle)) * speed;
    } else {
        switch (angleOutDir(rotationAngle)) {
            case Direction::Left:
                p = QPointF(-speed, 0);
                break;
            case Direction::Right:
                p = QPointF(speed, 0);
                break;
            case Direction::Up:
                p = QPointF(0, -speed);
                break;
            case Direction::Down:
                p = QPointF(0, speed);
                break;
            default:
                break;
        }
    }

    item->setVelocity(p);
}

QList<QGraphicsItem *> Conveyer::nearbyItems() {
    if (!scene()) return {};
    QPainterPath path = shape();
    QPainterPath scenePath = mapToScene(path);
    QRectF area = scenePath.boundingRect();
    QList<QGraphicsItem *> itemsInArea = scene()->items(area);
    itemsInArea.removeAll(this);
    return itemsInArea;
}

void Conveyer::connection(QList<BaseObject *> objects) {
    for (auto [type, obj] : connections.toStdMap()) {
        if (!obj) continue;
        for (auto [otherType, otherObj] : obj->getConnections().toStdMap()) {
            if (otherObj == this) {
                obj->setConnection(otherType, nullptr);
                break;
            }
        }
    }
    clearAllConnections();

    for (BaseObject *neighbor : objects) {
        Direction dirToNeighbor = getDirectionTo(this->pos(), neighbor->pos());
        Direction dirFromNeighbor = getDirectionTo(neighbor->pos(), this->pos());
        if (auto *conv = qobject_cast<Conveyer *>(neighbor)) {
            if (inDir == dirToNeighbor && conv->outDir == dirFromNeighbor) {
                setConnection(ConnectionType::Input, conv);
                conv->setConnection(ConnectionType::Output, this);
            } else if (outDir == dirToNeighbor && conv->inDir == dirFromNeighbor) {
                setConnection(ConnectionType::Output, conv);
                conv->setConnection(ConnectionType::Input, this);
            }
        }

        else if (auto *gen = qobject_cast<Generator *>(neighbor)) {
            if (inDir == dirToNeighbor && !gen->getConnection(ConnectionType::Output)) {
                setConnection(ConnectionType::Input, gen);
                gen->setConnection(ConnectionType::Output, this);
                gen->start();
            }
        }

        else if (auto *recv = qobject_cast<Receiver *>(neighbor)) {
            if (outDir == dirToNeighbor && !recv->getConnection(ConnectionType::Input)) {
                setConnection(ConnectionType::Output, recv);
                recv->setConnection(ConnectionType::Input, this);
            }
        }

        else if (auto *push = qobject_cast<Pusher *>(neighbor)) {
            if (angleOutDir(push->getRotationAngle()) == dirFromNeighbor) {
                setConnection(ConnectionType::SideInput, push);
                push->setConnection(ConnectionType::Output, this);
            }
        }
    }

    scene()->update();
}

void Conveyer::turn() {
    BaseObject *prev = getConnection(ConnectionType::Input);
    BaseObject *next = getConnection(ConnectionType::Output);
    if (!prev && !next) {
        rotationAngle = (rotationAngle + 90) % 360;
        inDir = angleInDir(rotationAngle);
        outDir = angleOutDir(rotationAngle);
        setTransformOriginPoint(boundingRect().center());
        setRotation(rotationAngle);
    } else if (prev && !next || prev && next) {
        rotationAngle = (rotationAngle + 90) % 360;
        if (angleOutDir(rotationAngle) == inDir) rotationAngle = (rotationAngle + 90) % 360;
        outDir = angleOutDir(rotationAngle);
        setTransformOriginPoint(boundingRect().center());
        setRotation(rotationAngle);
    } else if (next && !prev) {
        rotationAngle = (rotationAngle + 90) % 360;
        if (angleInDir(rotationAngle) == outDir) rotationAngle = (rotationAngle + 90) % 360;
        inDir = angleInDir(rotationAngle);
    }

    Scene *sc = getScene();
    QList<BaseObject *> neighbors = sc->findNeighbors(this);
    this->connection(neighbors);
    sc->update();
}

QWidget *Conveyer::createPropertiesWidget(QWidget *parent) {
    QWidget *container = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(container);

    QLabel *nameLabel = new QLabel("Конвейер", container);
    layout->addWidget(nameLabel);

    QLabel *speedLabel = new QLabel("Скорость: ", container);
    QSpinBox *speedSpin = new QSpinBox(container);
    speedSpin->setRange(1, 10);
    speedSpin->setValue(speed);
    layout->addWidget(speedLabel);
    layout->addWidget(speedSpin);
    connect(speedSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
        speed = value;
    });

    QLabel *label = new QLabel("Действия: ", container);
    layout->addWidget(label);

    QPushButton *orientationBtn = new QPushButton("Повернуть", container);
    connect(orientationBtn, &QPushButton::clicked, this, &Conveyer::turn);
    layout->addWidget(orientationBtn);

    // QPushButton *clearBtn = new QPushButton("Очистить", container);
    // connect(clearBtn, &QPushButton::clicked, this, &Conveyer::clearItems);
    // layout->addWidget(clearBtn);

    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QPushButton *deleteBtn = new QPushButton("Удалить", container);
    layout->addWidget(deleteBtn);
    connect(deleteBtn, &QPushButton::clicked, this, [this]() {
        Scene *sc = getScene();
        sc->deleteObject(this);
        sc->showObjectProperties(nullptr);
    });

    container->setLayout(layout);
    return container;
}