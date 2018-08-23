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
#include <windows.h>
#include <string>
#include <vector>
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
signals:
	void SendDiskInfomation(const SV_ASSIST::Storage::DISK_SMART_INFO& info);
private:
	QSpacerItem * horizontalSpace;
	QVector<SV_ASSIST::Storage::DISK_SMART_INFO> BaseInfo;
};

class DiskInfomationWidget : public QWidget
{
	Q_OBJECT
public:
	DiskInfomationWidget(QWidget * parent, const SV_ASSIST::Storage::DISK_SMART_INFO & data);
	~DiskInfomationWidget();
	QVBoxLayout *mainlayout;
public slots:
	void ReciveDiskInfomation(const SV_ASSIST::Storage::DISK_SMART_INFO& data);
private:
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
	DiskWidget();
	DiskWidget(QWidget *parent);
	void Init();
	~DiskWidget();
private:
	QVBoxLayout *mainlayout;
	DiskInfomationWidget* Infomation;
	DiskHeadWidget* Header;
};
