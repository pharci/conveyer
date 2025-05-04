#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QVector>
#include <QCheckBox>

#include "scene.h"
#include "context.h"


class Sidebar : public QWidget
{
	Q_OBJECT
public:
	explicit Sidebar(Context *context, QWidget *parent = nullptr);
	~Sidebar();
private:
	Context* context;
	void paintEvent(QPaintEvent *event) override;
};