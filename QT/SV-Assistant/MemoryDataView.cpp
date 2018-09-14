#include "MemoryDataView.h"
#include <QStylePainter>
#include "lib\Hardware\SV_Hardware.h"

MemoryDataView::MemoryDataView(QWidget *parent)
	: QTableWidget(parent),Base_Addr(0)
{
	this->Init();
}

void MemoryDataView::SetBaseAddr(ULARGE_INTEGER Base_Addr)
{
	QMutexLocker locker(&this->mutex);
	this->Base_Addr = Base_Addr.QuadPart;
}

MemoryDataView::~MemoryDataView()
{
}

void MemoryDataView::Init()
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
			SV_ASSIST::Ring0::RdMemory(Base_Addr + i * 0x10 + j, 1, iodata);
			std::shared_ptr<QTableWidgetItem> temp = std::make_shared<QTableWidgetItem>(QString::number(iodata).sprintf("%02X", iodata));
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

void MemoryDataView::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == timeID && this->isVisible())
	{
		this->UpdateData();
	}
	else
		QTableWidget::timerEvent(event);
}

void MemoryDataView::UpdateData()
{
	QMutexLocker locker(&this->mutex);
	for (size_t i = 0; i < 0x100; i++)
	{
		DWORD iodata = 0;
		SV_ASSIST::Ring0::RdMemory(Base_Addr + i, 1, iodata);
		datas.at(i)->setText(QString::number(iodata).sprintf("%02X", iodata));
	}
}
