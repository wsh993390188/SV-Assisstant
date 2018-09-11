#pragma once

#include <QWidget>
#include "hardwaretab.h"

class HardwareMasterTab : public QWidget
{
	Q_OBJECT

public:
	explicit HardwareMasterTab(QWidget *parent);
	~HardwareMasterTab();
signals:
	void SendHardwareTabIndex(int index);
private:
	void Init();
private:
	HardwareButton* IOButton;
	QVBoxLayout *layout;
	QSpacerItem *verticalSpace;
};
