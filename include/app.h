#pragma once

#include <QtWidgets/QMainWindow>
#include "lsidebar.h"
#include "rsidebar.h"
#include "context.h"
#include "scene.h"

class App : public QMainWindow
{
    Q_OBJECT
public:
    App(QWidget *parent = nullptr);
    LSidebar *lsidebar;
    Scene *scene;
    RSidebar *rsidebar;
    Context *context;
    ~App();
};
