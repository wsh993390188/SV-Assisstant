#pragma once

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QTimer>
#include <QTimerEvent>

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
	explicit CPUHardwareWidget(QWidget *parent = nullptr);
	~CPUHardwareWidget();
protected:
	void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
	void Init();
	void UpdateData();
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
	CPUHardware_Struct *cpuCurrentSpeed;
	CPUHardware_Struct *cpuBusSpeed;
	CPUHardware_Struct *cpuMultiplier;

	QVBoxLayout *mainlayout;
	QHBoxLayout *horizonallayout_1;
	QHBoxLayout *horizonallayout_2;
	QHBoxLayout *horizonallayout_3;
	QHBoxLayout *horizonallayout_4;
	QHBoxLayout *horizonallayout_5;
	QHBoxLayout *horizonallayout_6;
	QHBoxLayout *horizonallayout_7;
	QHBoxLayout *horizonallayout_8;

	size_t timeID;
};
