#include "pch.h"

#include "lsidebar.h"
#include "context.h"
#include "dropBtn.h"

LSidebar::LSidebar(Context *context, QWidget *parent) : QWidget(parent), context(context)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QButtonGroup *objectsGroup = new QButtonGroup(this);
    objectsGroup->setExclusive(false);
    connect(objectsGroup, &QButtonGroup::buttonClicked, this, &LSidebar::objBtnClicked);

    QLabel* objBtnsLabel = new QLabel("Типы объектов: ", this);
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
    
    // QToolButton *generatorBtn = new QToolButton(this);
    // generatorBtn->setText("Generator");
    // generatorBtn->setCheckable(true);
    // generatorBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    // generatorBtn->setProperty("objectType", static_cast<int>(ObjectType::Generator));
    // layout->addWidget(generatorBtn);
    // objectsGroup->addButton(generatorBtn);

    // QToolButton *receiverBtn = new QToolButton(this);
    // receiverBtn->setText("Receiver");
    // receiverBtn->setCheckable(true);
    // receiverBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    // receiverBtn->setProperty("objectType", static_cast<int>(ObjectType::Receiver));
    // layout->addWidget(receiverBtn);
    // objectsGroup->addButton(receiverBtn);

    // QToolButton *conveyerBtn = new QToolButton(this);
    // conveyerBtn->setText("Conveyer");
    // conveyerBtn->setCheckable(true);
    // conveyerBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    // conveyerBtn->setProperty("objectType", static_cast<int>(ObjectType::Conveyer));
    // layout->addWidget(conveyerBtn);
    // objectsGroup->addButton(conveyerBtn);

    // QToolButton *pusherBtn = new QToolButton(this);
    // pusherBtn->setText("Pusher");
    // pusherBtn->setCheckable(true);
    // pusherBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    // pusherBtn->setProperty("objectType", static_cast<int>(ObjectType::Pusher));
    // layout->addWidget(pusherBtn);
    // objectsGroup->addButton(pusherBtn);


    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

LSidebar::~LSidebar() {}

void LSidebar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen = QPen(Qt::lightGray, 1);
    painter.setPen(pen);
    painter.drawLine(width(), 0, width(), height());
}

void LSidebar::objBtnClicked(QAbstractButton *btn) {
    if (currentObjBtn == btn) {
        btn->setChecked(false);
        currentObjBtn = nullptr;
        emit context->setCurrentObjectType(static_cast<int>(ObjectType::None));
        return;
    }
    
    if (currentObjBtn)
        currentObjBtn->setChecked(false);
    
    currentObjBtn = btn;
    int id = btn->property("objectType").toInt();
    emit context->setCurrentObjectType(id);
}