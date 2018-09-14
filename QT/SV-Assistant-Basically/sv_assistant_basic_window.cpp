#include "sv_assistant_basic_window.h"
#include "ui_sv_assistant_basic_window.h"

SV_ASSISTANT_BASIC_WINDOW::SV_ASSISTANT_BASIC_WINDOW(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SVAssistMainWindow)
{
    ui->setupUi(this);
    this->InitWindows();
}

void SV_ASSISTANT_BASIC_WINDOW::InitWindows()
{
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    connect(ui->minButton, &QToolButton::clicked, this, [&](bool state){ if(!state) showMinimized();});
    connect(ui->HometoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(0); });
    connect(ui->HardwareViewtoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(1); });
    connect(ui->SoftwareViewtoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(2); });
    connect(ui->GameTesttoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(3); });
    connect(ui->NetworklistWidget, &NetworkListWidget::SendNetStruct, ui->NetworktextBrowser, &NetworkTableWidget::ReceiveNetdata);
    connect(ui->HardwareTabwidget, &HardwareTab::SendHardwareTabIndex, this, [&](int Index) {ui->HardwareInfomationstackedWidget->setCurrentIndex(Index); });
	connect(ui->usbtreeWidget, &usbtreewidget::sendUsbInfo, ui->usbtextBrowser, &usbtestbrowser::ReviceUsbInfo);
	connect(this, &SV_ASSISTANT_BASIC_WINDOW::MayUsbDeviceChanged, ui->usbtreeWidget, &usbtreewidget::DEVICECHANGE);
}

bool SV_ASSISTANT_BASIC_WINDOW::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	Q_UNUSED(eventType)
	MSG *msg = static_cast<MSG *>(message);
	if (msg->message == WM_DEVICECHANGE)
	{
		switch (msg->wParam)
		{
		case 0x8000: //DBT_DEVICEARRIVAL
		case 0x8004: //DBT_DEVICEREMOVECOMPLETE
			emit MayUsbDeviceChanged();
			break;
		default:
			break;
		}

	}
	return QMainWindow::nativeEvent("", message, result);
}

SV_ASSISTANT_BASIC_WINDOW::~SV_ASSISTANT_BASIC_WINDOW()
{
    delete ui;
}
