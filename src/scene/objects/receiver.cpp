#include "scene/objects/receiver.h"
#include "common/enums.h"
#include "common/pch.h"
#include "scene/items/baseItem.h"
#include "scene/objects/conveyer.h"
#include "scene/scene.h"

Receiver::Receiver(QGraphicsObject *parent) : BaseObject(parent) {}

Receiver::~Receiver() {}

void Receiver::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    BaseObject *input = getConnection(ConnectionType::Input);

    QRectF rect = boundingRect();
    painter->setBrush(input ? Qt::black : Qt::red);
    painter->setPen(QPen(
        isSelected() ? Qt::green : Qt::black,
        isSelected() ? 4.0 : 4.0));
    painter->drawRect(rect.adjusted(3, 3, -3, -3));

    painter->setBrush(input ? Qt::green : Qt::gray);
    painter->drawRect(15, 15, rect.width() - 30, rect.height() - 30);

    QString countText = QString::number(totalReceived);
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 14));
    painter->drawText(rect, Qt::AlignCenter, countText);
}

void Receiver::connection(QList<BaseObject *> objects) {
    BaseObject *input = getConnection(ConnectionType::Input);
    if (input) {
        input->setConnection(ConnectionType::Output, nullptr);
    }
    clearAllConnections();

    for (BaseObject *neighbor : objects) {
        if (auto *conv = qobject_cast<Conveyer *>(neighbor)) {
            Direction dir = getDirectionTo(conv->pos(), this->pos());
            if (conv->getOutDir() == dir) {
                setConnection(ConnectionType::Input, conv);
                conv->setConnection(ConnectionType::Output, this);
            }
        }
    }
}

void Receiver::advance(int step) {
    if (!step) return;

    QRectF area = mapToScene(boundingRect()).boundingRect();
    QList<QGraphicsItem *> items = scene()->items(area);
    items.removeAll(this);

    for (auto *item : items) {
        if (collidesWithItem(item)) {
            if (auto *baseItem = dynamic_cast<BaseItem *>(item)) {
                baseItem->deleteLater();
                totalReceived++;
                update();
            }
        }
    }
}

QWidget *Receiver::createPropertiesWidget(QWidget *parent) {
    QWidget *container = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(container);

    QLabel *nameLabel = new QLabel("Приёмник", container);
    layout->addWidget(nameLabel);

    QLabel *label = new QLabel("Действия:", container);
    QPushButton *reset = new QPushButton("Сбросить счётчик", container);
    layout->addWidget(label);
    layout->addWidget(reset);
    connect(reset, &QPushButton::clicked, this, [this] {
        totalReceived = 0;
    });

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