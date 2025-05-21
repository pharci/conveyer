#include "enums.h"
#include "pch.h"

Direction getDirectionTo(const QPointF &from, const QPointF &to) {
    QPointF delta = to - from;

    if (qAbs(delta.x()) > qAbs(delta.y())) {
        return delta.x() > 0 ? Direction::Right : Direction::Left;
    } else {
        return delta.y() > 0 ? Direction::Down : Direction::Up;
    }
}

Direction angleOutDir(int angle) {
    switch (angle % 360) {
        case 0:
            return Direction::Right;
        case 90:
            return Direction::Down;
        case 180:
            return Direction::Left;
        case 270:
            return Direction::Up;
        default:
            return Direction::Right;
    }
}

Direction angleInDir(int angle) {
    switch (angle % 360) {
        case 0:
            return Direction::Left;
        case 90:
            return Direction::Up;
        case 180:
            return Direction::Right;
        case 270:
            return Direction::Down;
        default:
            return Direction::Right;
    }
}