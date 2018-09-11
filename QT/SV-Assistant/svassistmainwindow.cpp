#include "svassistmainwindow.h"
#include "ui_svassistmainwindow.h"
#include "Windows.h"

class CCPUUseRate
{
public:
	bool Initialize()
	{
		FILETIME ftIdle, ftKernel, ftUser;
		bool flag = FALSE;
		if (flag = GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
		{
			m_fOldCPUIdleTime = FileTimeToDouble(ftIdle);
			m_fOldCPUKernelTime = FileTimeToDouble(ftKernel);
			m_fOldCPUUserTime = FileTimeToDouble(ftUser);

		}
		return flag;
	}
	//调用Initialize后要等待1秒再调用此函数
	double GetCPUUseRate()
	{
		double nCPUUseRate = 0.0;
		FILETIME ftIdle, ftKernel, ftUser;
		if (GetSystemTimes(&ftIdle, &ftKernel, &ftUser))
		{
			double fCPUIdleTime = FileTimeToDouble(ftIdle);
			double fCPUKernelTime = FileTimeToDouble(ftKernel);
			double fCPUUserTime = FileTimeToDouble(ftUser);
			nCPUUseRate = (100.0 - (fCPUIdleTime - m_fOldCPUIdleTime)
				/ (fCPUKernelTime - m_fOldCPUKernelTime + fCPUUserTime - m_fOldCPUUserTime)
				*100.0);
			m_fOldCPUIdleTime = fCPUIdleTime;
			m_fOldCPUKernelTime = fCPUKernelTime;
			m_fOldCPUUserTime = fCPUUserTime;
		}
		return nCPUUseRate;
	}

	static CCPUUseRate* Instance()
	{
		if (!self)
			self = new CCPUUseRate();
		return self;
	}
private:
	double FileTimeToDouble(FILETIME &filetime)
	{
		return (double)(filetime.dwHighDateTime * 4.294967296E9) + (double)filetime.dwLowDateTime;
	}
private:
	double m_fOldCPUIdleTime;
	double m_fOldCPUKernelTime;
	double m_fOldCPUUserTime;
	static CCPUUseRate *self;
};

SVAssistMainWindow::SVAssistMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SVAssistMainWindow)
{
    ui->setupUi(this);
    this->InitWindow();
}

void SVAssistMainWindow::InitWindow()
{
	CCPUUseRate::Instance()->Initialize();
	ui->Networksplitter->setStretchFactor(0, 1);
	ui->Networksplitter->setStretchFactor(1, 3);
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    connect(ui->minButton, &QToolButton::clicked, this, [&](bool state){ if(!state) showMinimized();});
    connect(ui->HometoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(0); });
    connect(ui->HardwareViewtoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(1); });
    connect(ui->HardwareManagetoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(2); });
    connect(ui->SoftwareViewtoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(3); });
    connect(ui->SoftwareManagetoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(4); });
    connect(ui->GameTesttoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(5); });
	connect(ui->NetworklistWidget, &NetworkListWidget::SendNetStruct, ui->NetworktextBrowser, &NetworkTableWidget::ReceiveNetdata);
	connect(ui->HardwareTabwidget, &HardwareTab::SendHardwareTabIndex, this, [&](int Index) {ui->HardwareInfomationstackedWidget->setCurrentIndex(Index); });
	connect(ui->HardwareMasterTabWidget, &HardwareMasterTab::SendHardwareTabIndex, this, [&](int Index) {ui->HardwareMasterStackedWidget->setCurrentIndex(Index); });
	connect(ui->TitleStackedWidget, &Homewidget::switchPage, this, [&](int Pages) { ui->BottomstackedWidget->setCurrentIndex(Pages); });
	connect(ui->IOBasetoolButton, &IOButton::SendBaseAddr, ui->IoBasetableView, [&](unsigned int addr) { ui->IoBasetableView->SetBaseAddr(addr); });
	CPUUsageID = this->startTimer(1000);
}

SVAssistMainWindow::~SVAssistMainWindow()
{
    delete ui;
}

void SVAssistMainWindow::timerEvent(QTimerEvent * event)
{
	if (event->timerId() == CPUUsageID )
	{
		if (ui->CPUUsagewidget->isVisible())
			this->UpdateUsages();
		else
			ui->CPUUsagewidget->clearData();
	}
}

void SVAssistMainWindow::UpdateUsages()
{
	auto Usage = CCPUUseRate::Instance()->GetCPUUseRate();
	ui->CPUUsagewidget->addData(Usage);
}

CCPUUseRate* CCPUUseRate::self = nullptr;
