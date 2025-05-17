#include <QGraphicsView>

class View : public QGraphicsView {
    Q_OBJECT
private:
    bool isPanning = false;
    QPoint panStart;
    double currentScale = 1.0;
public:
    explicit View(QWidget *parent = nullptr);
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
};