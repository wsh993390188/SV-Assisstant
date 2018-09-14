#include "PCIWidget.h"
#include <QListView>
#include "lib\Hardware\SV_Hardware.h"

PCIWidget::PCIWidget(QWidget *parent)
	: QWidget(parent), pcicombobox(new QComboBox(this)), pcitable(new PCITableWidget(this)),
	Browser(new QTextBrowser(this)), layout(new QGridLayout(this))
{
	this->Init();
}

PCIWidget::~PCIWidget()
{
}

void PCIWidget::Init()
{
	auto allpci = SV_ASSIST::Ring0::GetAllPciInfo();
	for (const auto& pci : allpci.pciconfig)
	{
		PCIBDF temp = { std::get<0>(pci.first), std::get<1>(pci.first), std::get<2>(pci.first) };
		std::string vendor = {}, device = {};
		SV_ASSIST::PCIE::GetPCIDeviceName(pci.second.VendorID, pci.second.DeviceID, vendor, device);
		this->pcicombobox->addItem(tr("BUS %1 DEV %2 FUNC %3 %4 %5").arg(QString::number(temp.bus,16).toUpper()).arg(QString::number(temp.dev).toUpper()).arg(QString::number(temp.func,16).toUpper()).arg(QString::fromStdString(vendor)).arg(QString::fromStdString(device)));
		
		bdf.push_back(std::move(temp));
	}
	this->pcicombobox->setMaximumSize(900, 40);
	this->pcicombobox->setMinimumSize(600, 0);
	this->pcitable->setMaximumSize(900, 750);
	this->pcitable->setMinimumSize(600, 0);
	this->Browser->setMaximumSize(300, 750);
	this->Browser->setMinimumSize(200, 0);
	layout->addWidget(pcicombobox, 0, 0, 1, 2);
	layout->addWidget(pcitable, 1, 0, 1, 2);
	layout->addWidget(Browser, 1, 2, 1, 1);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	connect(this->pcicombobox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int Index) {emit SendPCIName(bdf[Index]); });
	connect(this,&PCIWidget::SendPCIName,this->pcitable, &PCITableWidget::RecivePCIName);
	this->setLayout(layout);
	this->setContentsMargins(0, 0, 0, 0);
	this->pcicombobox->setStyleSheet(
		"QComboBox{ border: 1px solid gray; border-radius: 3px; font-family:\"consolas\"; padding: 1px 2px 1px 2px; min - width: 9em; }"
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

PCITableWidget::PCITableWidget(QWidget *parent) :
	QTableWidget(parent),bus(0),dev(0),func(0)
{
	this->Init();
}

void PCITableWidget::Init()
{
	this->setColumnCount(16);
	this->setHorizontalHeaderLabels(QStringList() << tr("00") << tr("01") << tr("02") << tr("03") << tr("04") << tr("05") << tr("06") << tr("07") << tr("08") << tr("09") << tr("0A") << tr("0B") << tr("0C") << tr("0D") << tr("0E") << tr("0F"));
	this->setRowCount(16);
	this->setVerticalHeaderLabels(QStringList() << tr("00") << tr("10") << tr("20") << tr("30") << tr("40") << tr("50") << tr("60") << tr("70") << tr("80") << tr("90") << tr("A0") << tr("B0") << tr("C0") << tr("D0") << tr("E0") << tr("F0"));
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->setHidden(false);

	for (int i = 0; i < 0x10; i++)
	{
		for (int j = 0; j < 0x10; j++)
		{
			PCI_COMMON_CONFIG data = {};
			SV_ASSIST::Ring0::ReadPci(0,0,0,data);
			std::shared_ptr<QTableWidgetItem> temp = std::make_shared<QTableWidgetItem>(QString::number(reinterpret_cast<UCHAR*>(&data)[i * 0x10 + j]).sprintf("%02X", reinterpret_cast<UCHAR*>(&data)[i * 0x10 + j]));
			if (reinterpret_cast<UCHAR*>(&data)[i * 0x10 + j] == 0)
				temp->setTextColor(Qt::gray);
			else if (reinterpret_cast<UCHAR*>(&data)[i * 0x10 + j] == 0xFF)
				temp->setTextColor(Qt::darkGreen);
			else
				temp->setTextColor(Qt::darkBlue);
			this->setItem(i, j, temp.get());
			datas.push_back(temp);
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
	timeID = this->startTimer(1000);
}

void PCITableWidget::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == timeID && this->isVisible())
	{
		this->UpdateData();
	}
	else
		QTableWidget::timerEvent(event);
}

void PCITableWidget::UpdateData()
{
	QMutexLocker lock(&this->mutex);
	PCI_COMMON_CONFIG pci = {};
	SV_ASSIST::Ring0::ReadPci(bus, dev, func, pci);
	for (int i = 0; i <= 0xFF; i++)
	{
		datas.at(i)->setText(QString::number(reinterpret_cast<UCHAR*>(&pci)[i]).sprintf("%02X", reinterpret_cast<UCHAR*>(&pci)[i]));
		if (reinterpret_cast<UCHAR*>(&pci)[i] == 0)
			datas.at(i)->setTextColor(Qt::gray);
		else if (reinterpret_cast<UCHAR*>(&pci)[i] == 0xFF)
			datas.at(i)->setTextColor(Qt::darkGreen);
		else
			datas.at(i)->setTextColor(Qt::darkBlue);
	}
}

void PCITableWidget::RecivePCIName(PCIBDF PCIName)
{
	bus = PCIName.bus;
	dev = PCIName.dev;
	func = PCIName.func;
	this->UpdateData();
}