#include "dropBtn.h"
#include "common/pch.h"
#include <QDrag>

DropBtn::DropBtn(QByteArray type, QWidget *parent) : QPushButton(parent), type(type) {}

void DropBtn::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton)) return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-object-type", type);
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction);
}