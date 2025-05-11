#include <QPainter>
#include <QVBoxLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>

#include "rsidebar.h"
#include "context.h"

RSidebar::RSidebar(Context *context, QWidget *parent)
    : QWidget(parent), context(context)
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

    connect(deleteBtn, &QPushButton::clicked, this, &RSidebar::onBtnDeleteClicked);

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
    objectSelected->turn();
}

void RSidebar::updateSelectedObject(BaseObject *obj) {
    if (!obj) return;
    objectSelected = obj;
    objectNameLabel->setText(obj->getObjectName());
    selectedCountLabel->setText("Выбрано: " + QString::number(context->getCountSelected()));
}

void RSidebar::onBtnDeleteClicked() {
    for (QGraphicsItem* item : *context->getSelected()) {
        if (item->scene()) {
            item->scene()->removeItem(item);  
            delete item;
        }
    }
    context->clearSelected();
}