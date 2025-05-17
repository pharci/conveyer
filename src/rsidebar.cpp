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
    layout->setContentsMargins(0, 0, 0, 0);

    // selectedCountLabel = new QLabel("", this);
    // deleteBtn = new QPushButton("Delete", this);
    // layout->addWidget(selectedCountLabel);
    // layout->addWidget(deleteBtn);

    // connect(deleteBtn, &QPushButton::clicked, this, [this]() {
    //     emit onBtnDeleteClicked();
    // });
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