#include "pch.h"
#include "context.h"
#include "receiver.h"
#include "generator.h"
#include "conveyer.h"
#include "pusher.h"

Context::Context(QObject *parent) : QObject(parent), currentObjectType(ObjectType::None)
{
}

Context::~Context() {}

void Context::setCurrentObjectType(int id) {
    this->currentObjectType = static_cast<ObjectType>(id);
}

ObjectType Context::getCurrentObjectType() { return currentObjectType; }