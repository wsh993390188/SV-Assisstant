#pragma once

#include <QWidget>

class MonitorWidget : public QWidget
{
	Q_OBJECT
public:
	MonitorWidget();
	MonitorWidget(QWidget *parent);
	void Init();
	~MonitorWidget();
};
