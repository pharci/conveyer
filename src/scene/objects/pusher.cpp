#include "scene/objects/pusher.h"
#include "common/enums.h"
#include "common/pch.h"
#include "scene/items/baseItem.h"
#include "scene/objects/conveyer.h"
#include "scene/scene.h"

Pusher::Pusher(QGraphicsObject *parent) : BaseObject(parent) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Pusher::push);
    timer->start(5000);

    setZValue(3);
}

Pusher::~Pusher() {}

int Pusher::getRotationAngle() { return rotationAngle; }

void Pusher::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    BaseObject *output = getConnection(ConnectionType::Output);

    QRectF rect = boundingRect();

    painter->setBrush(output ? Qt::darkYellow : Qt::red);
    painter->setPen(QPen(
        isSelected() ? Qt::green : Qt::black,
        isSelected() ? 4.0 : 4.0));

    painter->drawRect(QRect(rect.width() - 20 + pistonOffset, 0, 20, rect.height()).adjusted(0, 3, -3, -3));

    painter->setBrush(output ? Qt::white : Qt::red);
    painter->drawRect(QRect(rect.width() - 30, 40, 10 + pistonOffset, 20));

    painter->setBrush(output ? Qt::lightGray : Qt::red);
    painter->drawRect(QRect(0, 35, rect.width() - 30, 30).adjusted(3, 0, 0, 0));
}

void Pusher::turn() {
    rotationAngle = (rotationAngle + 90) % 360;
    setTransformOriginPoint(boundingRect().center());
    setRotation(rotationAngle);
    Scene *sc = getScene();
    QList<BaseObject *> neighbors = sc->findNeighbors(this);
    connection(neighbors);
    sc->update();
}

void Pusher::connection(QList<BaseObject *> objects) {
    BaseObject *output = getConnection(ConnectionType::Output);
    if (output) {
        output->setConnection(ConnectionType::SideInput, nullptr);
    }
    clearAllConnections();

    for (BaseObject *neighbor : objects) {
        if (auto *conv = qobject_cast<Conveyer *>(neighbor)) {
            Direction dir = getDirectionTo(pos(), conv->pos());
            if (angleOutDir(rotationAngle) == dir) {
                setConnection(ConnectionType::Output, conv);
                conv->setConnection(ConnectionType::SideInput, this);
            }
        }
    }
}

QWidget *Pusher::createPropertiesWidget(QWidget *parent) {
    QWidget *container = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(container);

    QLabel *nameLabel = new QLabel("Толкатель", container);
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

    QPushButton *orientationBtn = new QPushButton("Повернуть", container);
    connect(orientationBtn, &QPushButton::clicked, this, &Pusher::turn);
    layout->addWidget(orientationBtn);

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

void Pusher::push() {
    auto *conv = qobject_cast<Conveyer *>(getConnection(ConnectionType::Output));
    if (!conv) return;

    QRectF area = conv->mapToScene(conv->boundingRect()).boundingRect();
    QList<QGraphicsItem *> items = scene()->items(area);
    QList<BaseItem *> itemsToPush;

    for (QGraphicsItem *item : items) {
        if (item == this) continue;
        if (auto *baseItem = dynamic_cast<BaseItem *>(item)) {
            itemsToPush << baseItem;
        }
    }

    const qreal pushDistance = 150;
    int duration = static_cast<int>((pushDistance / speed * 10));
    QVariantAnimation *forwardAnim = new QVariantAnimation(this);
    forwardAnim->setDuration(duration);
    forwardAnim->setStartValue(0.0);
    forwardAnim->setEndValue(pushDistance);
    forwardAnim->setEasingCurve(QEasingCurve::OutCubic);

    QVariantAnimation *backAnim = new QVariantAnimation(this);
    backAnim->setDuration(duration);
    backAnim->setStartValue(pushDistance);
    backAnim->setEndValue(0.0);
    backAnim->setEasingCurve(QEasingCurve::OutCubic);

    qreal velocity = 100.0 / duration * 15;

    QPointF direction;
    switch (angleOutDir(rotationAngle)) {
        case Direction::Left:
            direction = QPointF(-velocity, 0);
            break;
        case Direction::Right:
            direction = QPointF(velocity, 0);
            break;
        case Direction::Up:
            direction = QPointF(0, -velocity);
            break;
        case Direction::Down:
            direction = QPointF(0, velocity);
            break;
        default:
            return;
    }

    connect(forwardAnim, &QVariantAnimation::valueChanged, this, [=](const QVariant &value) {
        for (auto *item : itemsToPush) {
            item->setVelocity(direction);
        }

        pistonOffset = value.toReal();
        update();
    });

    connect(backAnim, &QVariantAnimation::valueChanged, this, [=](const QVariant &value) {
        pistonOffset = value.toReal();
        update();
    });

    connect(forwardAnim, &QVariantAnimation::finished, this, [=]() {
        backAnim->start(QAbstractAnimation::DeleteWhenStopped);
    });
    forwardAnim->start(QAbstractAnimation::DeleteWhenStopped);
}