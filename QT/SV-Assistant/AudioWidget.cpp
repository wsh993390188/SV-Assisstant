#include "AudioWidget.h"
#include "lib\Hardware\SV_Hardware.h"

AudioWidget::AudioWidget(QWidget *parent)
	: QTableWidget(parent)
{
	QPalette pal(this->palette());
	//设置背景white色
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

void AudioWidget::Init()
{
	this->setStyleSheet("QTableWidget { background:#FFFFFF; border:none; font-size:12px; font-family:\"consolas\"; color:#000000; }");
	this->setColumnCount(5);
	this->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("ID") << tr("State") << tr("Desc") << tr("Audioif"));
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->setFocusPolicy(Qt::NoFocus);
	this->setHidden(false);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setContentsMargins(0, 0, 0, 0);
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
	//verticalHeader  
	this->verticalHeader()->setStyleSheet
	("QHeaderView::section {background-color:#FFFFFF;"
		"font-size:12px; font-family:\"consolas\";color: #000000;padding-left: 4px;border:1px solid #FFFFFF;}");

	//horizontalHeader  
	this->horizontalHeader()->setStyleSheet
	("QHeaderView::section {background-color: #FFFFFF;;"
		"font-size:12px; font-family:\"consolas\" color: #FFFFFF;padding-left: 4px;border: 1px solid #FFFFFF;}"
		"QHeaderView::section:checked{background-color: SkyBlue;border: 1px solid #FFFFFF;font-size:12px; font-family:\"consolas\";}");

	SV_ASSIST::AUDIO::UpdateData();
	auto asd = SV_ASSIST::AUDIO::GetOutputAudio();
	int row_count = -1;
	int count = 0;
	for (const auto& i : asd)
	{
		row_count = this->rowCount();
		this->insertRow(row_count); //插入新行
		this->setVerticalHeaderItem(row_count, new QTableWidgetItem(QString::number(++count)));
		this->setItem(row_count, 0, new QTableWidgetItem(QString::fromStdWString(i.name)));
		this->setItem(row_count, 1, new QTableWidgetItem(QString::fromStdWString(i.id)));
		this->setItem(row_count, 2, new QTableWidgetItem(QString::fromStdWString(i.state)));
		this->setItem(row_count, 3, new QTableWidgetItem(QString::fromStdWString(i.desc)));
		this->setItem(row_count, 4, new QTableWidgetItem(QString::fromStdWString(i.audioif)));
	}
}
AudioWidget::~AudioWidget()
{
}
