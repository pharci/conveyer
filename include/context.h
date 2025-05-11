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

    void addSelected(BaseObject* obj);
    void removeSelected(BaseObject* obj);
    int getCountSelected();
    QSet<BaseObject *>* getSelected();
    void clearSelected();
public slots:
    void setCurrentObjectType(int id);
    ObjectType getCurrentObjectType();
    BaseObject* createCurrentObject();
protected:
    ObjectType currentObjectType;
    QString objectTypeToString(ObjectType type);

    QSet<BaseObject *> selectedObjects;
};