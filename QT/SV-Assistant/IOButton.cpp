#include "IOButton.h"

IOButton::IOButton(QWidget *parent)
	: QToolButton(parent)
{
	this->setStyleSheet("QToolButton{ border - style:none; \
						border:1px solid #B6B6B6; \
						color:#57595B; \
						padding:5px; \
						min - height:15px; \
						border - radius:5px; \
						font-family: \"consolas\"; font-size:16px;\
						background:qlineargradient(spread : pad,x1 : 0,y1 : 0,x2 : 0,y2 : 1,stop : 0 #E4E4E4,stop:1 #E4E4E4);}");
	connect(this, &QToolButton::clicked, this, &IOButton::OnClickedInit);
}

void IOButton::OnClickedInit()
{
	QDialog diag;
	QVBoxLayout* dialogvlayout = new QVBoxLayout();
	QVBoxLayout* dialogvlayout_2 = new QVBoxLayout();
	QHBoxLayout* dialoghlayout = new QHBoxLayout(&diag);
	QHBoxLayout* dialoghlayout_2 = new QHBoxLayout();

	QPushButton* Done = new QPushButton(&diag);
	QPushButton* Close = new QPushButton(&diag);
	QLabel* Addr = new QLabel(&diag);
	QLineEdit* InputAddr = new QLineEdit(&diag);
	uint a1 = 0;
	sscanf(this->text().toStdString().c_str(), "Io Base Address: 0x%X", &a1);
	InputAddr->setText(tr("%1").arg(QString::number(a1,16).toUpper(),4, QLatin1Char('0')));
	Done->setStyleSheet("QPushButton{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	Close->setStyleSheet("QPushButton{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	Addr->setStyleSheet("QLabel{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	Addr->setText(tr("Base Address: "));
	Done->setText(tr("Done"));
	Close->setText(tr("Cancel"));
	QRegExp regx("^[a-fA-F0-9]{0,4}$");
	InputAddr->setValidator(new QRegExpValidator(regx, &diag));
	dialogvlayout->addWidget(Done);
	dialogvlayout->addWidget(Close);
	dialoghlayout_2->addWidget(Addr);
	dialoghlayout_2->addWidget(InputAddr);
	dialogvlayout_2->addLayout(dialoghlayout_2);
	dialoghlayout->addLayout(dialogvlayout_2);
	dialoghlayout->addLayout(dialogvlayout);
	connect(Done, &QPushButton::clicked, &diag, &QDialog::accept);
	connect(Close, &QPushButton::clicked, &diag, &QDialog::reject);
	diag.setLayout(dialoghlayout);

	if (diag.exec() == QDialog::Accepted && !InputAddr->text().isEmpty())
	{
		uint baseaddr = InputAddr->text().toUInt(nullptr, 16);
		this->setText(tr("Io Base Address: 0x%1").arg(QString::number(baseaddr, 16).toUpper(), 4, QLatin1Char('0')));
		qDebug() << "Dialog Accepted";
		emit SendBaseAddr(baseaddr);
	}
	else
	{
		qDebug() << "Dialog Reject";
	}
}

IOButton::~IOButton()
{
}

IOIndexButton::IOIndexButton(QWidget *parent)
	: QToolButton(parent)
{
	this->setStyleSheet("QToolButton{ border - style:none; \
						border:1px solid #B6B6B6; \
						color:#57595B; \
						padding:5px; \
						min - height:15px; \
						border - radius:5px; \
						font-family: \"consolas\"; font-size:16px;\
						background:qlineargradient(spread : pad,x1 : 0,y1 : 0,x2 : 0,y2 : 1,stop : 0 #E4E4E4,stop:1 #E4E4E4);}");
	connect(this, &QToolButton::clicked, this, &IOIndexButton::OnClickedInit);
}

void IOIndexButton::OnClickedInit()
{
	QDialog diag;
	QVBoxLayout* dialogvlayout = new QVBoxLayout();
	QVBoxLayout* dialogvlayout_2 = new QVBoxLayout();
	QHBoxLayout* dialoghlayout = new QHBoxLayout(&diag);
	QHBoxLayout* dialoghlayout_2 = new QHBoxLayout();
	QHBoxLayout* dialoghlayout_3 = new QHBoxLayout();

	QPushButton* Done = new QPushButton(&diag);
	QPushButton* Close = new QPushButton(&diag);
	QLabel* IndexAddr = new QLabel(&diag);
	QLineEdit* IndexAddrEdit = new QLineEdit(&diag);
	QLabel* DataAddr = new QLabel(&diag);
	QLineEdit* DataAddrEdit = new QLineEdit(&diag);
	int a1 = 0, a2 = 0;
	sscanf(this->text().toStdString().c_str(), "Io IndexData : 0x%x\\0x%x", &a1, &a2);
	Done->setStyleSheet("QPushButton{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	Close->setStyleSheet("QPushButton{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	IndexAddr->setStyleSheet("QLabel{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	DataAddr->setStyleSheet("QLabel{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	IndexAddr->setText(tr("Index Port:"));
	DataAddr->setText(tr("Data  Port:"));
	IndexAddrEdit->setText(QString::number(a1, 16).toUpper());
	DataAddrEdit->setText(QString::number(a2, 16).toUpper());
	Done->setText(tr("Done"));
	Close->setText(tr("Cancel"));

	QRegExp regx("^[a-fA-F0-9]{0,4}$");
	IndexAddrEdit->setValidator(new QRegExpValidator(regx, &diag));
	DataAddrEdit->setValidator(new QRegExpValidator(regx, &diag));
	dialogvlayout->addWidget(Done);
	dialogvlayout->addWidget(Close);
	dialoghlayout_2->addWidget(IndexAddr);
	dialoghlayout_2->addWidget(IndexAddrEdit);
	dialoghlayout_3->addWidget(DataAddr);
	dialoghlayout_3->addWidget(DataAddrEdit);
	dialogvlayout_2->addLayout(dialoghlayout_2);
	dialogvlayout_2->addLayout(dialoghlayout_3);
	dialoghlayout->addLayout(dialogvlayout_2);
	dialoghlayout->addLayout(dialogvlayout);
	connect(Done, &QPushButton::clicked, &diag, &QDialog::accept);
	connect(Close, &QPushButton::clicked, &diag, &QDialog::reject);
	diag.setLayout(dialoghlayout);

	if (diag.exec() == QDialog::Accepted && !IndexAddr->text().isEmpty() && !DataAddr->text().isEmpty())
	{
		uint Index = IndexAddr->text().toUInt(nullptr, 16);
		uint Data = DataAddr->text().toUInt(nullptr, 16);
		this->setText(tr("Io IndexData: 0x%1\\0x%2").arg(QString::number(Index, 16).toUpper()).arg(QString::number(Data, 16).toUpper()));
		qDebug() << "Dialog Accepted";
		emit SendBaseAddr(Index, Data);
	}
	else
	{
		qDebug() << "Dialog Reject";
	}
}


IOIndexButton::~IOIndexButton()
{
}
