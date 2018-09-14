#include "NetworkWidget.h"
#include <QListWidgetItem>
#include <QHeaderView>

NetworkListWidget::NetworkListWidget(QWidget *parent)
	: QListWidget(parent), netdata{}
{
	QPalette pal(this->palette());
	//设置背景white色
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

void NetworkListWidget::Init()
{
	this->setStyleSheet("QListWidget { background:#FFFFFF; border:none; font-size:16px; font-family:\"consolas\"; color:#000000; }");
//		"QHeaderView::section{font-size:12px; font-family:\"consolas\"; color:#000000; background:#60669B; border:none; text-align:left; min-height:49px; max-height:49px;	margin-left:0px; padding-left:0px;}");
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

	this->setFocusPolicy(Qt::NoFocus);
	this->setHidden(false);
	this->setContentsMargins(0, 0, 0, 0);
	SV_ASSIST::Net::Update();
	auto a = SV_ASSIST::Net::GetData();
	for (const auto& i : a)
	{
		netdata.push_back(i);
		this->addItem(QString::fromStdWString(i.DeviceName));
	}
	connect(this, &NetworkListWidget::itemClicked, this, &NetworkListWidget::ReceiveItemText);
}

void NetworkListWidget::ReceiveItemText(const QListWidgetItem *const item)
{
	for (const auto&i : this->netdata)
	{
		if (item->text() == QString::fromStdWString(i.DeviceName))
		{
			emit SendNetStruct(i);
			break;
		}
	}
}

NetworkListWidget::~NetworkListWidget()
{
}

NetworkTableWidget::NetworkTableWidget(QWidget *parent) : QTableWidget(parent)
{
	QPalette pal(this->palette());
	//设置背景white色
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

void NetworkTableWidget::ReceiveNetdata(const SV_ASSIST::Net::NetStruct& data)
{
	this->setItem(0, 1, new QTableWidgetItem(QString::fromStdWString(data.Name)));	
	this->setItem(1, 1, new QTableWidgetItem(QString::fromStdWString(data.DeviceName)));
	this->setItem(2, 1, new QTableWidgetItem(QString::fromStdWString(data.MACAddrss)));
	this->setItem(3, 1, new QTableWidgetItem(QString::fromStdWString(data.IPAddress)));
	this->setItem(4, 1, new QTableWidgetItem(QString::fromStdWString(data.IPMask)));
	this->setItem(5, 1, new QTableWidgetItem(QString::fromStdWString(data.GateWay)));
	this->setItem(6, 1, new QTableWidgetItem(QString::fromStdWString(data.NetWorkType)));
	this->setItem(7, 1, new QTableWidgetItem(QString::fromStdWString(data.DHCP.DHCPService)));
	this->setItem(8, 1, new QTableWidgetItem(QString::fromStdWString(data.DHCP.LeaseObtained)));
	this->setItem(9, 1, new QTableWidgetItem(QString::fromStdWString(data.DHCP.LeaseExpires)));
	this->setItem(10, 1, new QTableWidgetItem(QString::fromStdWString(data.Wins.PrimaryWinsServer)));
	this->setItem(11, 1, new QTableWidgetItem(QString::fromStdWString(data.Wins.SecondaryWinsServer)));
}

void NetworkTableWidget::Init()
{
	this->setColumnCount(2);
	this->setRowCount(12);
	this->setItem(0, 0, new QTableWidgetItem(tr("Connection Name")));
	this->setItem(0, 1, new QTableWidgetItem(""));
	this->setItem(1, 0, new QTableWidgetItem(tr("Device Name")));
	this->setItem(1, 1, new QTableWidgetItem(""));
	this->setItem(2, 0, new QTableWidgetItem(tr("MAC Address")));
	this->setItem(2, 1, new QTableWidgetItem(""));
	this->setItem(3, 0, new QTableWidgetItem(tr("IP Address")));
	this->setItem(3, 1, new QTableWidgetItem(""));
	this->setItem(4, 0, new QTableWidgetItem(tr("Subnet Mask")));
	this->setItem(4, 1, new QTableWidgetItem(""));
	this->setItem(5, 0, new QTableWidgetItem(tr("Default Gateway")));
	this->setItem(5, 1, new QTableWidgetItem(""));
	this->setItem(6, 0, new QTableWidgetItem(tr("Network Type")));
	this->setItem(6, 1, new QTableWidgetItem(""));
	this->setItem(7, 0, new QTableWidgetItem(tr("DHCP Server")));
	this->setItem(7, 1, new QTableWidgetItem(""));
	this->setItem(8, 0, new QTableWidgetItem(tr("DHCP Lease Obtained")));
	this->setItem(8, 1, new QTableWidgetItem(""));
	this->setItem(9, 0, new QTableWidgetItem(tr("DHCP Lease Expires")));
	this->setItem(9, 1, new QTableWidgetItem(""));
	this->setItem(10, 0, new QTableWidgetItem(tr("Primary Service")));
	this->setItem(10, 1, new QTableWidgetItem(""));
	this->setItem(11, 0, new QTableWidgetItem(tr("Secondary Service")));
	this->setItem(11, 1, new QTableWidgetItem(""));
	this->setHorizontalHeaderLabels(QStringList() << tr("Item") << tr("Description"));
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->setFocusPolicy(Qt::NoFocus);
	this->setHidden(false);
	this->setContentsMargins(0, 0, 0, 0);
	this->horizontalHeader()->setStyleSheet
	("QHeaderView::section {background-color: #FFFFFF;;"
		"font-size:12px; font-family:\"consolas\" color: #FFFFFF;padding-left: 4px;border: 1px solid #FFFFFF;}"
		"QHeaderView::section:checked{background-color: SkyBlue;border: 1px solid #FFFFFF;font-size:12px; font-family:\"consolas\";}");
	this->setStyleSheet("QTableWidget { background:#FFFFFF; border:none; font-size:16px; font-family:\"consolas\"; color:#000000; }");
	//		"QHeaderView::section{font-size:12px; font-family:\"consolas\"; color:#000000; background:#60669B; border:none; text-align:left; min-height:49px; max-height:49px;	margin-left:0px; padding-left:0px;}");
	this->horizontalScrollBar()->setStyleSheet(
		"QScrollBar:horizontal{ background:#E4E4E4; max-height:12px;}"
		"QScrollBar::handle:horizontal{ background:#B6B6B6; min-height:50px; border-radius:6px; }"
		"QScrollBar::handle:horizontal:hover{	background:#00BB9E; }"
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
	this->verticalHeader()->setVisible(false);
}

NetworkTableWidget::~NetworkTableWidget()
{

}
