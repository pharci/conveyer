#include <QPainter>
#include <QVBoxLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QLabel>

#include "lsidebar.h"
#include "context.h"

LSidebar::LSidebar(Context *context, QWidget *parent)
    : QWidget(parent), context(context)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QButtonGroup *objectsGroup = new QButtonGroup(this);
    objectsGroup->setExclusive(false);
    connect(objectsGroup, &QButtonGroup::buttonClicked, this, &LSidebar::objBtnClicked);

    QLabel* objBtnsLabel = new QLabel("Типы объектов: ", this);
    layout->addWidget(objBtnsLabel);
    
    QToolButton *generatorBtn = new QToolButton(this);
    generatorBtn->setText("Generator");
    generatorBtn->setCheckable(true);
    generatorBtn->setProperty("objectType", static_cast<int>(ObjectType::Generator));
    layout->addWidget(generatorBtn);
    objectsGroup->addButton(generatorBtn);

    QToolButton *receiverBtn = new QToolButton(this);
    receiverBtn->setText("Receiver");
    receiverBtn->setCheckable(true);
    receiverBtn->setProperty("objectType", static_cast<int>(ObjectType::Receiver));
    layout->addWidget(receiverBtn);
    objectsGroup->addButton(receiverBtn);

    QToolButton *conveyerBtn = new QToolButton(this);
    conveyerBtn->setText("Conveyer");
    conveyerBtn->setCheckable(true);
    conveyerBtn->setProperty("objectType", static_cast<int>(ObjectType::Conveyer));
    layout->addWidget(conveyerBtn);
    objectsGroup->addButton(conveyerBtn);

    QToolButton *pusherBtn = new QToolButton(this);
    pusherBtn->setText("Pusher");
    pusherBtn->setCheckable(true);
    pusherBtn->setProperty("objectType", static_cast<int>(ObjectType::Pusher));
    layout->addWidget(pusherBtn);
    objectsGroup->addButton(pusherBtn);


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