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
#include "lib\Hardware\memorydefination.h"

struct MemoryBaseInfo
{
	QHBoxLayout* layout;
	QLabel* BaseLabel;
	QLabel* BaseInfo;
	QSpacerItem *horizontalSpace;
	QSpacerItem *BeginhorizontalSpace;
	MemoryBaseInfo(QWidget* father);
};

class MemoryHeadWidget : public QWidget
{
	Q_OBJECT
public:
	MemoryHeadWidget(QWidget *parent, QVector<MemoryData> meminfo);
	QHBoxLayout * mainlayout;
signals:
	void SendMemoryInfomation(const MemoryData& info);
private:
	QSpacerItem * horizontalSpace;
	QVector<MemoryData> BaseInfo;
};

class MemoryInfomationWidget : public QWidget
{
	Q_OBJECT
public:
	MemoryInfomationWidget(QWidget * parent, const MemoryData & data);
	~MemoryInfomationWidget();
	QVBoxLayout *mainlayout;
public slots:
	void ReciveMemoryInfomation(const MemoryData& data);
private:
	MemoryBaseInfo * ModuleType;
	MemoryBaseInfo * ModuleSize;
	MemoryBaseInfo * ModuleManufacturer;
	MemoryBaseInfo * DRAMManufacturer;
	MemoryBaseInfo * SerialNumber;
	MemoryBaseInfo * PartNumber;
	MemoryBaseInfo * ProductDate;
};

class MemoryWidget : public QWidget
{
	Q_OBJECT

public:
	MemoryWidget();
	MemoryWidget(QWidget *parent);
	void Init();
	~MemoryWidget();
private:
	QVBoxLayout *mainlayout;
	MemoryInfomationWidget* Infomation;
	MemoryHeadWidget* Header;
};

