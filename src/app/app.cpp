#include "common/pch.h"

#include "app.h"
#include "interface/lsidebar.h"
#include "interface/rsidebar.h"
#include "interface/view.h"
#include "scene/scene.h"

// #include <QOpenGLWidget>

App::App(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    scene = new Scene();
    view = new View;
    view->setScene(scene);
    // view->setViewport(new QOpenGLWidget());
    view->show();

    lsidebar = new LSidebar(this);
    rsidebar = new RSidebar(this);

    layout->addWidget(lsidebar);
    layout->addWidget(view);
    layout->addWidget(rsidebar);

    connect(scene, &Scene::showObjectProperties, rsidebar, &RSidebar::updateObjectProperties);
    connect(rsidebar, &RSidebar::onBtnDeleteClicked, scene, &Scene::deleteSelectedObjects);
}

App::~App() {}