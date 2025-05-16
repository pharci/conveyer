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
    BaseObject* objectSelected = nullptr;
	void paintEvent(QPaintEvent *event) override;
    QLabel* objectNameLabel;
	QLabel* selectedCountLabel;
	QPushButton* deleteBtn;
	void turnObject();
public slots:
    void updateSelectedObject(BaseObject *obj);
	void updateSelectedCount(int count);
signals:
	void onBtnDeleteClicked();
};