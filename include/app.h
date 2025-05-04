#pragma once

#include <QtWidgets/QMainWindow>
#include "sidebar.h"
#include "context.h"
#include "scene.h"

class App : public QMainWindow
{
    Q_OBJECT
public:
    App(QWidget *parent = nullptr);
    Sidebar *sidebar;
    Scene *scene;
    Context *context;
    ~App();
};
