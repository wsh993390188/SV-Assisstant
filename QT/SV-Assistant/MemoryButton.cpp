#include "MemoryButton.h"

MemoryButton::MemoryButton(QWidget *parent)
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
	connect(this, &QToolButton::clicked, this, &MemoryButton::OnClickedInit);
}

void MemoryButton::OnClickedInit()
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
	sscanf(this->text().toStdString().c_str(), "Memory Base Address: 0x%X", &a1);
	InputAddr->setText(tr("%1").arg(QString::number(a1, 16).toUpper(), 16, QLatin1Char('0')));
	Done->setStyleSheet("QPushButton{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	Close->setStyleSheet("QPushButton{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	Addr->setStyleSheet("QLabel{color:#000000;font-family: \"consolas\"; font-size:14px;}");
	Addr->setText(tr("Base Address: "));
	Done->setText(tr("Done"));
	Close->setText(tr("Cancel"));
	QRegExp regx("^[a-fA-F0-9]{0,16}$");
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
		unsigned __int64 baseaddr = InputAddr->text().toULongLong(nullptr, 16);
		this->setText(tr("Memory Base Address: 0x%1").arg(QString::number(baseaddr, 16).toUpper(), 16, QLatin1Char('0')));
		qDebug() << "Dialog Accepted";
		ULARGE_INTEGER temp;

		if constexpr (sizeof(void*) == 4)
		{
			temp.HighPart = 0;
			temp.LowPart = (ulong)baseaddr;
		}
		else
		{
			temp.QuadPart = baseaddr;
		}
		emit SendBaseAddr(temp);
	}
	else
	{
		qDebug() << "Dialog Reject";
	}
}

MemoryButton::~MemoryButton()
{
}
