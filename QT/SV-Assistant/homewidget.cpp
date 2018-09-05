#include "homewidget.h"
#include "QVector"
#include "lib\Hardware\SV_Hardware.h"

HomebaseInfo::HomebaseInfo(QWidget* father) : BaseLabel(new QPushButton(father)), BaseInfo(new QLabel(father)),
    horizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum)), layout(new QHBoxLayout()),
    BeginhorizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum)),
	EndhorizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum))
{
    BaseLabel->setStyleSheet("QPushButton{color:#000000;border-style: outset; border-width: 2px; border-radius: 5px;border-color: #8B7355;font-family: \"consolas\"; font-size:14px;background-color: rgb(255, 255, 255);padding: 5px;}\
		QPushButton:hover{ \
		background - color: rgb(0, 150, 0); \
		}\
		QPushButton : pressed{ \
		background - color: #1E90FF; \
		border - style: inset; \
		}\
		QPushButton : !enabled{ \
		background - color: rgb(100, 100, 100); \
		border - style: inset; \
		}");
	BaseLabel->setMinimumSize(100, 50);
	BaseLabel->setMaximumSize(150, 100);
    BaseInfo->setStyleSheet(QObject::tr("QLabel{color:#000000;font-family: \"consolas\"; font-size:13px;background-color: rgb(255, 255, 255);}"));
	BaseInfo->setAlignment(Qt::AlignCenter);
    layout->addItem(BeginhorizontalSpace);
    layout->addWidget(BaseLabel);
    layout->addItem(horizontalSpace);
    layout->addWidget(BaseInfo);
	layout->addItem(EndhorizontalSpace);
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
		connect(motherbroadbaseinfo->BaseLabel, &QPushButton::clicked, this, [=] {emit switchPage(1); });
	}

	connect(cpubaseinfo->BaseLabel, &QPushButton::clicked, this, [=] {emit switchPage(0); });

	auto memoryinfo = SV_ASSIST::Memory::GetMemoryGlobalInfomation();
	memorybaseinfo = new HomebaseInfo(this);
	memorybaseinfo->BaseLabel->setText(tr("Memory"));
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

	memorybaseinfo->BaseInfo->setText(memoryInfomation);
	connect(memorybaseinfo->BaseLabel, &QPushButton::clicked, this, [=] {emit switchPage(2); });

	gpubaseinfo = new HomebaseInfo(this);
	gpubaseinfo->BaseLabel->setText(tr("GPU"));
	SV_ASSIST::GPU::UpdateDate();
	auto gpu = SV_ASSIST::GPU::GetGpuInfo();
	if (gpu.empty())
		gpubaseinfo->BaseInfo->setText(tr("Unknown GPU"));
	else
		gpubaseinfo->BaseInfo->setText(QString::fromStdString(gpu.at(0).GPUname));
	connect(gpubaseinfo->BaseLabel, &QPushButton::clicked, this, [=] {emit switchPage(3); });

	auto diskinfo = SV_ASSIST::Storage::GetDiskSMARTInfo();
	diskbaseinfo = new HomebaseInfo(this);
	if(diskinfo.empty())
		diskbaseinfo->BaseInfo->setText(tr("None"));
	else
		diskbaseinfo->BaseInfo->setText(QString::fromStdWString(diskinfo.at(0).Model));
	diskbaseinfo->BaseLabel->setText(tr("Storage"));
	connect(diskbaseinfo->BaseLabel, &QPushButton::clicked, this, [=] {emit switchPage(4); });

	monitorbaseinfo = new HomebaseInfo(this);
	monitorbaseinfo->BaseLabel->setText(tr("Monitor"));
	monitorbaseinfo->BaseInfo->setText(QString::fromStdString(SV_ASSIST::Display::GetMonitorName()));
	connect(monitorbaseinfo->BaseLabel, &QPushButton::clicked, this, [=] {emit switchPage(5); });

	auto net = SV_ASSIST::Net::GetData();
	networkbaseinfo = new HomebaseInfo(this);
	networkbaseinfo->BaseLabel->setText(tr("Network"));
	if (!net.empty())
		for (const auto& i : net)
		{
			if (i.state == L"CONNECTED")
			{
				networkbaseinfo->BaseInfo->setText(QString::fromStdWString(i.DeviceName));
				break;
			}
		}
	else
		networkbaseinfo->BaseInfo->setText(tr("Unknown Network Device"));
	connect(networkbaseinfo->BaseLabel, &QPushButton::clicked, this, [=] {emit switchPage(6); });
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
    m_verticalLayout->addLayout(memorybaseinfo->layout);
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
	delete memorybaseinfo;
	delete gpubaseinfo;
	delete diskbaseinfo;
	delete monitorbaseinfo;
	delete networkbaseinfo;
	delete audiobaseinfo;
}
