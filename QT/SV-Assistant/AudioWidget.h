#pragma once

#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>

class AudioWidget : public QTableWidget
{
	Q_OBJECT

public:
	AudioWidget(QWidget *parent = nullptr);
	void Init();
	~AudioWidget();
};
