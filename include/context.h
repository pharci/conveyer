#pragma once
#include <QObject>
#include <QVector>
#include "baseObject.h"

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