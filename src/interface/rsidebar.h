#pragma once

#include <QWidget>

class BaseObject;
class RSidebar : public QWidget {
    Q_OBJECT
  public:
    explicit RSidebar(QWidget *parent = nullptr);
    ~RSidebar();

  private:
    QWidget *widgetProperties = nullptr;
    void paintEvent(QPaintEvent *event) override;
  public slots:
    void updateObjectProperties(BaseObject *obj);
  signals:
    void onBtnDeleteClicked();
};