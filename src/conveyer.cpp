#include "conveyer.h"

Conveyer::Conveyer(QGraphicsObject *parent) : BaseObject(parent, ObjectType::Conveyer, QRect(0, 0, 50, 50), Qt::gray)
{
}

Conveyer::~Conveyer()
{
}