#pragma once

#include <QtWidgets/QMainWindow>

class Context;
class Scene;
class RSidebar;
class LSidebar;
class View;
class App : public QMainWindow {
    Q_OBJECT
  public:
    App(QWidget *parent = nullptr);
    LSidebar *lsidebar;
    Scene *scene;
    RSidebar *rsidebar;
    Context *context;
    View *view;
    ~App();
};
