#include "pch.h"
#include "generator.h"
#include "conveyer.h"
#include "baseItem.h"
#include "scene.h"

Generator::Generator(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Generator) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Generator::spawnItem);
}

Generator::~Generator() {}

void Generator::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    painter->setBrush(related ? Qt::blue : Qt::red); 
    painter->setPen(QPen(
        isSelected() ? Qt::green : Qt::black, 
        isSelected() ? 4.0 : 4.0)
    );
    if (isSelected()) { painter->setPen(QPen(Qt::green, 3));}
    painter->drawRect(rect.adjusted(3, 3, -3, -3));

    painter->setBrush(related ? Qt::white : Qt::gray); 
    painter->drawEllipse(rect.center(), rect.width() / 2 - 10, rect.height() / 2 - 10);

    QString countText = QString::number(timer->remainingTime() / 1000.0, 'f', 2);
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 14));
    painter->drawText(rect, Qt::AlignCenter, countText);
}

void Generator::setRelated(BaseObject* obj) { 
    related = obj; 
    start();
}

void Generator::start() {
    timer->start(frequency);
}

void Generator::stop() {
    timer->stop();
}

BaseObject* Generator::getRelated() { return related; }

void Generator::connection(QList<BaseObject*> objects) {
    if (related) {
        if (auto* conv = qobject_cast<Conveyer*>(related)) conv->setPrev(nullptr);
        related = nullptr;
        stop();
    }

    for (BaseObject* neighbor : objects) {
        if (auto* conv = qobject_cast<Conveyer*>(neighbor)) {
            Direction dir = conv->getDirectionTo(conv->pos(), this->pos());
            if (conv->getInDir() == dir) {
                setRelated(conv);
                conv->setPrev(this);
            }
        } 
    }
}

void Generator::spawnItem() {
    if (!related || related->getObjectType() != ObjectType::Conveyer)
        return;

    Conveyer* conveyer = dynamic_cast<Conveyer*>(related);
    auto* item = new BaseItem();
    item->setPos(conveyer->getStartPoint());
    conveyer->addItem(item);
    scene()->addItem(item);
}

QWidget* Generator::createPropertiesWidget(QWidget* parent) {
    QWidget* container = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(container);

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

    QLabel* label = new QLabel("Частота:", container);
    QSpinBox* spin = new QSpinBox(container);
    spin->setRange(300, 100000);
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
        getScene()->deleteObject(this);
    });

    container->setLayout(layout);
    return container;
}