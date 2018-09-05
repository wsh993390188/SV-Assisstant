#include "CPUHardwareWidget.h"
#include "lib\Hardware\SV_Hardware.h"

using namespace SV_ASSIST;

CPUHardware_Struct::CPUHardware_Struct(QWidget * parent) : cpulabel(new QLabel(parent)), cpuinfo(new QLabel(parent)),
	layout(new QHBoxLayout()), horizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum)),
	horizontalSpacelast(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum))
{
	cpulabel->setStyleSheet(QObject::tr("QLabel{color:#000000;font-family: \"consolas\"; font-size:12px; background-color: rgb(255, 255, 255);}"));
	cpuinfo->setStyleSheet(QObject::tr("QLabel{color:#000000;font-family: \"consolas\"; font-size:12px; background-color: rgb(255, 255, 255);}"));
	cpulabel->setBaseSize(QSize(20,20));
	cpuinfo->setBaseSize(QSize(20,20));
	cpuinfo->setAlignment(Qt::AlignCenter);
	layout->addWidget(cpulabel);
	layout->addSpacerItem(horizontalSpace);
	layout->addWidget(cpuinfo);
	layout->addSpacerItem(horizontalSpacelast);
}

CPUHardwareWidget::CPUHardwareWidget(QWidget *parent)
	: QWidget(parent), mainlayout(new QVBoxLayout(this)), horizonallayout_1(new QHBoxLayout()),
	horizonallayout_2(new QHBoxLayout()), horizonallayout_3(new QHBoxLayout()), horizonallayout_4(new QHBoxLayout()),
	horizonallayout_5(new QHBoxLayout()), horizonallayout_6(new QHBoxLayout()), horizonallayout_7(new QHBoxLayout()),
	horizonallayout_8(new QHBoxLayout())
{
	QPalette pal(this->palette());
	//ÉèÖÃ±³¾°whiteÉ«
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

CPUHardwareWidget::~CPUHardwareWidget()
{
	delete cpuName;
	delete cpucodename;
	delete cpuMAXTDP;
	delete cpuPackage;
	delete cpuTechnology;
	delete cpuCoreVoltage;
	delete cpuSpecification;
	delete cpuFamily;
	delete cpuExtFamily;
	delete cpuModel;
	delete cpuExtModel;
	delete cpuSteping;
	delete cpuInstructions;
	delete cpuCores;
	delete cpuThreads;
	delete cpuCurrentSpeed;
	delete cpuBusSpeed;
	delete cpuMultiplier;
}

void CPUHardwareWidget::Init()
{
	SUPERIO::UpdateData();
	cpuName = new CPUHardware_Struct(this);
	cpuName->cpulabel->setText(tr("Name"));
	cpuName->cpuinfo->setText(QString::fromStdString(CPU::GetCPUName()));

	cpucodename = new CPUHardware_Struct(this);
	cpucodename->cpulabel->setText(tr("Code Name"));
	cpucodename->cpuinfo->setText(QString::fromStdString(CPU::GetCodeName()));

	cpuMAXTDP = new CPUHardware_Struct(this);
	cpuMAXTDP->cpulabel->setText(tr("Max TDP"));
	cpuMAXTDP->cpuinfo->setText(QString::number(CPU::GetMaxTDP()) + tr(" W"));
	horizonallayout_1->addLayout(cpucodename->layout);
	horizonallayout_1->addLayout(cpuMAXTDP->layout);

	cpuPackage = new CPUHardware_Struct(this);
	cpuPackage->cpulabel->setText(tr("Package"));
	cpuPackage->cpuinfo->setText(QString::fromStdString(CPU::GetSocketDesignation()));

	cpuTechnology = new CPUHardware_Struct(this);
	cpuTechnology->cpulabel->setText(tr("Technology"));
	cpuTechnology->cpuinfo->setText(QString::number(CPU::GetTechnology()) + tr(" nm"));

	cpuCoreVoltage = new CPUHardware_Struct(this);
	cpuCoreVoltage->cpulabel->setText(tr("Core Voltage"));
	cpuCoreVoltage->cpuinfo->setText(QString::number(CPU::GetCpuVCore(Ring0::GetPCIVendorID())) + tr(" V"));

	cpuSpecification = new CPUHardware_Struct(this);
	cpuSpecification->cpulabel->setText(tr("Specification"));
	cpuSpecification->cpuinfo->setText(QString::fromStdString(CPU::GetCPUSpecification()));

	cpuFamily = new CPUHardware_Struct(this);
	cpuFamily->cpulabel->setText(tr("Family"));
	cpuFamily->cpuinfo->setText(QString::number(CPU::GetFamily(), 16).toUpper());

	cpuExtFamily = new CPUHardware_Struct(this);
	cpuExtFamily->cpulabel->setText(tr("Ext.Family"));
	cpuExtFamily->cpuinfo->setText(QString::number(CPU::GetExtFamily(), 16).toUpper());

	cpuModel = new CPUHardware_Struct(this);
	cpuModel->cpulabel->setText(tr("Model"));
	cpuModel->cpuinfo->setText(QString::number(CPU::GetModel(), 16).toUpper());

	cpuExtModel = new CPUHardware_Struct(this);
	cpuExtModel->cpulabel->setText(tr("Ext.Model"));
	cpuExtModel->cpuinfo->setText(QString::number(CPU::GetExtModel(), 16).toUpper());

	cpuSteping = new CPUHardware_Struct(this);
	cpuSteping->cpulabel->setText(tr("Steping"));
	cpuSteping->cpuinfo->setText(QString::number(CPU::GetStepping(), 16).toUpper());

	cpuInstructions = new CPUHardware_Struct(this);
	cpuInstructions->cpulabel->setText(tr("Instructions"));
	cpuInstructions->cpuinfo->setText(ExecInstructions());

	cpuCores = new CPUHardware_Struct(this);
	cpuCores->cpulabel->setText(tr("Cores"));
	cpuCores->cpuinfo->setText(QString::number(CPU::GetCore()));

	cpuThreads = new CPUHardware_Struct(this);
	cpuThreads->cpulabel->setText(tr("Threads"));
	cpuThreads->cpuinfo->setText(QString::number(CPU::GetThread()));

	cpuCurrentSpeed = new CPUHardware_Struct(this);
	cpuCurrentSpeed->cpulabel->setText(tr("Current Speed"));
	cpuCurrentSpeed->cpuinfo->setText(QString::number(CPU::GetCurrentClockSpeed().at(0)) + tr(" MHZ"));

	cpuBusSpeed = new CPUHardware_Struct(this);
	cpuBusSpeed->cpulabel->setText(tr("Bus Speed"));
	cpuBusSpeed->cpuinfo->setText(QString::number(CPU::GetExtClock()) + tr(" MHZ"));

	cpuMultiplier = new CPUHardware_Struct(this);
	cpuMultiplier->cpulabel->setText(tr("Multiplier"));
	cpuMultiplier->cpuinfo->setText(QString::number((size_t)(CPU::GetCurrentClockSpeed().at(0) / CPU::GetExtClock())) + tr("x"));

	auto cache = CPU::GetCache();
	cpucacheL1D = new CPUHardware_Struct(this);
	cpucacheL1D->cpulabel->setText(tr("Cache Level1 Data"));
	if (cache[0].Cache_Size / 1024 > 1024)
		cpucacheL1D->cpuinfo->setText(tr("%1 MB %2-Ways").arg(QString::number(cache[0].Cache_Size / 1024 / 1024, 10)).arg(QString::number(cache[0].Cache_Ways)));
	else
		cpucacheL1D->cpuinfo->setText(tr("%1 KB %2-Ways").arg(QString::number(cache[0].Cache_Size / 1024, 10)).arg(QString::number(cache[0].Cache_Ways)));

	cpucacheL1T = new CPUHardware_Struct(this);
	cpucacheL1T->cpulabel->setText(tr("Cache Level1 Inst."));
	if (cache[1].Cache_Size / 1024 > 1024)
		cpucacheL1T->cpuinfo->setText(tr("%1 MB %2-Ways").arg(QString::number(cache[1].Cache_Size / 1024 / 1024, 10)).arg(QString::number(cache[1].Cache_Ways)));
	else
		cpucacheL1T->cpuinfo->setText(tr("%1 KB %2-Ways").arg(QString::number(cache[1].Cache_Size / 1024, 10)).arg(QString::number(cache[1].Cache_Ways)));

	cpucacheL2 = new CPUHardware_Struct(this);
	cpucacheL2->cpulabel->setText(tr("Cache Level2"));
	if (cache[2].Cache_Size / 1024 > 1024)
		cpucacheL2->cpuinfo->setText(tr("%1 MB %2-Ways").arg(QString::number(cache[2].Cache_Size / 1024 / 1024, 10)).arg(QString::number(cache[2].Cache_Ways)));
	else
		cpucacheL2->cpuinfo->setText(tr("%1 KB %2-Ways").arg(QString::number(cache[2].Cache_Size / 1024, 10)).arg(QString::number(cache[2].Cache_Ways)));

	cpucacheL3 = new CPUHardware_Struct(this);
	cpucacheL3->cpulabel->setText(tr("Cache Level3"));
	if (cache[3].Cache_Size > 1024)
	if (cache[3].Cache_Size / 1024 > 1024)
		cpucacheL3->cpuinfo->setText(tr("%1 MB %2-Ways").arg(QString::number(cache[3].Cache_Size / 1024 / 1024, 10)).arg(QString::number(cache[3].Cache_Ways)));
	else
		cpucacheL3->cpuinfo->setText(tr("%1 KB %2-Ways").arg(QString::number(cache[3].Cache_Size / 1024, 10)).arg(QString::number(cache[3].Cache_Ways)));

	horizonallayout_2->addLayout(cpuFamily->layout);
	horizonallayout_2->addLayout(cpuModel->layout);
	horizonallayout_2->addLayout(cpuSteping->layout);

	horizonallayout_3->addLayout(cpuExtFamily->layout);
	horizonallayout_3->addLayout(cpuExtModel->layout);
	horizonallayout_3->addLayout(cpuTechnology->layout);
	
	horizonallayout_4->addLayout(cpuCores->layout);
	horizonallayout_4->addLayout(cpuThreads->layout);
	horizonallayout_4->addLayout(cpuPackage->layout);

	horizonallayout_5->addLayout(cpucacheL1D->layout);
	horizonallayout_5->addLayout(cpuCoreVoltage->layout);

	horizonallayout_6->addLayout(cpucacheL1T->layout);
	horizonallayout_6->addLayout(cpuCurrentSpeed->layout);

	horizonallayout_7->addLayout(cpucacheL2->layout);
	horizonallayout_7->addLayout(cpuBusSpeed->layout);

	horizonallayout_8->addLayout(cpucacheL3->layout);
	horizonallayout_8->addLayout(cpuMultiplier->layout);

	mainlayout->setContentsMargins(0, 0, 6, 0);
	mainlayout->setMargin(0);
	mainlayout->addLayout(cpuName->layout);
	mainlayout->addLayout(horizonallayout_1);
	mainlayout->addLayout(horizonallayout_2);
	mainlayout->addLayout(horizonallayout_3);
	mainlayout->addLayout(horizonallayout_4);
	mainlayout->addLayout(cpuSpecification->layout);
	mainlayout->addLayout(cpuInstructions->layout);
	mainlayout->addLayout(horizonallayout_5);
	mainlayout->addLayout(horizonallayout_6);
	mainlayout->addLayout(horizonallayout_7);
	mainlayout->addLayout(horizonallayout_8);

	timeID = this->startTimer(1000);
}

void CPUHardwareWidget::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == timeID && this->isVisible())
	{
		this->UpdateData();
	}
}

void CPUHardwareWidget::UpdateData()
{
	SV_ASSIST::CPU::Updatedata();
	SV_ASSIST::SUPERIO::UpdateData();
	cpuCoreVoltage->cpuinfo->setText(QString::number(CPU::GetCpuVCore(Ring0::GetPCIVendorID())) + tr(" V"));
	cpuMultiplier->cpuinfo->setText(QString::number((size_t)(CPU::GetCurrentClockSpeed().at(0) / CPU::GetExtClock())) + tr("x"));
	cpuCurrentSpeed->cpuinfo->setText(QString::number(CPU::GetCurrentClockSpeed().at(0)) + tr(" MHZ"));
	cpuBusSpeed->cpuinfo->setText(QString::number(CPU::GetExtClock()) + tr(" MHZ"));
}

const QString CPUHardwareWidget::ExecInstructions()
{
	auto feature = CPU::GetFeature();
	QString re = {};
	if (CPU::GetManufacturer() == "GenuineIntel")
	{
		if (feature.IntelFeature.MMX)		re += "MMX ";
		if (feature.IntelFeature.SSE)		re += "SSE ";
		if (feature.IntelFeature.SSE2)		re += "SSE2 ";
		if (feature.IntelFeature.SSE3)		re += "SSE3 ";
		if (feature.IntelFeature.SSSE3)		re += "SSSE3 ";
		if (feature.IntelFeature.SSE4_1)	re += "SSE4.1 ";
		if (feature.IntelFeature.SSE4_2)	re += "SSE4.2 ";
		if (feature.IntelFeature.AVX)		re += "AVX ";
		if (feature.IntelFeature.AVX2)		re += "AVX2 ";
		if (feature.IntelFeature.AESNI)		re += "AES ";
		if (feature.IntelFeature.FMA)		re += "FMA ";
	}
	else if (CPU::GetManufacturer() == "AuthenticAMD")
	{
		if (feature.AMDFeature.MMX)				re += "MMX ";
		if (feature.AMDFeature.SSE)				re += "SSE ";
		if (feature.AMDFeature.SSE2)			re += "SSE2 ";
		if (feature.AMDFeature.SSE3)			re += "SSE3 ";
		if (feature.AMDFeature.SSSE3)			re += "SSSE3 ";
		if (feature.AMDFeature.SSE4A)			re += "SSE4A ";
		if (feature.AMDFeature.AVX)				re += "AVX ";
		if (feature.AMDFeature._3DNowPrefetch)	re += "3DNowPrefetch ";
		if (feature.AMDFeature._3DNowExt)		re += "3D!NowExt. ";
		if (feature.AMDFeature._3DNow)			re += "3D!Now ";
	}
	else if (CPU::GetManufacturer() == "CentaurHauls")
	{
		if (feature.ZhaoxinFeature.MMX)		re += "MMX ";
		if (feature.ZhaoxinFeature.SSE)		re += "SSE ";
		if (feature.ZhaoxinFeature.SSE2)	re += "SSE2 ";
		if (feature.ZhaoxinFeature.SSE3)	re += "SSE3 ";
		if (feature.ZhaoxinFeature.SSSE3)	re += "SSSE3 ";
		if (feature.ZhaoxinFeature.SSE4_1)	re += "SSE4.1 ";
		if (feature.ZhaoxinFeature.SSE4_2)	re += "SSE4.2 ";
		if (feature.ZhaoxinFeature.SSE4_2)	re += "SSE4.2 ";
	}
	return re;

}


