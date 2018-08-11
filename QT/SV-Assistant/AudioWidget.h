#pragma once

#include <QWidget>

class AudioWidget : public QWidget
{
	Q_OBJECT

public:
	AudioWidget();
	AudioWidget(QWidget *parent);
	void Init();
	~AudioWidget();
};
