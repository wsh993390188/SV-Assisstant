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
	HardwareButton* PCIButton;
	HardwareButton* SIOButton;
	HardwareButton* SMBUSButton;
	HardwareButton* SPDButton;
	HardwareButton* MSRButton;
	HardwareButton* ECButton;
	HardwareButton* ACPIButton;
	HardwareButton* SMBIOSButton;
	HardwareButton* DISKButton;
	HardwareButton* USBButton;
	QVBoxLayout *layout;
	QSpacerItem *verticalSpace;
};
