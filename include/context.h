#pragma once
#include <QObject>
#include <QVector>
#include "baseObject.h"
#include "receiver.h"
#include "generator.h"
#include "conveyer.h"

enum class ObjectType
{
    Generator,
    Receiver,
    Conveyer
};

class Context : public QObject
{
    Q_OBJECT
public:
    explicit Context(QObject *parent = nullptr);
    ~Context();
public slots:
    void setObjectType(int id);
    BaseObject* createCurrentObject();
protected:
    ObjectType currentObjectType;
    QString objectTypeToString(ObjectType type);
};