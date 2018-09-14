#include "HardwareMasterTab.h"

HardwareMasterTab::HardwareMasterTab(QWidget *parent)
	: QWidget(parent), IOButton{ new HardwareButton(this) },
	PCIButton{ new HardwareButton(this) }, SIOButton{ new HardwareButton(this) }, SMBUSButton{ new HardwareButton(this) }, SPDButton{ new HardwareButton(this) },
	MSRButton{ new HardwareButton(this) }, ECButton{ new HardwareButton(this) }, ACPIButton{ new HardwareButton(this) }, SMBIOSButton{ new HardwareButton(this) },
	DISKButton{ new HardwareButton(this) }, USBButton{ new HardwareButton(this) },
	layout{ new QVBoxLayout(this) }, verticalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum))
{
	QPalette pal(this->palette());
	//ÉèÖÃ±³¾°whiteÉ«
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();

	connect(this->IOButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(0); });
	connect(this->PCIButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(1); });
	connect(this->SIOButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(2); });
	connect(this->SMBUSButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(3); });
	connect(this->SPDButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(4); });
	connect(this->MSRButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(5); });
	connect(this->ECButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(6); });
	connect(this->ACPIButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(7); });
	connect(this->SMBIOSButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(8); });
	connect(this->DISKButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(9); });
	connect(this->USBButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(10); });
}

HardwareMasterTab::~HardwareMasterTab()
{
}

void HardwareMasterTab::Init()
{
	IOButton->setText(tr("IO"));
	PCIButton->setText(tr("PCI"));
	SIOButton->setText(tr("Super Io"));
	SMBUSButton->setText(tr("SMBUS"));
	SPDButton->setText(tr("SPD"));
	MSRButton->setText(tr("MSR"));
	ECButton->setText(tr("EC"));
	ACPIButton->setText(tr("ACPI"));
	SMBIOSButton->setText(tr("SMBIOS"));
	DISKButton->setText(tr("Storage"));
	USBButton->setText(tr("Usb"));

	layout->setContentsMargins(0, 0, 0, 0);
	layout->setMargin(0);
	layout->addWidget(IOButton);
	layout->addWidget(PCIButton);
	layout->addWidget(SIOButton);
	layout->addWidget(SMBUSButton);
	layout->addWidget(SPDButton);
	layout->addWidget(MSRButton);
	layout->addWidget(ECButton);
	layout->addWidget(ACPIButton);
	layout->addWidget(SMBIOSButton);
	layout->addWidget(DISKButton);
	layout->addWidget(USBButton);
	layout->addSpacerItem(verticalSpace);
	layout->setSpacing(0);
}
