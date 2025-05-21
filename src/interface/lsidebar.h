#pragma once

#include <QWidget>

class LSidebar : public QWidget {
    Q_OBJECT
  public:
    explicit LSidebar(QWidget *parent = nullptr);
    ~LSidebar();

  private:
    void paintEvent(QPaintEvent *event) override;
};