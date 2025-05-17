#include "pch.h"
#include "receiver.h"
#include "conveyer.h"
#include "scene.h"

Receiver::Receiver(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Receiver) {}

Receiver::~Receiver() {}

void Receiver::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QRectF rect = boundingRect();
    painter->setBrush(related ? Qt::black : Qt::red); 
    painter->setPen(QPen(
        isSelected() ? Qt::green : Qt::black, 
        isSelected() ? 4.0 : 4.0)
    );
    painter->drawRect(rect.adjusted(3, 3, -3, -3));

    painter->setBrush(related ? Qt::green : Qt::gray); 
    painter->drawRect(15, 15, rect.width() - 30, rect.height() - 30);

    QString countText = QString::number(totalReceived);
    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 14));
    painter->drawText(rect, Qt::AlignCenter, countText);
}

void Receiver::setRelated(BaseObject* obj) { related = obj; }

BaseObject* Receiver::getRelated() { return related; }

void Receiver::connection(QList<BaseObject*> objects) {
    if (related) {
        if (auto* conv = qobject_cast<Conveyer*>(related)) conv->setNext(nullptr);
        related = nullptr;
    }

    for (BaseObject* neighbor : objects) {
        if (auto* conv = qobject_cast<Conveyer*>(neighbor)) {
            Direction dir = conv->getDirectionTo(conv->pos(), this->pos());
            if (conv->getOutDir() == dir) {
                setRelated(conv);
                conv->setNext(this);
            }
        } 
    }
}

void Receiver::acceptItem() {
    totalReceived++;
    scene()->update();
}


QWidget* Receiver::createPropertiesWidget(QWidget* parent) {
    QWidget* container = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(container);

    QLabel *nameLabel = new QLabel("Приёмник", container);
    layout->addWidget(nameLabel);

    QLabel* label = new QLabel("Действия:", container);
    QPushButton* reset = new QPushButton("Сбросить счётчик", container);
    layout->addWidget(label);
    layout->addWidget(reset);
    connect(reset, &QPushButton::clicked, this, [this] {
        totalReceived = 0;
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