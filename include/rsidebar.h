#pragma once
#include <QWidget>
#include <QAbstractButton>
#include <QLabel>
#include "scene.h"
#include "context.h"
#include <QPushButton>

class RSidebar : public QWidget
{
	Q_OBJECT
public:
	explicit RSidebar(Context *context, QWidget *parent = nullptr);
	~RSidebar();
private:
	Context *context;
    BaseObject* objectSelected = nullptr;
	void paintEvent(QPaintEvent *event) override;
    QLabel* objectNameLabel;
	QLabel* selectedCountLabel;
	QPushButton* deleteBtn;
	void onBtnDeleteClicked();
	void turnObject();
public slots:
    void updateSelectedObject(BaseObject *obj);
};