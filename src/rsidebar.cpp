#include <QPainter>
#include <QVBoxLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>

#include "scene.h"
#include "conveyer.h"
#include "generator.h"
#include "receiver.h"
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
    if (objectSelected != nullptr)
        objectSelected->turn();
}

void RSidebar::updateSelectedObject(BaseObject *obj) {
    objectSelected = obj;
    selectedCountLabel->setText("Выбрано: " + QString::number(context->getCountSelected()));
    if (obj == nullptr) {
        objectNameLabel->setText("Не выбрано");
        return;
    }
    objectNameLabel->setText(obj->getObjectName());
}

void RSidebar::onBtnDeleteClicked() {
    for (BaseObject* obj : *context->getSelected()) {
        auto* sc = dynamic_cast<Scene*>(obj->scene());
        QList<BaseObject*> neighbors = sc->findNeighbors(obj);

        for (auto neighbor : neighbors) {
            if (auto* conv = qobject_cast<Conveyer*>(neighbor)) {
                if (conv->getNext() == obj) conv->setNext(nullptr);
                if (conv->getPrev() == obj) conv->setPrev(nullptr);
            }
            if (auto* gen = qobject_cast<Generator*>(neighbor)) {
                if (gen->getRelated() == obj) gen->setRelated(nullptr);
            }
            if (auto* recv = qobject_cast<Receiver*>(neighbor)) {
                if (recv->getRelated() == obj) recv->setRelated(nullptr);
            }
        }


        if (auto* conv = qobject_cast<Conveyer*>(obj)) {
            QList<BaseItem*> items = conv->getItems();
            if (items.size()) {
                for (auto* item : items) {
                    conv->removeItem(item);
                    item->setConveyer(nullptr);
                    item->deleteLater();
                }
            }
        }

        obj->deleteLater();
        sc->update();
    }

    context->clearSelected();
    updateSelectedObject(nullptr);
}