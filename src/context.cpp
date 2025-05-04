#include "context.h"
#include "QMessageBox"

Context::Context(QObject *parent) : QObject(parent), currentObjectType(ObjectType::Generator)
{
}

Context::~Context() {}

void Context::setObjectType(int id) {
    this->currentObjectType = static_cast<ObjectType>(id);

    QMessageBox::information(nullptr, "Выбран:", objectTypeToString(currentObjectType));
}

QString Context::objectTypeToString(ObjectType type) {
    switch (type) {
        case ObjectType::Generator: return "Generator";
        case ObjectType::Receiver: return "Receiver";
        case ObjectType::Conveyer: return "Conveyer";
        default: return "Unknown";
    }
}

BaseObject* Context::createCurrentObject() {
    switch (currentObjectType) {
        case ObjectType::Generator: return new Generator;
        case ObjectType::Receiver: return new Receiver;
        case ObjectType::Conveyer: return new Conveyer;
        default: return nullptr;
    }
}