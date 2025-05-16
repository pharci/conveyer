#pragma once

#include <QObject>

#include "enums.h"

class BaseObject;
class Context : public QObject
{
    Q_OBJECT
public:
    explicit Context(QObject *parent = nullptr);
    ~Context();
public slots:
    void setCurrentObjectType(int id);
    ObjectType getCurrentObjectType();
    BaseObject* createCurrentObject();
protected:
    ObjectType currentObjectType;
    QString objectTypeToString(ObjectType type);
};