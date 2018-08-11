#include "MemoryWidget.h"
#include "mainctrlbutton.h"
#include "lib/Hardware/SV_Hardware.h"

MemoryWidget::MemoryWidget() :mainlayout(new QVBoxLayout(this))
{
	QPalette pal(this->palette());
	//设置背景white色
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

MemoryWidget::MemoryWidget(QWidget *parent)
	: QWidget(parent), mainlayout(new QVBoxLayout(this))
{
	QPalette pal(this->palette());
	//设置背景white色
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

void MemoryWidget::Init()
{
	auto meminfo = SV_ASSIST::Memory::GetMemoryGlobalInfomation();

	if (!meminfo.empty())
	{
		Infomation = new MemoryInfomationWidget(this, meminfo.at(0).second);
		QVector<MemoryData> temp;
		for (const auto& i : meminfo)
		{
			temp.push_back(i.second);
		}
		Header = new MemoryHeadWidget(this, temp);
	}
	mainlayout->addWidget(Header);
	mainlayout->addWidget(Infomation);
	connect(this->Header, &MemoryHeadWidget::SendMemoryInfomation, this->Infomation, &MemoryInfomationWidget::ReciveMemoryInfomation);
}

MemoryWidget::~MemoryWidget()
{
	delete Infomation;
	delete Header;
}

MemoryBaseInfo::MemoryBaseInfo(QWidget* father) : BaseLabel(new QLabel(father)), BaseInfo(new QLabel(father)),
horizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum)), layout(new QHBoxLayout()),
BeginhorizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum))
{
	BaseLabel->setStyleSheet(QObject::tr("QLabel{color:#000000;font-family: \"consolas\"; font-size:14px;background-color: rgb(255, 255, 255);}"));
	BaseInfo->setStyleSheet(QObject::tr("QLabel{color:#000000;font-family: \"consolas\"; font-size:14px;background-color: rgb(255, 255, 255);}"));
	BaseInfo->setAlignment(Qt::AlignCenter);
	layout->addItem(BeginhorizontalSpace);
	layout->addWidget(BaseLabel);
	layout->addItem(horizontalSpace);
	layout->addWidget(BaseInfo);
}

MemoryHeadWidget::MemoryHeadWidget(QWidget *parent, QVector<MemoryData> meminfo) : QWidget(parent), mainlayout(new QHBoxLayout(this)), BaseInfo(meminfo),
horizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum))
{
	this->setMinimumSize(QSize(725,80));
	this->setMaximumSize(QSize(725,80));
	for (size_t i = 0; i < meminfo.size(); i++)
	{
		mainctrlbutton * toolbutton = new mainctrlbutton(this);
		toolbutton->setMinimumSize(QSize(45, 80));
		toolbutton->setMaximumSize(QSize(60, 100));
		QString temp = QString::fromStdString(BaseInfo[i].ModuleManufacturer);
		toolbutton->setText(tr("DIMM #%1").arg(i));
		toolbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		connect(toolbutton, &QToolButton::clicked, this, [=] { emit SendMemoryInfomation(BaseInfo[i]); });
		mainlayout->addWidget(toolbutton);
	}
	mainlayout->addSpacerItem(horizontalSpace);
}

MemoryInfomationWidget::MemoryInfomationWidget(QWidget *parent, const MemoryData& data) : QWidget(parent), mainlayout(new QVBoxLayout(this))
{
	ModuleType = new MemoryBaseInfo(this);
	ModuleType->BaseLabel->setText("Module Type");

	ModuleSize = new MemoryBaseInfo(this);
	ModuleSize->BaseLabel->setText("Module Size");

	ModuleManufacturer = new MemoryBaseInfo(this);
	ModuleManufacturer->BaseLabel->setText("Module Manufacturer");

	DRAMManufacturer = new MemoryBaseInfo(this);
	DRAMManufacturer->BaseLabel->setText("DRAM Manufacturer");

	SerialNumber = new MemoryBaseInfo(this);
	SerialNumber->BaseLabel->setText("SerialNumber");

	PartNumber = new MemoryBaseInfo(this);
	PartNumber->BaseLabel->setText("PartNumber");

	ProductDate = new MemoryBaseInfo(this);
	ProductDate->BaseLabel->setText("Product Date");

	ModuleType->BaseInfo->setText(QString::fromStdString(data.ModuleType));
	ModuleSize->BaseInfo->setText(tr("%1 GBytes").arg(QString::number(data.ModuleSize)));
	ModuleManufacturer->BaseInfo->setText(QString::fromStdString(data.ModuleManufacturer));
	DRAMManufacturer->BaseInfo->setText(QString::fromStdString(data.DRAMManufacturer));
	SerialNumber->BaseInfo->setText(QString::fromStdString(data.SerialNumber));
	PartNumber->BaseInfo->setText(QString::fromStdString(data.PartNumber));
	ProductDate->BaseInfo->setText(QString::fromStdString(data.ProductDate));

	mainlayout->addLayout(ModuleType->layout);
	mainlayout->addLayout(ModuleSize->layout);
	mainlayout->addLayout(ModuleManufacturer->layout);
	mainlayout->addLayout(DRAMManufacturer->layout);
	mainlayout->addLayout(SerialNumber->layout);
	mainlayout->addLayout(PartNumber->layout);
	mainlayout->addLayout(ProductDate->layout);
}

void MemoryInfomationWidget::ReciveMemoryInfomation(const MemoryData& data)
{
	ModuleType->BaseInfo->setText(QString::fromStdString(data.ModuleType));
	ModuleSize->BaseInfo->setText(tr("%1 GBytes").arg(QString::number(data.ModuleSize)));
	ModuleManufacturer->BaseInfo->setText(QString::fromStdString(data.ModuleManufacturer));
	DRAMManufacturer->BaseInfo->setText(QString::fromStdString(data.DRAMManufacturer));
	SerialNumber->BaseInfo->setText(QString::fromStdString(data.SerialNumber));
	PartNumber->BaseInfo->setText(QString::fromStdString(data.PartNumber));
	ProductDate->BaseInfo->setText(QString::fromStdString(data.ProductDate));
}

MemoryInfomationWidget::~MemoryInfomationWidget()
{
	delete ModuleType;
	delete ModuleSize;
	delete ModuleManufacturer;
	delete DRAMManufacturer;
	delete SerialNumber;
	delete PartNumber;
	delete ProductDate;
}
