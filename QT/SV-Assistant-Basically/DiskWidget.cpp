#include "DiskWidget.h"

DiskWidget::DiskWidget(QWidget *parent)
	: QWidget(parent), mainlayout(new QVBoxLayout(this))
{
	QPalette pal(this->palette());
	//ÉèÖÃ±³¾°whiteÉ«
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

void DiskWidget::Init()
{
	auto diskinfo = SV_ASSIST::Storage::GetDiskSMARTInfo();

	if (!diskinfo.empty())
	{
		Infomation = new DiskInfomationWidget(this, diskinfo.at(0));
		QVector<SV_ASSIST::Storage::DISK_SMART_INFO> temp;
		for (const auto& i : diskinfo)
		{
			temp.push_back(i);
		}
		Header = new DiskHeadWidget(this, temp);
		mainlayout->addWidget(Header);
		mainlayout->addWidget(Infomation);
	}

	connect(this->Header, &DiskHeadWidget::SendDiskInfomation, this->Infomation, &DiskInfomationWidget::ReciveDiskInfomation);
	connect(this, static_cast<void (DiskWidget::*)(QVector<SV_ASSIST::Storage::DISK_SMART_INFO> data)>(&DiskWidget::UpdateData), this->Header, &DiskHeadWidget::ReceiveUpdateData);
	connect(this, static_cast<void (DiskWidget::*)(SV_ASSIST::Storage::DISK_SMART_INFO data)>(&DiskWidget::UpdateData), this->Infomation, &DiskInfomationWidget::ReciveDiskInfomation);
	diskTimeID = this->startTimer(10000);
}

DiskWidget::~DiskWidget()
{
	delete Infomation;
	delete Header;
}

void DiskWidget::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == diskTimeID && this->isVisible())
	{
		this->UpdateData();
	}
}

void DiskWidget::UpdateData()
{
	SV_ASSIST::Storage::UpdateData();
	auto diskinfo = SV_ASSIST::Storage::GetDiskSMARTInfo();
	QVector<SV_ASSIST::Storage::DISK_SMART_INFO> temp;
	for (const auto& i : diskinfo)
	{
		if (Infomation->DiskName->text().indexOf(QString::fromStdWString(i.Model)) != -1)
			emit UpdateData(i);
		temp.push_back(i);
	}
	emit UpdateData(temp);
}

DiskHeadWidget::DiskHeadWidget(QWidget *parent, QVector<SV_ASSIST::Storage::DISK_SMART_INFO> diskinfo) : QWidget(parent), mainlayout(new QHBoxLayout(this)), BaseInfo(diskinfo),
horizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum))
{
	this->setMinimumSize(QSize(725, 80));
	this->setMaximumSize(QSize(1125, 80));
	for (size_t i = 0; i < diskinfo.size(); i++)
	{
		mainctrlbutton * toolbutton = new mainctrlbutton(this);
		toolbutton->setMinimumSize(QSize(45, 80));
		toolbutton->setMaximumSize(QSize(60, 100));
		QString temp = QString::fromStdWString(BaseInfo[i].LogicalDrive);
		toolbutton->setText(tr("Disk\n%1").arg(QString::fromStdWString(diskinfo[i].LogicalDrive)));
		toolbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		connect(toolbutton, &QToolButton::clicked, this, [=] {	emit SendDiskInfomation(BaseInfo[i]); });
		buttons.push_back(toolbutton);
		mainlayout->addWidget(toolbutton);
	}
	mainlayout->setMargin(4);
	mainlayout->addSpacerItem(horizontalSpace);
}

void DiskHeadWidget::ReceiveUpdateData(QVector<SV_ASSIST::Storage::DISK_SMART_INFO> data)
{
	if (BaseInfo.size() == data.size())
	{
		for (int i =0;i<BaseInfo.size();i++)
		{
			BaseInfo[i].Temperature = data[i].Temperature;
			BaseInfo[i].PowerOnCount = data[i].PowerOnCount;
			BaseInfo[i].HostWrites = data[i].HostWrites;
			BaseInfo[i].HostReads = data[i].HostReads;
			BaseInfo[i].NandWrites = data[i].NandWrites;
			BaseInfo[i].PowerOnHours = data[i].PowerOnHours;
			BaseInfo[i].DiskState = data[i].DiskState;
		}
	}
	else
	{
		BaseInfo.swap(data);
	}

}

DiskBaseInfo::DiskBaseInfo(QWidget* father) : BaseLabel(new QLabel(father)), BaseInfo(new QLabel(father)),
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
DiskInfomationWidget::DiskInfomationWidget(QWidget * parent, const SV_ASSIST::Storage::DISK_SMART_INFO & data) : QWidget(parent), mainlayout(new QVBoxLayout(this)),
horizonallayout_1(new QHBoxLayout()), horizonallayout_2(new QHBoxLayout()), horizonallayout_3(new QHBoxLayout()),
horizonallayout_4(new QHBoxLayout()), horizonallayout_5(new QHBoxLayout()), horizonallayout_6(new QHBoxLayout())
{
	DiskName = new QLabel(this);
	DiskName->setText(tr("%1 %2").arg(QString::fromStdWString(data.Model)).arg(QString::fromStdWString(data.TotalDiskSize)));
	DiskName->setAlignment(Qt::AlignCenter);
	DiskName->setStyleSheet(QObject::tr("QLabel{color:#000000;font-family: \"consolas\"; font-size:14px;background-color: rgb(255, 255, 255);}"));

	Firmware = new DiskBaseInfo(this);
	Firmware->BaseLabel->setText(tr("Firmware"));
	Firmware->BaseInfo->setText(QString::fromStdWString(data.FirmwareRev));

	SerialNumber = new DiskBaseInfo(this);
	SerialNumber->BaseLabel->setText(tr("Serial Number"));
	SerialNumber->BaseInfo->setText(QString::fromStdWString(data.SerialNumber));

	Interface = new DiskBaseInfo(this);
	Interface->BaseLabel->setText(tr("Interface"));
	Interface->BaseInfo->setText(QString::fromStdWString(data.Interface));

	Transfermode = new DiskBaseInfo(this);
	Transfermode->BaseLabel->setText(tr("Transfer Mode"));
	Transfermode->BaseInfo->setText(tr("%1 | %2").arg(QString::fromStdWString(data.CurrentTransferMode)).arg(QString::fromStdWString(data.MaxTransferMode)));

	Driverletter = new DiskBaseInfo(this);
	Driverletter->BaseLabel->setText(tr("Driver Letter"));
	Driverletter->BaseInfo->setText(QString::fromStdWString(data.LogicalDrive));

	Standard = new DiskBaseInfo(this);
	Standard->BaseLabel->setText(tr("Standard"));
	Standard->BaseInfo->setText(tr("%1 | %2").arg(QString::fromStdWString(data.MinorVersion)).arg(QString::fromStdWString(data.MajorVersion)));

	Features = new DiskBaseInfo(this);
	Features->BaseLabel->setText(tr("Features"));
	Features->BaseInfo->setText(QString::fromStdWString(data.Feature));

	HostReads = new DiskBaseInfo(this);
	HostReads->BaseLabel->setText(tr("Total Host Reads"));
	if (data.HostReads.empty())
		HostReads->BaseInfo->setText(tr("----"));
	else
		HostReads->BaseInfo->setText(QString::fromStdWString(data.HostReads));

	HostWrites = new DiskBaseInfo(this);
	HostWrites->BaseLabel->setText(tr("Total Host Writes"));
	if (data.HostWrites.empty())
		HostWrites->BaseInfo->setText(tr("----"));
	else
		HostWrites->BaseInfo->setText(QString::fromStdWString(data.HostWrites));

	RotationRate = new DiskBaseInfo(this);
	RotationRate->BaseLabel->setText(tr("Rotation rate"));
	RotationRate->BaseInfo->setText(QString::fromStdWString(data.NominalMediaRotationRate));

	Temperature = new DiskBaseInfo(this);
	Temperature->BaseLabel->setText(tr("Temperature"));
	if(data.Temperature.empty())
		Temperature->BaseInfo->setText(tr("----"));
	else
		Temperature->BaseInfo->setText(QString::fromStdWString(data.Temperature));

	NandWrites = new DiskBaseInfo(this);
	NandWrites->BaseLabel->setText(tr("Nand Writes"));
	if (data.NandWrites.empty())
		NandWrites->BaseInfo->setText(tr("----"));
	else
		NandWrites->BaseInfo->setText(QString::fromStdWString(data.NandWrites));

	BufferSize = new DiskBaseInfo(this);
	BufferSize->BaseLabel->setText(tr("Buffer Size"));
	if (data.BufferSize.empty())
		BufferSize->BaseInfo->setText(tr("----"));
	else
		BufferSize->BaseInfo->setText(QString::fromStdWString(data.BufferSize));

	PowerOnHours = new DiskBaseInfo(this);
	PowerOnHours->BaseLabel->setText(tr("Power On Hours"));
	if (data.PowerOnHours.empty())
		PowerOnHours->BaseInfo->setText(tr("----"));
	else
		PowerOnHours->BaseInfo->setText(QString::fromStdWString(data.PowerOnHours));

	PowerOnCount = new DiskBaseInfo(this);
	PowerOnCount->BaseLabel->setText(tr("Power On Counts"));
	if (data.PowerOnCount.empty())
		PowerOnCount->BaseInfo->setText(tr("----"));
	else
		PowerOnCount->BaseInfo->setText(QString::fromStdWString(data.PowerOnCount));

	horizonallayout_1->addLayout(Firmware->layout);
	horizonallayout_1->addLayout(HostReads->layout);

	horizonallayout_2->addLayout(SerialNumber->layout);
	horizonallayout_2->addLayout(HostWrites->layout);

	horizonallayout_3->addLayout(Interface->layout);
	horizonallayout_3->addLayout(Temperature->layout);

	horizonallayout_4->addLayout(Transfermode->layout);
	horizonallayout_4->addLayout(PowerOnCount->layout);

	horizonallayout_5->addLayout(Driverletter->layout);
	horizonallayout_5->addLayout(PowerOnHours->layout);

	horizonallayout_6->addLayout(BufferSize->layout);
	horizonallayout_6->addLayout(RotationRate->layout);
	horizonallayout_6->addLayout(NandWrites->layout);

	mainlayout->setContentsMargins(0, 0, 6, 0);
	mainlayout->setMargin(0);
	mainlayout->addWidget(DiskName);
	mainlayout->addLayout(horizonallayout_1);
	mainlayout->addLayout(horizonallayout_2);
	mainlayout->addLayout(horizonallayout_3);
	mainlayout->addLayout(horizonallayout_4);
	mainlayout->addLayout(horizonallayout_5);
	mainlayout->addLayout(horizonallayout_6);
	mainlayout->addLayout(Standard->layout);
	mainlayout->addLayout(Features->layout);
}

void DiskInfomationWidget::ReciveDiskInfomation(const SV_ASSIST::Storage::DISK_SMART_INFO& data)
{
	QMutexLocker locker(&this->mutex);
	DiskName->setText(tr("%1 %2").arg(QString::fromStdWString(data.Model)).arg(QString::fromStdWString(data.TotalDiskSize)));
	Firmware->BaseInfo->setText(QString::fromStdWString(data.FirmwareRev));
	SerialNumber->BaseInfo->setText(QString::fromStdWString(data.SerialNumber));
	Interface->BaseInfo->setText(QString::fromStdWString(data.Interface));
	Transfermode->BaseInfo->setText(tr("%1 | %2").arg(QString::fromStdWString(data.CurrentTransferMode)).arg(QString::fromStdWString(data.MaxTransferMode)));
	Driverletter->BaseInfo->setText(QString::fromStdWString(data.LogicalDrive));
	Firmware->BaseInfo->setText(QString::fromStdWString(data.FirmwareRev));
	Standard->BaseInfo->setText(tr("%1 | %2").arg(QString::fromStdWString(data.MinorVersion)).arg(QString::fromStdWString(data.MajorVersion)));
	Features->BaseInfo->setText(QString::fromStdWString(data.Feature));
	RotationRate->BaseInfo->setText(QString::fromStdWString(data.NominalMediaRotationRate));

	if (data.HostReads.empty())
		HostReads->BaseInfo->setText(tr("----"));
	else
		HostReads->BaseInfo->setText(QString::fromStdWString(data.HostReads));

	if (data.HostWrites.empty())
		HostWrites->BaseInfo->setText(tr("----"));
	else
		HostWrites->BaseInfo->setText(QString::fromStdWString(data.HostWrites));

	if (data.Temperature.empty())
		Temperature->BaseInfo->setText(tr("----"));
	else
		Temperature->BaseInfo->setText(QString::fromStdWString(data.Temperature));

	if (data.NandWrites.empty())
		NandWrites->BaseInfo->setText(tr("----"));
	else
		NandWrites->BaseInfo->setText(QString::fromStdWString(data.NandWrites));

	if (data.BufferSize.empty())
		BufferSize->BaseInfo->setText(tr("----"));
	else
		BufferSize->BaseInfo->setText(QString::fromStdWString(data.BufferSize));

	if (data.PowerOnHours.empty())
		PowerOnHours->BaseInfo->setText(tr("----"));
	else
		PowerOnHours->BaseInfo->setText(QString::fromStdWString(data.PowerOnHours));

	if (data.PowerOnCount.empty())
		PowerOnCount->BaseInfo->setText(tr("----"));
	else
		PowerOnCount->BaseInfo->setText(QString::fromStdWString(data.PowerOnCount));
}

DiskInfomationWidget::~DiskInfomationWidget()
{
	delete Firmware;
	delete SerialNumber;
	delete Interface;
	delete Transfermode;
	delete Driverletter;
	delete PowerOnHours;
	delete PowerOnCount;
	delete Temperature;
	delete Standard;
	delete Features;
	delete RotationRate;
	delete HostWrites;
	delete NandWrites;
	delete HostReads;
	delete BufferSize;
}
