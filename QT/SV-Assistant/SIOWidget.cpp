#include "SIOWidget.h"
#include "lib\Hardware\SV_Hardware.h"

SIOWidget::SIOWidget(QWidget *parent)
	: QWidget(parent), siocombobox(new QComboBox(this)), siotable(new SIOTableWidget(this)),
	Browser(new QTextBrowser(this)), layout(new QGridLayout(this))
{
	this->Init();
}

SIOWidget::~SIOWidget()
{
}

void SIOWidget::Init()
{
	auto allsio = SV_ASSIST::SUPERIO::GetSioPage(SV_ASSIST::SUPERIO::GetSuperIoChipId());
	for (const auto& data : allsio)
	{
		this->siocombobox->addItem(tr("Logical Device #%1").arg(QString::number(data,16).toUpper()));
	}
	
	this->siotable->setMaximumSize(900, 750);
	this->siotable->setMinimumSize(600, 0);
	this->Browser->setMaximumSize(300, 750);
	this->Browser->setMinimumSize(200, 0);
	layout->addWidget(siocombobox, 0, 0, 1, 1);
	layout->addWidget(siotable, 1, 0, 1, 2);
	layout->addWidget(Browser, 1, 2, 1, 1);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	connect(this->siocombobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](const int text) {emit SendPCIName(text); });
	connect(this,&SIOWidget::SendPCIName,this->siotable, &SIOTableWidget::ReciveSIO);
	this->setLayout(layout);
	this->setContentsMargins(0, 0, 0, 0);
	this->siocombobox->setStyleSheet(
		"QScrollBar:vertical{ background:#E4E4E4; padding:0px; border-radius:6px; max-width:12px;}"
		"QScrollBar::handle:vertical{ background:#B6B6B6; min-height:50px; border-radius:6px; }"
		"QScrollBar::handle:vertical:hover{	background:#00BB9E; }"
		"QScrollBar::handle:vertical:pressed{ background:#00BB9E;}"
		"QScrollBar::add-page:vertical{background:none;}"
		"QScrollBar::sub-page:vertical{background:none;}"
		"QScrollBar::add-line:vertical{background:none;}"
		"QScrollBar::sub-line:vertical{background:none;}"
		"QScrollArea{border:0px; }");
}

SIOTableWidget::SIOTableWidget(QWidget *parent) :
	QTableWidget(parent)
{
	this->Init();
}

void SIOTableWidget::Init()
{
	this->setColumnCount(16);
	this->setHorizontalHeaderLabels(QStringList() << tr("00") << tr("01") << tr("02") << tr("03") << tr("04") << tr("05") << tr("06") << tr("07") << tr("08") << tr("09") << tr("0A") << tr("0B") << tr("0C") << tr("0D") << tr("0E") << tr("0F"));
	this->setRowCount(16);
	this->setVerticalHeaderLabels(QStringList() << tr("00") << tr("10") << tr("20") << tr("30") << tr("40") << tr("50") << tr("60") << tr("70") << tr("80") << tr("90") << tr("A0") << tr("B0") << tr("C0") << tr("D0") << tr("E0") << tr("F0"));
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->setHidden(false);

	SV_ASSIST::SUPERIO::ReadSioPage(datas, SV_ASSIST::SUPERIO::GetSioPage(SV_ASSIST::SUPERIO::GetSuperIoChipId()));
	for (int i = 0; i < 0x10; i++)
	{
		for (int j = 0; j < 0x10; j++)
		{
			std::shared_ptr<QTableWidgetItem> temp = std::make_shared<QTableWidgetItem>(QString::number(datas[0].SuperioData[i * 0x10 + j]).sprintf("%02X", datas[0].SuperioData[i * 0x10 + j]));
			if (datas[0].SuperioData[i * 0x10 + j] == 0)
				temp->setTextColor(Qt::gray);
			else if (datas[0].SuperioData[i * 0x10 + j] == 0xFF)
				temp->setTextColor(Qt::darkGreen);
			else
				temp->setTextColor(Qt::darkBlue);
			this->setItem(i, j, temp.get());
			items.push_back(temp);
		}
	}

	this->horizontalScrollBar()->setStyleSheet(
		"QScrollBar:horizontal{ background:#E4E4E4; max-height:12px;}"
		"QScrollBar::handle:horizontal{ background:#B6B6B6; min-height:50px; border-radius:6px; }"
		"QScrollBar::handle:horizontal:hover{ background:#00BB9E; }"
		"QScrollBar::handle:horizontal:pressed{ background:#00BB9E;}"
		"QScrollBar::add-page:horizontal{background:none;}"
		"QScrollBar::sub-page:horizontal{background:none;}"
		"QScrollBar::add-line:horizontal{background:none;}"
		"QScrollBar::sub-line:horizontal{background:none;}"
		"QScrollArea{border:0px; }");

	this->verticalScrollBar()->setStyleSheet(
		"QScrollBar:vertical{ background:#E4E4E4; padding:0px; border-radius:6px; max-width:12px;}"
		"QScrollBar::handle:vertical{ background:#B6B6B6; min-height:50px; border-radius:6px; }"
		"QScrollBar::handle:vertical:hover{	background:#00BB9E; }"
		"QScrollBar::handle:vertical:pressed{ background:#00BB9E;}"
		"QScrollBar::add-page:vertical{background:none;}"
		"QScrollBar::sub-page:vertical{background:none;}"
		"QScrollBar::add-line:vertical{background:none;}"
		"QScrollBar::sub-line:vertical{background:none;}"
		"QScrollArea{border:0px; }");
}

void SIOTableWidget::ReciveSIO(const int Index)
{
	for (int i = 0; i <= 0xFF; i++)
	{
		items.at(i)->setText(QString::number(datas[Index].SuperioData[i]).sprintf("%02X", datas[Index].SuperioData[i]));
		if (datas[Index].SuperioData[i] == 0)
			items.at(i)->setTextColor(Qt::gray);
		else if (datas[Index].SuperioData[i] == 0xFF)
			items.at(i)->setTextColor(Qt::darkGreen);
		else
			items.at(i)->setTextColor(Qt::darkBlue);
	}
}