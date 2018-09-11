#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpacerItem>
#include <QPalette>
#include <QVector>
#include <QString>
#include <QToolButton>
#include <QTimerEvent>
#include <QMutex>
#include <windows.h>
#include <string>
#include <vector>
#include "mainctrlbutton.h"
#include "lib\Hardware\Ring0Defination.h"
#include "lib\Hardware\gpu.h"

struct GpuGUIBaseInfo
{
	QHBoxLayout* layout;
	QLabel* BaseLabel;
	QLabel* BaseInfo;
	QSpacerItem *horizontalSpace;
	QSpacerItem *BeginhorizontalSpace;
	GpuGUIBaseInfo(QWidget* father);
};

class GPUWidget : public QWidget
{
	Q_OBJECT
public:
	GPUWidget(QWidget *parent = nullptr);
	~GPUWidget();
protected:
	void timerEvent(QTimerEvent * event);
private:
	void Init();
	void Updatedata();
	void Updatedata(int Index);
private:
	QMutex mutex;
	int gpuTimeID;
	QVBoxLayout* mainlayout;
	GpuGUIBaseInfo * gpuname;
	GpuGUIBaseInfo * gpubios;
	GpuGUIBaseInfo * gpuDriver;
	GpuGUIBaseInfo * gpuTemperature;
	GpuGUIBaseInfo * gpuClock;
	GpuGUIBaseInfo * gpuMemoryClock;
	GpuGUIBaseInfo * gpuUsage;
	GpuGUIBaseInfo * gpuFans;
	GpuGUIBaseInfo * gpuMemory;
	GpuGUIBaseInfo * gpuSharedMemory;
	QVector<std::pair<GPUBaseInfo, GPUSensorInfo>> BaseInfo;
	QComboBox *allGPUName;
};
