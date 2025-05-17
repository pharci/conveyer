#include "pch.h"

#include "app.h"
#include "lsidebar.h"
#include "rsidebar.h"
#include "context.h"
#include "scene.h"
#include "view.h"

App::App(QWidget *parent) : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    context = new Context(this);

    scene = new Scene(context);
    lsidebar = new LSidebar(context, this);
    rsidebar = new RSidebar(context, this);

    layout->addWidget(lsidebar);

    view = new View;
    view->setScene(scene);
    view->show();

    layout->addWidget(view);
    layout->addWidget(rsidebar);

    lsidebar->setFixedWidth(150);
    rsidebar->setFixedWidth(150);

    connect(scene, &Scene::showObjectProperties, rsidebar, &RSidebar::updateObjectProperties);
    connect(rsidebar, &RSidebar::onBtnDeleteClicked, scene, &Scene::deleteSelectedObjects);
}

App::~App() {}