#pragma once

#include <QWidget>

class GPUWidget : public QWidget
{
	Q_OBJECT

public:
	GPUWidget();
	GPUWidget(QWidget *parent);
	void Init();
	~GPUWidget();
};
