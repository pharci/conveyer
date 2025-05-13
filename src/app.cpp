#include "app.h"
#include "scene.h"
#include "lsidebar.h"
#include "context.h"

#include <QHBoxLayout>
#include <QWidget>
#include <QGraphicsView>

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

    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setWindowTitle("Conveyer System");
    view->setMouseTracking(true);
    view->show();

    layout->addWidget(view);
    layout->addWidget(rsidebar);

    lsidebar->setFixedWidth(150);
    rsidebar->setFixedWidth(150);
    scene->setSceneRect(0, 0, 3000, 3000);

    connect(scene, &Scene::objectSelected, rsidebar, &RSidebar::updateSelectedObject);
}

App::~App() {}