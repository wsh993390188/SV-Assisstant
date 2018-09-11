#include "HardwareMasterTab.h"

HardwareMasterTab::HardwareMasterTab(QWidget *parent)
	: QWidget(parent), IOButton{ new HardwareButton(this) },
	layout{ new QVBoxLayout(this) }, verticalSpace(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum))
{
	QPalette pal(this->palette());
	//ÉèÖÃ±³¾°whiteÉ«
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();

	connect(this->IOButton, &QToolButton::clicked, this, [this] { emit SendHardwareTabIndex(0); });
}

HardwareMasterTab::~HardwareMasterTab()
{
}

void HardwareMasterTab::Init()
{
	IOButton->setText(tr("IO"));

	layout->setContentsMargins(0, 0, 0, 0);
	layout->setMargin(0);
	layout->addWidget(IOButton);
	layout->addSpacerItem(verticalSpace);
	layout->setSpacing(0);
}
