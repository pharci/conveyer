#pragma once

#include <QWidget>

class BaseObject;
class Context;
class RSidebar : public QWidget
{
	Q_OBJECT
public:
	explicit RSidebar(Context *context, QWidget *parent = nullptr);
	~RSidebar();
private:
	Context *context;
	QWidget *widgetProperties;
	void paintEvent(QPaintEvent *event) override;
public slots:
    void updateObjectProperties(BaseObject *obj);
signals:
	void onBtnDeleteClicked();
};