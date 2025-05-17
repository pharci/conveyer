#include "pch.h"
#include "dropBtn.h"

#include <QDrag>

DropBtn::DropBtn(QByteArray type, QWidget *parent) : QPushButton(parent), type(type) {
}

void DropBtn::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton)) return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-object-type", type);
    drag->setMimeData(mimeData);
    
    // QPixmap pixmap(100, 40); // Размер изображения
    // pixmap.fill(Qt::transparent); // Прозрачный фон

    // QPainter painter(&pixmap);
    // painter.setRenderHint(QPainter::Antialiasing);

    // // Рисуем прямоугольник
    // QPen pen(Qt::black);
    // pen.setWidth(2);
    // painter.setPen(pen);
    // painter.setBrush(Qt::yellow);
    // painter.drawRoundedRect(0, 0, 100, 40, 10, 10);

    // // Рисуем текст
    // painter.setPen(Qt::black);
    // painter.setFont(QFont("Arial", 12));
    // painter.drawText(pixmap.rect(), Qt::AlignCenter, "Generator");

    // // Завершаем
    // painter.end();

    // // Используем этот pixmap
    // drag->setPixmap(pixmap);
    // drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));

    drag->exec(Qt::CopyAction);
}