#pragma once

#include <QWidget>

class DiskWidget : public QWidget
{
	Q_OBJECT

public:
	DiskWidget();
	DiskWidget(QWidget *parent);
	void Init();
	~DiskWidget();
};
