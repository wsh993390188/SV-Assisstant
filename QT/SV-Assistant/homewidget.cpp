#include "homewidget.h"
#include "QVector"
#include "lib\Hardware\SV_Hardware.h"

HomebaseInfo::HomebaseInfo(QWidget* father) : BaseLabel(new QLabel(father)), BaseInfo(new QLabel(father)),
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

Homewidget::Homewidget(QWidget *parent) : QWidget(parent), m_verticalLayout(new QVBoxLayout(this))
{
    QPalette pal(this->palette());
    //设置背景white色
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->Init();
}

void Homewidget::Init()
{
	auto smbios = SV_ASSIST::DMI_SMBIOS::GetSMBIOSinfo();
	cpubaseinfo = new HomebaseInfo(this);
	cpubaseinfo->BaseLabel->setText(tr("CPU"));
	cpubaseinfo->BaseInfo->setText(QString::fromStdString(SV_ASSIST::CPU::GetCPUSpecification()));

	{
		motherbroadbaseinfo = new HomebaseInfo(this);
		motherbroadbaseinfo->BaseLabel->setText(tr("Mother Broad"));
		QString temp = {};
		smbios.BaseBroadinfo.count("BaseBroad Manufacturer") > 0 ? temp += QString::fromStdString(smbios.BaseBroadinfo["BaseBroad Manufacturer"]) + tr(" ") : temp = "Unknown MotherBroad";
		smbios.BaseBroadinfo.count("BaseBroad Product") > 0 ? temp += QString::fromStdString(smbios.BaseBroadinfo["BaseBroad Product"]) : temp = "Unknown MotherBroad";
		motherbroadbaseinfo->BaseInfo->setText(temp);
	}

	auto memoryinfo = SV_ASSIST::Memory::GetMemoryGlobalInfomation();
	memorycpubaseinfo = new HomebaseInfo(this);
	memorycpubaseinfo->BaseLabel->setText(tr("Memory"));
	QString memoryInfomation = {};
	{
		QString temp = {};
		size_t memorysize = 0;
		bool state = true;
		for (size_t i = 0; i < memoryinfo.size(); i++)
		{
			if(state)
				memoryInfomation += (QString::fromStdString(memoryinfo.at(i).second.ModuleType) + tr(" "));
			state = false;
			memorysize += memoryinfo.at(i).second.ModuleSize;
			temp = tr("%1 GBytes").arg(memorysize);
		}
		memoryInfomation += temp;
	}

	memorycpubaseinfo->BaseInfo->setText(memoryInfomation);


	gpubaseinfo = new HomebaseInfo(this);
	gpubaseinfo->BaseLabel->setText(tr("GPU"));
	SV_ASSIST::GPU::UpdateDate();
	auto gpu = SV_ASSIST::GPU::GetGpuInfo();
	if (gpu.empty())
		gpubaseinfo->BaseInfo->setText(tr("Unknown GPU"));
	else
		gpubaseinfo->BaseInfo->setText(QString::fromStdString(gpu.at(0).GetGPUName()));

	auto diskinfo = SV_ASSIST::Storage::GetDiskSMARTInfo();
	diskbaseinfo = new HomebaseInfo(this);
	if(diskinfo.empty())
		diskbaseinfo->BaseInfo->setText(tr("None"));
	else
		diskbaseinfo->BaseInfo->setText(QString::fromStdWString(diskinfo.at(0).Model));
	diskbaseinfo->BaseLabel->setText(tr("Storage"));


	SV_ASSIST::Display::UpdateData();

	monitorbaseinfo = new HomebaseInfo(this);
	monitorbaseinfo->BaseLabel->setText(tr("Monitor"));
	monitorbaseinfo->BaseInfo->setText(QString::fromStdString(SV_ASSIST::Display::GetMonitorName()));

	SV_ASSIST::Net::Exec();
	auto net = SV_ASSIST::Net::GetData();
	networkbaseinfo = new HomebaseInfo(this);
	networkbaseinfo->BaseLabel->setText(tr("Network"));
	if (!net.empty())
		for (const auto& i : net)
		{
			if (i.MediaType == L"LAN")
			{
				networkbaseinfo->BaseInfo->setText(QString::fromStdWString(i.DeviceName));
				break;
			}
		}
	else
		networkbaseinfo->BaseInfo->setText(tr("Unknown Network Device"));

	SV_ASSIST::AUDIO::UpdateData();
	auto audio = SV_ASSIST::AUDIO::GetOutputAudio();
	audiobaseinfo = new HomebaseInfo(this);
	audiobaseinfo->BaseLabel->setText(tr("Audio"));
	if (!audio.empty())
		for (const auto& i : audio)
		{
			if (i.state == L"ACTIVE")
			{
				audiobaseinfo->BaseInfo->setText(QString::fromStdWString(i.name));
				break;
			}
		}
    else
        audiobaseinfo->BaseInfo->setText(tr("Unknown Audio Device"));

    m_verticalLayout->setContentsMargins(0,0,0,0);
    m_verticalLayout->setMargin(3);
    m_verticalLayout->addLayout(cpubaseinfo->layout);
    m_verticalLayout->addLayout(motherbroadbaseinfo->layout);
    m_verticalLayout->addLayout(memorycpubaseinfo->layout);
    m_verticalLayout->addLayout(gpubaseinfo->layout);
    m_verticalLayout->addLayout(diskbaseinfo->layout);
    m_verticalLayout->addLayout(monitorbaseinfo->layout);
    m_verticalLayout->addLayout(networkbaseinfo->layout);
    m_verticalLayout->addLayout(audiobaseinfo->layout);
}

Homewidget::~Homewidget()
{
	delete cpubaseinfo;
	delete motherbroadbaseinfo;
	delete memorycpubaseinfo;
	delete gpubaseinfo;
	delete diskbaseinfo;
	delete monitorbaseinfo;
	delete networkbaseinfo;
	delete audiobaseinfo;
}
