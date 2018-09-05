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
		allGPUName->addItem(QString::fromStdString(i.GPUname));
	}

	if(!BaseInfo.empty())
	{
		gpuname = new GpuGUIBaseInfo(this);
		gpuname->BaseLabel->setText(tr("Name"));
		gpuname->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).GPUname));

		gpubios = new GpuGUIBaseInfo(this);
		gpubios->BaseLabel->setText(tr("BIOS Version"));
		gpubios->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).GPUBiosVersion));

		gpuDriver = new GpuGUIBaseInfo(this);
		gpuDriver->BaseLabel->setText(tr("Driver Version"));
		gpuDriver->BaseInfo->setText(QString::fromStdString(BaseInfo.at(0).GPUDriverVersion));

		gpuTemperature = new GpuGUIBaseInfo(this);
		gpuTemperature->BaseLabel->setText(tr("Temperature"));
		gpuTemperature->BaseInfo->setText(tr("%1 ¡ãC").arg(QString::number(BaseInfo.at(0).Temperature)));

		gpuClock = new GpuGUIBaseInfo(this);
		gpuClock->BaseLabel->setText(tr("GPU Clock"));
		gpuClock->BaseInfo->setText(tr("%1 Mhz").arg(QString::number(BaseInfo.at(0).GPUClock)));

		gpuMemoryClock = new GpuGUIBaseInfo(this);
		gpuMemoryClock->BaseLabel->setText(tr("Memory Clock"));
		gpuMemoryClock->BaseInfo->setText(tr("%1 Mhz").arg(QString::number(BaseInfo.at(0).GPUMemclock)));

		gpuUsage = new GpuGUIBaseInfo(this);
		gpuUsage->BaseLabel->setText(tr("GPU Usage"));
		gpuUsage->BaseInfo->setText(tr("%1 %").arg(QString::number(BaseInfo.at(0).GPUusage)));

		gpuFans = new GpuGUIBaseInfo(this);
		gpuFans->BaseLabel->setText(tr("GPU Fans"));
		gpuFans->BaseInfo->setText(tr("%1 RPM").arg(QString::number(BaseInfo.at(0).fans)));

		gpuMemory = new GpuGUIBaseInfo(this);
		gpuMemory->BaseLabel->setText(tr("GPU Memory"));
		if(BaseInfo.at(0).dedicatedVideoMemory > 1024)
			if(BaseInfo.at(0).dedicatedVideoMemory / 1024 > 1024)
				if(BaseInfo.at(0).dedicatedVideoMemory / 1024 / 1024 > 1024)
					gpuMemory->BaseInfo->setText(tr("%1 GB").arg(QString::number(BaseInfo.at(0).dedicatedVideoMemory / 1024 / 1024)));
				else
					gpuMemory->BaseInfo->setText(tr("%1 MB").arg(QString::number(BaseInfo.at(0).dedicatedVideoMemory / 1024)));
			else
				gpuMemory->BaseInfo->setText(tr("%1 KB").arg(QString::number(BaseInfo.at(0).dedicatedVideoMemory)));

		gpuSharedMemory = new GpuGUIBaseInfo(this);
		gpuSharedMemory->BaseLabel->setText(tr("Shared memory"));		
		if (BaseInfo.at(0).sharedSystemMemory > 1024)
			if (BaseInfo.at(0).sharedSystemMemory / 1024 > 1024)
				if (BaseInfo.at(0).sharedSystemMemory / 1024 / 1024 > 1024)
					gpuSharedMemory->BaseInfo->setText(tr("%1 GB").arg(QString::number(BaseInfo.at(0).sharedSystemMemory / 1024 / 1024)));
				else
					gpuSharedMemory->BaseInfo->setText(tr("%1 MB").arg(QString::number(BaseInfo.at(0).sharedSystemMemory / 1024)));
			else
				gpuSharedMemory->BaseInfo->setText(tr("%1 KB").arg(QString::number(BaseInfo.at(0).sharedSystemMemory)));

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
		gpuname->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).GPUname));
		gpubios->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).GPUBiosVersion));
		gpuDriver->BaseInfo->setText(QString::fromStdString(BaseInfo.at(Index).GPUDriverVersion));
		gpuTemperature->BaseInfo->setText(tr("%1 ¡ãC").arg(QString::number(BaseInfo.at(Index).Temperature)));
		gpuClock->BaseInfo->setText(tr("%1 Mhz").arg(QString::number(BaseInfo.at(Index).GPUClock)));
		gpuMemoryClock->BaseInfo->setText(tr("%1 Mhz").arg(QString::number(BaseInfo.at(Index).GPUMemclock)));
		gpuUsage->BaseInfo->setText(tr("%1 %").arg(QString::number(BaseInfo.at(Index).GPUusage)));
		gpuFans->BaseInfo->setText(tr("%1 RPM").arg(QString::number(BaseInfo.at(Index).fans)));
		gpuMemory->BaseLabel->setText(tr("GPU Memory"));
		if (BaseInfo.at(0).dedicatedVideoMemory > 1024)
			if (BaseInfo.at(0).dedicatedVideoMemory / 1024 > 1024)
				if (BaseInfo.at(0).dedicatedVideoMemory / 1024 / 1024 > 1024)
					gpuMemory->BaseInfo->setText(tr("%1 GB").arg(QString::number(BaseInfo.at(Index).dedicatedVideoMemory / 1024 / 1024)));
				else
					gpuMemory->BaseInfo->setText(tr("%1 MB").arg(QString::number(BaseInfo.at(Index).dedicatedVideoMemory / 1024)));
			else
				gpuMemory->BaseInfo->setText(tr("%1 KB").arg(QString::number(BaseInfo.at(Index).dedicatedVideoMemory)));
		if (BaseInfo.at(0).sharedSystemMemory > 1024)
			if (BaseInfo.at(0).sharedSystemMemory / 1024 > 1024)
				if (BaseInfo.at(0).sharedSystemMemory / 1024 / 1024 > 1024)
					gpuSharedMemory->BaseInfo->setText(tr("%1 GB").arg(QString::number(BaseInfo.at(Index).sharedSystemMemory / 1024 / 1024)));
				else
					gpuSharedMemory->BaseInfo->setText(tr("%1 MB").arg(QString::number(BaseInfo.at(Index).sharedSystemMemory / 1024)));
			else
				gpuSharedMemory->BaseInfo->setText(tr("%1 KB").arg(QString::number(BaseInfo.at(Index).sharedSystemMemory)));
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
