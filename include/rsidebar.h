#pragma once
#include <QWidget>
#include <QAbstractButton>
#include <QLabel>
#include "scene.h"
#include "context.h"

class RSidebar : public QWidget
{
	Q_OBJECT
public:
	explicit RSidebar(Context *context, QWidget *parent = nullptr);
	~RSidebar();
private:
	Context *context;
    BaseObject* objectSelected;
    void setObjectSelected(BaseObject* obj);
	void paintEvent(QPaintEvent *event) override;
    QLabel* objectNameLabel;
public slots:
    void updateSelectedObject(BaseObject *obj);
};