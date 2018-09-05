#pragma once

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QGroupBox>

struct MotherBroad_Struct
{
	explicit MotherBroad_Struct(QWidget* parent);
	QLabel *baselabel;
	QLabel *baseinfo;
	QHBoxLayout *layout;
	QSpacerItem *horizontalSpace;
	QSpacerItem *BeginhorizontalSpace;
};

class MotherBroadWidget : public QWidget
{
	Q_OBJECT

public:
	MotherBroadWidget(QWidget *parent = nullptr);
	~MotherBroadWidget();
private:
	void Init();
private:
	MotherBroad_Struct * Manufacturer;
	MotherBroad_Struct * Model;
	MotherBroad_Struct * Chipset;
	MotherBroad_Struct * SouthBridge;
	MotherBroad_Struct * LPCIO;
	MotherBroad_Struct * BIOSBrand;
	MotherBroad_Struct * BIOSVersion;
	MotherBroad_Struct * BIOSDate;
	QVBoxLayout *mainlayout;
};
