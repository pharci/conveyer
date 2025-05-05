#pragma once
#include <QWidget>
#include <QAbstractButton>
#include "scene.h"
#include "context.h"


class LSidebar : public QWidget
{
	Q_OBJECT
public:
	explicit LSidebar(Context *context, QWidget *parent = nullptr);
	~LSidebar();
private:
	Context *context;
	void paintEvent(QPaintEvent *event) override;
	QAbstractButton *currentObjBtn = nullptr;
	void objBtnClicked(QAbstractButton *btn);
};