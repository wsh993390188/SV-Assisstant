#include "GPUWidget.h"
#include "lib\Hardware\SV_Hardware.h"

GPUWidget::GPUWidget(QWidget *parent)
	: QWidget(parent), mainlayout(new QVBoxLayout(this))
{
	QPalette pal(this->palette());
	//ÉèÖÃ±³¾°whiteÉ«
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
	gpuTimeID = this->startTimer(1000);
}

void GPUWidget::Init()
{
	allGPUName = new QComboBox(this);
	allGPUName->setStyleSheet("QComboBox{color:#000000;font-family: \"consolas\"; font-size:14px;background-color: rgb(255, 255, 255);}");
	SV_ASSIST::GPU::UpdateDate();
	for (const auto& i : SV_ASSIST::GPU::GetGpuInfo())
	{
		this->BaseInfo.push_back(i);
		allGPUName->addItem(QString::fromStdString(i.first.GPUname));
	}

	if(!BaseInfo.empty())
	{
		gpuname = new GpuGUIBaseInfo(this);
		gpuname->BaseLabel->setText(tr("Name"));
		gpuname->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).first.GPUname));

		gpubios = new GpuGUIBaseInfo(this);
		gpubios->BaseLabel->setText(tr("BIOS Version"));
		gpubios->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).first.GPUBiosVersion));

		gpuDriver = new GpuGUIBaseInfo(this);
		gpuDriver->BaseLabel->setText(tr("Driver Version"));
		gpuDriver->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).first.GPUDriverVersion));

		gpuTemperature = new GpuGUIBaseInfo(this);
		gpuTemperature->BaseLabel->setText(tr("Temperature"));
		gpuTemperature->BaseInfo->setText(QString::fromStdWString(BaseInfo.at(0).second.Temperature));

		gpuClock = new GpuGUIBaseInfo(this);
		gpuClock->BaseLabel->setText(tr("GPU Clock"));
		gpuClock->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).second.GPUClock));

		gpuMemoryClock = new GpuGUIBaseInfo(this);
		gpuMemoryClock->BaseLabel->setText(tr("Memory Clock"));
		gpuMemoryClock->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).second.GPUMemClock));

		gpuUsage = new GpuGUIBaseInfo(this);
		gpuUsage->BaseLabel->setText(tr("GPU Usage"));
		gpuUsage->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).second.GPUUsage));

		gpuFans = new GpuGUIBaseInfo(this);
		gpuFans->BaseLabel->setText(tr("GPU Fans"));
		gpuFans->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).second.fans));

		gpuMemory = new GpuGUIBaseInfo(this);
		gpuMemory->BaseLabel->setText(tr("GPU Memory"));
		gpuMemory->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).first.GPUMemorySize));

		gpuSharedMemory = new GpuGUIBaseInfo(this);
		gpuSharedMemory->BaseLabel->setText(tr("Shared memory"));		
		gpuSharedMemory->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).first.GPUSharedMemorySize));

		mainlayout->addLayout(gpuname->layout);
		mainlayout->addLayout(gpubios->layout);
		mainlayout->addLayout(gpuDriver->layout);
		mainlayout->addLayout(gpuTemperature->layout);
		mainlayout->addLayout(gpuClock->layout);
		mainlayout->addLayout(gpuMemoryClock->layout);
		mainlayout->addLayout(gpuUsage->layout);
		mainlayout->addLayout(gpuFans->layout);
		mainlayout->addLayout(gpuMemory->layout);
		mainlayout->addLayout(gpuSharedMemory->layout);
		mainlayout->addWidget(this->allGPUName);
		connect(this->allGPUName, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, static_cast<void (GPUWidget::*)(int)>(&GPUWidget::Updatedata));
	}
}

void GPUWidget::Updatedata()
{
	QMutexLocker locker(&this->mutex);
	SV_ASSIST::GPU::UpdateDate();
	BaseInfo.clear();
	for (const auto& i : SV_ASSIST::GPU::GetGpuInfo())
	{
		this->BaseInfo.push_back(i);
	}
}

void GPUWidget::Updatedata(int Index)
{
	if (!BaseInfo.empty() && Index < BaseInfo.size())
	{
		QMutexLocker locker(&this->mutex);
		gpuname->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).first.GPUname));
		gpubios->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).first.GPUBiosVersion));
		gpuDriver->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).first.GPUDriverVersion));
		gpuTemperature->BaseInfo->setText(QString::fromStdWString(BaseInfo.at(Index).second.Temperature));
		gpuClock->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).second.GPUClock));
		gpuMemoryClock->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).second.GPUMemClock));
		gpuUsage->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).second.GPUUsage));
		gpuFans->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).second.fans));
		gpuMemory->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).first.GPUMemorySize));
		gpuSharedMemory->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).first.GPUSharedMemorySize));
	}
}


GpuGUIBaseInfo::GpuGUIBaseInfo(QWidget* father) : BaseLabel(new QLabel(father)), BaseInfo(new QLabel(father)),
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

GPUWidget::~GPUWidget()
{
	delete gpuname;
	delete gpubios;
	delete gpuDriver;
	delete gpuTemperature;
	delete gpuClock;
	delete gpuMemoryClock;
	delete gpuUsage;
	delete gpuFans;
	delete gpuMemory;
	delete gpuSharedMemory;
}

void GPUWidget::timerEvent(QTimerEvent * event)
{
	if (event->timerId() == gpuTimeID && this->isVisible())
	{
		this->Updatedata();
		this->Updatedata(this->allGPUName->currentIndex());
	}
}
