#include "app.h"
#include "scene.h"
#include "sidebar.h"
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
    sidebar = new Sidebar(context, this);

    layout->addWidget(sidebar);

    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setWindowTitle("Conveyer System");
    view->setMouseTracking(true);
    view->show();

    layout->addWidget(view);

    sidebar->setFixedWidth(300);
    scene->setSceneRect(0, 0, 3000, 3000);
}

App::~App() {}