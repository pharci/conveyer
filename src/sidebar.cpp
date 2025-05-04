#include <QPainter>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>

#include "sidebar.h"
#include "context.h"

Sidebar::Sidebar(Context *context, QWidget *parent)
    : QWidget(parent), context(context)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QRadioButton *generatorBtn = new QRadioButton("Generator", this);
    QRadioButton *receiverBtn = new QRadioButton("Receiver", this);
    QRadioButton *conveyerBtn = new QRadioButton("Conveyer", this);
    QButtonGroup *objectsGroup = new QButtonGroup(this);

    objectsGroup->addButton(generatorBtn, static_cast<int>(ObjectType::Generator));
    objectsGroup->addButton(receiverBtn, static_cast<int>(ObjectType::Receiver));
    objectsGroup->addButton(conveyerBtn, static_cast<int>(ObjectType::Conveyer));

    connect(objectsGroup, &QButtonGroup::idClicked, context, &Context::setObjectType);


    generatorBtn->setChecked(true);

    layout->addWidget(generatorBtn);
    layout->addWidget(receiverBtn);
    layout->addWidget(conveyerBtn);

    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

}

Sidebar::~Sidebar() {}

void Sidebar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen = QPen(Qt::lightGray, 1);
    painter.setPen(pen);
    painter.drawLine(width(), 0, width(), height());
}