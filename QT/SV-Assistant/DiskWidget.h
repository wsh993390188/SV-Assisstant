#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
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
#include "lib\Hardware\DiskLib.h"

struct DiskBaseInfo
{
	QHBoxLayout* layout;
	QLabel* BaseLabel;
	QLabel* BaseInfo;
	QSpacerItem *horizontalSpace;
	QSpacerItem *BeginhorizontalSpace;
	DiskBaseInfo(QWidget* father);
};

class DiskHeadWidget : public QWidget
{
	Q_OBJECT
public:
	DiskHeadWidget(QWidget *parent, QVector<SV_ASSIST::Storage::DISK_SMART_INFO> diskinfo);
	QHBoxLayout * mainlayout;
public slots:
	void ReceiveUpdateData(QVector<SV_ASSIST::Storage::DISK_SMART_INFO> data);
signals:
	void SendDiskInfomation(const SV_ASSIST::Storage::DISK_SMART_INFO& info);
private:
	QSpacerItem * horizontalSpace;
	QVector<mainctrlbutton*> buttons;
	QVector<SV_ASSIST::Storage::DISK_SMART_INFO> BaseInfo;
};

class DiskInfomationWidget : public QWidget
{
	Q_OBJECT
	friend class DiskWidget;
public:
	DiskInfomationWidget(QWidget * parent, const SV_ASSIST::Storage::DISK_SMART_INFO & data);
	~DiskInfomationWidget();
	QVBoxLayout *mainlayout;
public slots:
	void ReciveDiskInfomation(const SV_ASSIST::Storage::DISK_SMART_INFO& data);
private:
	QMutex mutex;
	QLabel * DiskName;
	DiskBaseInfo * Firmware;
	DiskBaseInfo * SerialNumber;
	DiskBaseInfo * Interface;
	DiskBaseInfo * Transfermode;
	DiskBaseInfo * Driverletter;
	DiskBaseInfo * PowerOnHours;
	DiskBaseInfo * PowerOnCount;
	DiskBaseInfo * Standard;
	DiskBaseInfo * Features;
	DiskBaseInfo * RotationRate;
	DiskBaseInfo * Temperature;
	DiskBaseInfo * HostWrites;
	DiskBaseInfo * NandWrites;
	DiskBaseInfo * HostReads;
	DiskBaseInfo * BufferSize;

	QHBoxLayout *horizonallayout_1;
	QHBoxLayout *horizonallayout_2;
	QHBoxLayout *horizonallayout_3;
	QHBoxLayout *horizonallayout_4;
	QHBoxLayout *horizonallayout_5;
	QHBoxLayout *horizonallayout_6;
};

class DiskWidget : public QWidget
{
	Q_OBJECT

public:
	DiskWidget(QWidget *parent = nullptr);
	~DiskWidget();
protected:
	void timerEvent(QTimerEvent * event);
private:
	void Init();
	void UpdateData();
signals:
	void UpdateData(QVector<SV_ASSIST::Storage::DISK_SMART_INFO> data);
	void UpdateData(SV_ASSIST::Storage::DISK_SMART_INFO data);
private:
	int diskTimeID;
	QVBoxLayout *mainlayout;
	DiskInfomationWidget* Infomation;
	DiskHeadWidget* Header;
};
