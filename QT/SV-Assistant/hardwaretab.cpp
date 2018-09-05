#include "hardwaretab.h"

HardwareButton::HardwareButton(QWidget *parent) : QToolButton(parent), Changecolor(true)
{
    this->setStyleSheet("QToolButton {border:none;color:#000000;font-family: \"consolas\"; font-size:14px;background-color: #FFFFFF;}");
    this->setMouseTracking(true);
    this->installEventFilter(this);
    this->setGeometry(0,0,30,185);
    QSizePolicy policy = this->sizePolicy();
    policy.setHorizontalStretch(1);
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalStretch(1);
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    this->setSizePolicy(policy);
    connect(this, &QToolButton::released,this,[&]
    { if(!this->isChecked())
        {
            this->setStyleSheet("QToolButton {border-style:none;font-family: \"consolas\"; font-size:14px;background-color: #4169E1;}");
        }
    } );
}
bool HardwareButton::eventFilter(QObject *watched, QEvent *event)
{
    QToolButton *btn = (QToolButton*)watched;
    if (watched->inherits("QToolButton")) {
        if (event->type() == QEvent::Enter) {
            this->setStyleSheet("QToolButton {border-style:none;background-color: #d81e06;font-family: \"consolas\"; font-size:14px;}");
        } else if (event->type() == QEvent::Leave) {
            if(!btn->isChecked())
                    this->setStyleSheet("QToolButton {border-style:none;font-family: \"consolas\"; font-size:14px;background-color: #FFFFFF;}");
            else
                this->setStyleSheet("QToolButton {border-style:none;background-color: #d81e06;font-family: \"consolas\"; font-size:14px;}");
        }
    }
    return QObject::eventFilter(watched, event);
}

HardwareTab::HardwareTab(QWidget *parent) : QWidget(parent),
    CPUButton{new HardwareButton(this)}, MotherBroadButton{new HardwareButton(this)}, MemoryButton{new HardwareButton(this)}, GPUButton{new HardwareButton(this)},
    DiskButton{new HardwareButton(this)}, MonitorButton{new HardwareButton(this)}, AudioButton{new HardwareButton(this)}, NetworkButton{new HardwareButton(this)},
    OtherButton{new HardwareButton(this)}, layout{new QVBoxLayout(this)}, verticalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum))
{
    QPalette pal(this->palette());
    //设置背景white色
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->Init();
	connect(this->CPUButton, &QToolButton::clicked, this, [&] { emit SendHardwareTabIndex(0); });
	connect(this->MotherBroadButton, &QToolButton::clicked, this, [&] { emit SendHardwareTabIndex(1); });
	connect(this->MemoryButton, &QToolButton::clicked, this, [&] { emit SendHardwareTabIndex(2); });
	connect(this->GPUButton, &QToolButton::clicked, this, [&] { emit SendHardwareTabIndex(3); });
	connect(this->DiskButton, &QToolButton::clicked, this, [&] { emit SendHardwareTabIndex(4); });
	connect(this->MonitorButton, &QToolButton::clicked, this, [&] { emit SendHardwareTabIndex(5); });
	connect(this->AudioButton, &QToolButton::clicked, this, [&] { emit SendHardwareTabIndex(6); });
	connect(this->NetworkButton, &QToolButton::clicked, this, [&] { emit SendHardwareTabIndex(7); });
	connect(this->OtherButton, &QToolButton::clicked, this, [&] { emit SendHardwareTabIndex(8); });
}

void HardwareTab::Init()
{
    CPUButton->setText("CPU");
    MotherBroadButton->setText("Mother Broad");
    MemoryButton->setText("Memory");
    GPUButton->setText("GPU");
    DiskButton->setText("Storage");
    MonitorButton->setText("Monitor");
    AudioButton->setText("Audio");
    OtherButton->setText("Others");
    NetworkButton->setText("Network");
    layout->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    layout->addWidget(CPUButton);
    layout->addWidget(MotherBroadButton);
    layout->addWidget(MemoryButton);
    layout->addWidget(GPUButton);
    layout->addWidget(DiskButton);
    layout->addWidget(MonitorButton);
    layout->addWidget(AudioButton);
    layout->addWidget(NetworkButton);
    layout->addWidget(OtherButton);
    layout->addSpacerItem(verticalSpace);
	layout->setSpacing(0);
}

