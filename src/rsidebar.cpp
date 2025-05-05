#include <QPainter>
#include <QVBoxLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QLabel>

#include "rsidebar.h"
#include "context.h"

RSidebar::RSidebar(Context *context, QWidget *parent)
    : QWidget(parent), context(context)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    objectNameLabel = new QLabel("Объект не выбран", this);
    layout->addWidget(objectNameLabel);

    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
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

void RSidebar::setObjectSelected(BaseObject* obj) {
    objectSelected = obj;
}

void RSidebar::updateSelectedObject(BaseObject *obj) {
    if (!obj) return;
    objectNameLabel->setText(obj->getObjectName());
}