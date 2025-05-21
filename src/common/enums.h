#pragma once

enum class Direction {
    Up,
    Down,
    Left,
    Right,
    None
};

enum class ObjectType {
    None,
    Generator,
    Receiver,
    Conveyer,
    Pusher
};

enum class ConnectionType {
    Input,
    Output,
    SideInput
};

Direction getDirectionTo(const QPointF &from, const QPointF &to);
Direction angleOutDir(int angle);
Direction angleInDir(int angle);