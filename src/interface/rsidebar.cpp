#include "common/pch.h"

#include "rsidebar.h"
#include "scene/objects/conveyer.h"
#include "scene/objects/generator.h"
#include "scene/objects/receiver.h"
#include "scene/scene.h"

RSidebar::RSidebar(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    setFixedWidth(150);
}

RSidebar::~RSidebar() {}

void RSidebar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen = QPen(Qt::lightGray, 1);
    painter.setPen(pen);
    painter.drawLine(0, 0, 0, height());
}

void RSidebar::updateObjectProperties(BaseObject *obj) {
    if (widgetProperties) {
        layout()->removeWidget(widgetProperties);
        widgetProperties->setParent(nullptr);
        widgetProperties->deleteLater();
        widgetProperties = nullptr;
    }

    if (obj) {
        widgetProperties = obj->createPropertiesWidget(this);
        layout()->addWidget(widgetProperties);
    }
}