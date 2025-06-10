#pragma once

#include <QPushButton>

class QByteArray;
class QWidget;
class DropBtn : public QPushButton {
    Q_OBJECT
  private:
    const QByteArray type;

  public:
    explicit DropBtn(QByteArray type, QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *event) override;
};