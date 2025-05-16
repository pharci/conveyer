#pragma once

enum class Direction 
{
    Up,
    Down,
    Left,
    Right,
    None
}; 

enum class ObjectType
{
    None,
    Generator,
    Receiver,
    Conveyer,
    Pusher
};