#pragma once

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QGroupBox>

struct CPUHardware_Struct
{
	explicit CPUHardware_Struct(QWidget* parent);
	QLabel *cpulabel;
	QLabel *cpuinfo;
	QHBoxLayout *layout;
	QSpacerItem *horizontalSpace;
	QSpacerItem *horizontalSpacelast;
};

class CPUHardwareWidget : public QWidget
{
	Q_OBJECT
public:
	CPUHardwareWidget();
	explicit CPUHardwareWidget(QWidget *parent);
	~CPUHardwareWidget();
private:
	void Init();
	const QString ExecInstructions();
private:
	CPUHardware_Struct *cpuName;
	CPUHardware_Struct *cpucodename;
	CPUHardware_Struct *cpuMAXTDP;
	CPUHardware_Struct *cpuPackage;
	CPUHardware_Struct *cpuTechnology;
	CPUHardware_Struct *cpuCoreVoltage;
	CPUHardware_Struct *cpuSpecification;
	CPUHardware_Struct *cpuFamily;
	CPUHardware_Struct *cpuExtFamily;
	CPUHardware_Struct *cpuModel;
	CPUHardware_Struct *cpuExtModel;
	CPUHardware_Struct *cpuSteping;
	CPUHardware_Struct *cpuInstructions;
	CPUHardware_Struct *cpuCores;
	CPUHardware_Struct *cpuThreads;
	CPUHardware_Struct *cpucacheL1D;
	CPUHardware_Struct *cpucacheL1T;
	CPUHardware_Struct *cpucacheL2;
	CPUHardware_Struct *cpucacheL3;
	QVBoxLayout *mainlayout;
	QHBoxLayout *horizonallayout_1;
	QHBoxLayout *horizonallayout_2;
	QHBoxLayout *horizonallayout_3;
	QHBoxLayout *horizonallayout_4;
};
