#include "MotherBroadWidget.h"
#include "lib\Hardware\SV_Hardware.h"

using namespace SV_ASSIST;
MotherBroadWidget::MotherBroadWidget(QWidget *parent)
	: QWidget(parent) ,mainlayout(new QVBoxLayout(this))
{
	QPalette pal(this->palette());
	//ÉèÖÃ±³¾°whiteÉ«
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

void MotherBroadWidget::Init()
{
	auto motherbroad = SV_ASSIST::DMI_SMBIOS::GetSMBIOSinfo().BaseBroadinfo;
	auto biosinfo = DMI_SMBIOS::GetSMBIOSinfo().BIOSinfo;
	Manufacturer = new MotherBroad_Struct(this);
	Manufacturer->baselabel->setText(tr("Manufacturer"));
	QString temp = {};
	motherbroad.count("BaseBroad Manufacturer") > 0 ? temp = QString::fromStdString(motherbroad["BaseBroad Manufacturer"]) + tr(" ") : temp = "Unknown Manufacturer";
	Manufacturer->baseinfo->setText(temp);

	temp = "";
	Model = new MotherBroad_Struct(this);
	Model->baselabel->setText(tr("Model"));
	motherbroad.count("BaseBroad Product") > 0 ? temp = QString::fromStdString(motherbroad["BaseBroad Product"]) : temp = "Unknown Product";
	Model->baseinfo->setText(temp);

	Chipset = new MotherBroad_Struct(this);
	Chipset->baselabel->setText(tr("Chipset"));

	SouthBridge = new MotherBroad_Struct(this);
	SouthBridge->baselabel->setText(tr("SouthBridge"));
	if (CPU::GetManufacturer() == "GenuineIntel")
	{
		Chipset->baseinfo->setText(tr("Intel %1").arg(QString::fromStdString(SV_ASSIST::CPU::GetCodeName())));
		SouthBridge->baseinfo->setText(tr("Intel %1").arg(QString::fromStdString(SV_ASSIST::CPU::GetSouthbridgeName())));
	}
	else if (CPU::GetManufacturer() == "AuthenticAMD")
	{
		Chipset->baseinfo->setText(tr("AMD %1").arg(QString::fromStdString(SV_ASSIST::CPU::GetCodeName())));
		SouthBridge->baseinfo->setText(tr("AMD %1").arg(QString::fromStdString(SV_ASSIST::CPU::GetSouthbridgeName())));
	}
	else if (CPU::GetManufacturer() == "CentaurHauls")
	{
		if (CPU::GetCPUName().find("VIA") != std::string::npos)
		{
			Chipset->baseinfo->setText(tr("VIA %1").arg(QString::fromStdString(SV_ASSIST::CPU::GetCodeName())));
			SouthBridge->baseinfo->setText(tr("VIA %1").arg(QString::fromStdString(SV_ASSIST::CPU::GetSouthbridgeName())));
		}
		else
		{
			Chipset->baseinfo->setText(tr("Zhaoxin %1").arg(QString::fromStdString(SV_ASSIST::CPU::GetCodeName())));
			SouthBridge->baseinfo->setText(tr("Zhaoxin %1").arg(QString::fromStdString(SV_ASSIST::CPU::GetSouthbridgeName())));
		}
	}

	LPCIO = new MotherBroad_Struct(this);
	LPCIO->baselabel->setText(tr("LPCIO"));
	LPCIO->baseinfo->setText(QString::fromStdString(SUPERIO::GetSioSensor().ChipName));

	temp = "";
	BIOSBrand = new MotherBroad_Struct(this);
	BIOSBrand->baselabel->setText(tr("BIOS Brand"));
	biosinfo.count("BIOS Vendor") > 0 ? temp = QString::fromStdString(biosinfo["BIOS Vendor"]) + tr(" ") : temp = "Unknown BIOS Vendor";
	BIOSBrand->baseinfo->setText(temp);

	temp = "";
	BIOSVersion = new MotherBroad_Struct(this);
	BIOSVersion->baselabel->setText(tr("BIOS Version"));
	biosinfo.count("BIOS Version") > 0 ? temp = QString::fromStdString(biosinfo["BIOS Version"]) + tr(" ") : temp = "Unknown BIOS Version";
	BIOSVersion->baseinfo->setText(temp);

	temp = "";
	BIOSDate = new MotherBroad_Struct(this);
	BIOSDate->baselabel->setText(tr("BIOS Date"));
	biosinfo.count("BIOS Release Date") > 0 ? temp = QString::fromStdString(biosinfo["BIOS Release Date"]) + tr(" ") : temp = "Unknown BIOS Release Date";
	BIOSDate->baseinfo->setText(temp);

	mainlayout->addLayout(Manufacturer->layout);
	mainlayout->addLayout(Model->layout);
	mainlayout->addLayout(Chipset->layout);
	mainlayout->addLayout(SouthBridge->layout);
	mainlayout->addLayout(LPCIO->layout);
	mainlayout->addLayout(BIOSBrand->layout);
	mainlayout->addLayout(BIOSVersion->layout);
	mainlayout->addLayout(BIOSDate->layout);
}

MotherBroadWidget::~MotherBroadWidget()
{
	delete Manufacturer;
	delete Model;
	delete Chipset;
	delete SouthBridge;
	delete LPCIO;
	delete BIOSBrand;
	delete BIOSVersion;
	delete BIOSDate;
}

MotherBroad_Struct::MotherBroad_Struct(QWidget * parent) : baselabel(new QLabel(parent)), baseinfo(new QLabel(parent)),
	layout(new QHBoxLayout()), horizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum)),
	BeginhorizontalSpace(new QSpacerItem(80, 20, QSizePolicy::Minimum, QSizePolicy::Minimum))
{
	baselabel->setStyleSheet(QObject::tr("QLabel{color:#000000;font-family: \"consolas\"; font-size:12px; background-color: rgb(255, 255, 255);}"));
	baseinfo->setStyleSheet(QObject::tr("QLabel{color:#000000;font-family: \"consolas\"; font-size:12px; background-color: rgb(255, 255, 255);}"));
	baseinfo->setAlignment(Qt::AlignCenter);
	layout->addWidget(baselabel);
	layout->addSpacerItem(horizontalSpace);
	layout->addWidget(baseinfo);
	layout->addSpacerItem(BeginhorizontalSpace);
}
