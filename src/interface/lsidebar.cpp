#include "common/pch.h"

#include "interface/dropBtn.h"
#include "interface/lsidebar.h"

LSidebar::LSidebar(QWidget *parent) : QWidget(parent) {
    setFixedWidth(150);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *objBtnsLabel = new QLabel("Типы объектов: ", this);
    layout->addWidget(objBtnsLabel);

    DropBtn *gen = new DropBtn("Generator");
    gen->setText("Генератор");
    layout->addWidget(gen);

    DropBtn *conv = new DropBtn("Conveyer");
    conv->setText("Конвейер");
    layout->addWidget(conv);

    DropBtn *recv = new DropBtn("Receiver");
    recv->setText("Приемник");
    layout->addWidget(recv);

    DropBtn *push = new DropBtn("Pusher");
    push->setText("Толкатель");
    layout->addWidget(push);

    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

LSidebar::~LSidebar() {}

void LSidebar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen = QPen(Qt::lightGray, 1);
    painter.setPen(pen);
    painter.drawLine(width(), 0, width(), height());
}