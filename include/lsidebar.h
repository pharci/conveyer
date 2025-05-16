#pragma once

#include <QWidget>

class QAbstractButton;
class Context;
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