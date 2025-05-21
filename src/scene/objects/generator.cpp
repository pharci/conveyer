#include "scene/objects/generator.h"
#include "common/enums.h"
#include "common/pch.h"
#include "scene/items/baseItem.h"
#include "scene/objects/conveyer.h"
#include "scene/scene.h"

Generator::Generator(QGraphicsObject *parent) : BaseObject(parent) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Generator::spawnItem);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        update();
    });
    updateTimer->start(16);
}

Generator::~Generator() {}

void Generator::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    BaseObject *next = getConnection(ConnectionType::Output);

    QRectF rect = boundingRect();
    painter->setBrush(next ? Qt::blue : Qt::red);
    painter->setPen(QPen(
        isSelected() ? Qt::green : Qt::black,
        isSelected() ? 4.0 : 4.0));
    if (isSelected()) { painter->setPen(QPen(Qt::green, 3)); }
    painter->drawRect(rect.adjusted(3, 3, -3, -3));

    painter->setBrush(next ? Qt::white : Qt::gray);
    painter->drawEllipse(rect.center(), rect.width() / 2 - 10, rect.height() / 2 - 10);

    QString countText = QString::number(timer->remainingTime() / 1000.0, 'f', 2);
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 14));
    painter->drawText(rect, Qt::AlignCenter, countText);
}

void Generator::start() {
    timer->start(frequency);
}

void Generator::stop() {
    timer->stop();
}

void Generator::connection(QList<BaseObject *> objects) {
    BaseObject *next = getConnection(ConnectionType::Output);
    if (next) {
        next->setConnection(ConnectionType::Input, nullptr);
    }
    clearAllConnections();

    for (BaseObject *neighbor : objects) {
        if (auto *conv = qobject_cast<Conveyer *>(neighbor)) {
            Direction dir = getDirectionTo(conv->pos(), this->pos());
            if (conv->getInDir() == dir) {
                setConnection(ConnectionType::Output, conv);
                conv->setConnection(ConnectionType::Input, this);
                start();
            }
        }
    }
}

void Generator::spawnItem() {
    BaseObject *next = getConnection(ConnectionType::Output);

    if (!next) return;

    if (auto *conveyer = dynamic_cast<Conveyer *>(next)) {
        auto *item = new BaseItem();
        item->setPos(conveyer->getStartPoint());
        scene()->addItem(item);
    }
}

QWidget *Generator::createPropertiesWidget(QWidget *parent) {
    QWidget *container = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(container);

    QLabel *nameLabel = new QLabel("Генератор", container);
    layout->addWidget(nameLabel);

    QPushButton *timerBtn = new QPushButton(container);
    layout->addWidget(timerBtn);
    timerBtn->setText(timer->isActive() ? "Стоп" : "Старт");
    connect(timerBtn, &QPushButton::clicked, this, [this, timerBtn]() {
        if (timer->isActive()) {
            timer->stop();
            timerBtn->setText("Старт");
        } else {
            timer->start(1000);
            timerBtn->setText("Стоп");
        }
    });

    QLabel *label = new QLabel("Частота:", container);
    QSpinBox *spin = new QSpinBox(container);
    spin->setRange(300, 100000);
    spin->setSingleStep(100);
    spin->setValue(timer->interval());
    layout->addWidget(label);
    layout->addWidget(spin);
    connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value) {
        timer->setInterval(value);
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