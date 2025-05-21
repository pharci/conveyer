#include "interface/view.h"
#include "common/pch.h"
#include "scene/scene.h"

#include <QScrollBar>
#include <QWheelEvent>

View::View(QWidget *parent) : QGraphicsView(parent) {
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setMouseTracking(true);
    setDragMode(QGraphicsView::RubberBandDrag);
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);
}

bool View::isPanningActive() const {
    return isPanning;
}

void View::wheelEvent(QWheelEvent *event) {
    const double zoomStep = 1.15;
    double factor;
    if (event->angleDelta().y() > 0)
        factor = zoomStep;
    else if (event->angleDelta().y() < 0)
        factor = 1.0 / zoomStep;
    else
        event->ignore();

    double newScale = currentScale * factor;
    if (newScale < 0.3 || newScale > 5) {
        event->ignore();
        return;
    }

    QPointF cursorScenePos = mapToScene(event->position().toPoint());

    scale(factor, factor);
    currentScale = newScale;

    QPointF newCursorScenePos = mapToScene(event->position().toPoint());
    QPointF offset = newCursorScenePos - cursorScenePos;
    translate(offset.x(), offset.y());

    event->accept();
}

void View::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        isPanning = true;

        panStart = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event) {
    if (isPanning) {
        QPoint delta = event->pos() - panStart;
        panStart = event->pos();

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
}

void View::mouseReleaseEvent(QMouseEvent *event) {
    if (isPanning && event->button() == Qt::RightButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void View::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/x-object-type")) event->acceptProposedAction();
}

void View::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasFormat("application/x-object-type")) event->acceptProposedAction();
}

void View::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasFormat("application/x-object-type")) return;

    QString type = QString::fromUtf8(event->mimeData()->data("application/x-object-type"));
    QPointF pos = mapToScene(event->position().toPoint());

    Scene *sc = qobject_cast<Scene *>(scene());
    if (sc) sc->craeteNewObject(pos, type);

    event->acceptProposedAction();
}