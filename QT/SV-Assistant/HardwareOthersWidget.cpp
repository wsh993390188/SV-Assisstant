#include "HardwareOthersWidget.h"
#include "lib\Hardware\SV_Hardware.h"

HardwareOthersWidget::HardwareOthersWidget(QWidget *parent)
	: QWidget(parent), mainlayout(new QVBoxLayout(this))
{
	QPalette pal(this->palette());
	//ÉèÖÃ±³¾°whiteÉ«
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

HardwareOthersWidget::~HardwareOthersWidget()
{
}

void HardwareOthersWidget::Init()
{
	SV_ASSIST::USB::UpdateData();
	auto usbdev = SV_ASSIST::USB::GetAllUsbDeviceInfo();
	for (const auto&i : usbdev)
	{
		HardwareOthersBaseInfo * temp = new HardwareOthersBaseInfo(this);
		temp->BaseLabel->setText(QString::fromStdWString(i.DeviceDescName));
		mainlayout->addLayout(temp->layout);
		devs.push_back(temp);
	}
}

HardwareOthersBaseInfo::HardwareOthersBaseInfo(QWidget * father) : BaseLabel(new QLabel(father)), 
horizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum)), layout(new QHBoxLayout()),
BeginhorizontalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum))
{
	BaseLabel->setStyleSheet(QObject::tr("QLabel{color:#000000;font-family: \"consolas\"; font-size:14px;background-color: rgb(255, 255, 255);}"));
	layout->addItem(BeginhorizontalSpace);
	layout->addWidget(BaseLabel);
	layout->addItem(horizontalSpace);
}
