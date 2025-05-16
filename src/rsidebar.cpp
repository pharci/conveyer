#include "pch.h"

#include "scene.h"
#include "conveyer.h"
#include "generator.h"
#include "receiver.h"
#include "rsidebar.h"
#include "context.h"

RSidebar::RSidebar(Context *context, QWidget *parent) : QWidget(parent), context(context)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    objectNameLabel = new QLabel("Объект не выбран", this);
    layout->addWidget(objectNameLabel);

    QPushButton *orientationBtn = new QPushButton("Повернуть", this);
    connect(orientationBtn, &QPushButton::clicked, this, &RSidebar::turnObject);
    layout->addWidget(orientationBtn);

    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    selectedCountLabel = new QLabel("", this);
    deleteBtn = new QPushButton("Delete", this);
    layout->addWidget(selectedCountLabel);
    layout->addWidget(deleteBtn);

    connect(deleteBtn, &QPushButton::clicked, this, [this]() {
        emit onBtnDeleteClicked();
    });
}

RSidebar::~RSidebar() {}

void RSidebar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen = QPen(Qt::lightGray, 1);
    painter.setPen(pen);
    painter.drawLine(0, 0, 0, height());
}

void RSidebar::turnObject() {
    if (auto* conv = qobject_cast<Conveyer*>(objectSelected))
        conv->turn();
}

void RSidebar::updateSelectedObject(BaseObject *obj) {
    objectSelected = obj;
    
    if (obj == nullptr) {
        objectNameLabel->setText("Объект не выбран");
        return;
    }
    objectNameLabel->setText(obj->getObjectName());
}

void RSidebar::updateSelectedCount(int count) {
    selectedCountLabel->setText("Выбрано: " + QString::number(count));
}