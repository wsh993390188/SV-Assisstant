#pragma once

#include <QWidget>

class NetworkWidget : public QWidget
{
	Q_OBJECT

public:
	NetworkWidget();
	NetworkWidget(QWidget *parent);
	void Init();
	~NetworkWidget();
};
