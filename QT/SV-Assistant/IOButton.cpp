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
						font-family: \"consolas\"; font-size:14px;\
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
	Done->setStyleSheet("QPushButton{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	Close->setStyleSheet("QPushButton{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	Addr->setStyleSheet("QLabel{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	Addr->setText(tr("Start Address: "));
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
