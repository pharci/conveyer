#include "QMessageBox"
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

    QMessageBox::information(nullptr, "Выбран:", objectTypeToString(currentObjectType));
}

ObjectType Context::getCurrentObjectType() { return currentObjectType; }

QString Context::objectTypeToString(ObjectType type) {
    switch (type) {
        case ObjectType::Generator: return "Generator";
        case ObjectType::Receiver: return "Receiver";
        case ObjectType::Conveyer: return "Conveyer";
        case ObjectType::Pusher: return "Pusher";
        case ObjectType::None: return "None";
        default: return "Unknown";
    }
}

BaseObject* Context::createCurrentObject() {
    switch (currentObjectType) {
        case ObjectType::Generator: return new Generator;
        case ObjectType::Receiver: return new Receiver;
        case ObjectType::Conveyer: return new Conveyer;
        case ObjectType::Pusher: return new Pusher;
        case ObjectType::None: return nullptr;
        default: return nullptr;
    }
}