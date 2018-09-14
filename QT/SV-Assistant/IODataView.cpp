#include "IODataView.h"
#include <QStylePainter>
#include "lib\Hardware\SV_Hardware.h"

IODataView::IODataView(QWidget *parent)
	: QTableWidget(parent), Base_Addr(0)
{
	this->Init();
}

void IODataView::SetBaseAddr(unsigned long Base_Addr)
{
	QMutexLocker locker(&this->mutex);
	this->Base_Addr = Base_Addr;
}

IODataView::~IODataView()
{
}

void IODataView::Init()
{
	this->setColumnCount(16);
	this->setHorizontalHeaderLabels(QStringList() << tr("00") << tr("01") << tr("02") << tr("03") << tr("04") << tr("05") << tr("06") << tr("07") << tr("08") << tr("09") << tr("0A") << tr("0B") << tr("0C") << tr("0D") << tr("0E") << tr("0F"));
	this->setRowCount(16);
	this->setVerticalHeaderLabels(QStringList() << tr("00") << tr("10") << tr("20") << tr("30") << tr("40") << tr("50") << tr("60") << tr("70") << tr("80") << tr("90") << tr("A0") << tr("B0") << tr("C0") << tr("D0") << tr("E0") << tr("F0"));
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->setHidden(false);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	for (int i = 0; i < 0x10; i++)
	{
		for (int j = 0; j < 0x10; j++)
		{
			DWORD iodata = 0;
			SV_ASSIST::Ring0::RdIOPort(Base_Addr + i * 0x10 + j, 1, iodata);
			std::shared_ptr<QTableWidgetItem> temp = std::make_shared<QTableWidgetItem>(QString::number(iodata).sprintf("%02X", iodata));
			this->setItem(i, j, temp.get());
			if (iodata == 0)
				temp->setTextColor(Qt::gray);
			else if (iodata == 0xFF)
				temp->setTextColor(Qt::darkGreen);
			else
				temp->setTextColor(Qt::darkBlue);
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

	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	timeID = this->startTimer(1000);
}

void IODataView::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == timeID && this->isVisible())
	{
		this->UpdateData();
	}
	else
		QTableWidget::timerEvent(event);
}

void IODataView::UpdateData()
{
	QMutexLocker locker(&this->mutex);
	for (size_t i = 0; i < 0x100; i++)
	{
		DWORD iodata = 0;
		SV_ASSIST::Ring0::RdIOPort(Base_Addr + i, 1, iodata);
		if (iodata == 0)
			datas.at(i)->setTextColor(Qt::gray);
		else if (iodata == 0xFF)
			datas.at(i)->setTextColor(Qt::darkGreen);
		else
			datas.at(i)->setTextColor(Qt::darkBlue);
		datas.at(i)->setText(QString::number(iodata).sprintf("%02X", iodata));
	}
}

IOIndexDataView::IOIndexDataView(QWidget *parent)
	: QTableWidget(parent),Index(0x70), Data(0x71)
{
	this->Init();
}

void IOIndexDataView::SetBaseAddr(uint Index, uint Data)
{
	QMutexLocker locker(&this->mutex);
	this->Index = Index;
	this->Data = Data;
}

IOIndexDataView::~IOIndexDataView()
{
}

void IOIndexDataView::Init()
{
	this->setColumnCount(16);
	this->setHorizontalHeaderLabels(QStringList() << tr("00") << tr("01") << tr("02") << tr("03") << tr("04") << tr("05") << tr("06") << tr("07") << tr("08") << tr("09") << tr("0A") << tr("0B") << tr("0C") << tr("0D") << tr("0E") << tr("0F"));
	this->setRowCount(16);
	this->setVerticalHeaderLabels(QStringList() << tr("00") << tr("10") << tr("20") << tr("30") << tr("40") << tr("50") << tr("60") << tr("70") << tr("80") << tr("90") << tr("A0") << tr("B0") << tr("C0") << tr("D0") << tr("E0") << tr("F0"));
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->setHidden(false);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	for (int i = 0; i < 0x10; i++)
	{
		for (int j = 0; j < 0x10; j++)
		{
			DWORD iodata = 0;
			SV_ASSIST::Ring0::WrIOPort(this->Index, 1, i * 0x10 + j);
			SV_ASSIST::Ring0::RdIOPort(this->Data, 1, iodata);
			std::shared_ptr<QTableWidgetItem> temp = std::make_shared<QTableWidgetItem>(QString::number(iodata).sprintf("%02X", iodata));
			if (iodata == 0)
				temp->setTextColor(Qt::gray);
			else if (iodata == 0xFF)
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

	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	timeID = this->startTimer(1000);
}

void IOIndexDataView::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == timeID && this->isVisible())
	{
		this->UpdateData();
	}
	else
		QTableWidget::timerEvent(event);
}

void IOIndexDataView::UpdateData()
{
	QMutexLocker locker(&this->mutex);
	for (size_t i = 0; i < 0x100; i++)
	{
		DWORD iodata = 0;
		SV_ASSIST::Ring0::WrIOPort(this->Index, 1, i);
		SV_ASSIST::Ring0::RdIOPort(this->Data, 1, iodata);
		if (iodata == 0)
			datas.at(i)->setTextColor(Qt::gray);
		else if (iodata == 0xFF)
			datas.at(i)->setTextColor(Qt::darkGreen);
		else
			datas.at(i)->setTextColor(Qt::darkBlue);
		datas.at(i)->setText(QString::number(iodata).sprintf("%02X", iodata));
	}
}
