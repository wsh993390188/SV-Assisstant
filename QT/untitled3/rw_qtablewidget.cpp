#include "rw_qtablewidget.h"
#include <QPushButton>
#include <QCheckBox>
#include <QString>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QValidator>
#include <QDebug>

RW_Qtablewidget::RW_Qtablewidget(QWidget *parent) : QTableWidget(parent), IOBase(0), MemBase(0)
{
    this->Inittitle();
    this->InitStyle();
}

void RW_Qtablewidget::InitStyle()
{
    QFile file(":/resources/QSS/QScroll.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        this->setStyleSheet(qss);
        file.close();
    }
}

void RW_Qtablewidget::Inittitle()
{
    this->setColumnCount(16);
    this->setHorizontalHeaderLabels(QStringList()<< tr("00")<< tr("01")<< tr("02")<< tr("03")<< tr("04")<< tr("05")<< tr("06")<< tr("07")<< tr("08")<< tr("09")<< tr("0A")<< tr("0B")<< tr("0C")<< tr("0D")<< tr("0E")<< tr("0F"));
    this->setRowCount(16);
    this->setVerticalHeaderLabels(QStringList()<< tr("00")<< tr("10")<< tr("20")<< tr("30")<< tr("40")<< tr("50")<< tr("60")<< tr("70")<< tr("80")<< tr("90")<< tr("A0")<< tr("B0")<< tr("C0")<< tr("D0")<< tr("E0")<< tr("F0"));
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setHidden(false);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void RW_Qtablewidget::ReceiveIOBase(unsigned int IOBase)
{
    this->IOBase = IOBase;
    this->setIOMapping();
}

void RW_Qtablewidget::ReceiveMemoryBase(unsigned int MemBase)
{
    this->MemBase = MemBase;
    this->setMemoryMapping();
}

void RW_Qtablewidget::setIOMapping()
{
    this->setRowCount(0);
    int row_count = -1;
    for(int i = 0; i <=0xFF; i++)
    {
        DWORD64 IOData = 0;
		SV_ASSIST::Ring0::RdIOPort(this->IOBase + i, 1, IOData);
        if(i%16 == 0)
        {
            row_count = this->rowCount();
            this->insertRow(row_count); //插入新行
            this->setVerticalHeaderItem(row_count, new QTableWidgetItem(QString(tr("%1").arg(QString::number(row_count, 16).toUpper())) + tr("0")));
        }
        this->setItem(row_count, i%16, new QTableWidgetItem(QString::number(IOData, 16).toUpper()));
    }
}

void RW_Qtablewidget::setMemoryMapping()
{
    this->setRowCount(0);
    int row_count = -1;
    for(int i = 0; i <=0xFF; i++)
    {
        DWORD MemData = 0;
		SV_ASSIST::Ring0::RdMemory(this->MemBase + i, 1, MemData);
        if(i%16 == 0)
        {
            row_count = this->rowCount();
            this->insertRow(row_count); //插入新行
            this->setVerticalHeaderItem(row_count, new QTableWidgetItem(QString(tr("%1").arg(QString::number(row_count, 16).toUpper())) + tr("0")));
        }
        this->setItem(row_count, i%16, new QTableWidgetItem(QString::number(MemData, 16).toUpper()));
    }
}

void RW_Qtablewidget::SetIOData(int row, int column)
{
    QDialog *diag = new QDialog(this);
    QVBoxLayout* dialogvlayout = new QVBoxLayout();
    QVBoxLayout* dialogvlayout_2 = new QVBoxLayout();
    QHBoxLayout* dialoghlayout = new QHBoxLayout(diag);
    QTableWidget *diagTabelwidget = new QTableWidget(diag);
    QPushButton* Done = new QPushButton(diag);
    QPushButton* Close = new QPushButton(diag);
    QLineEdit* InputAddr = new QLineEdit(diag);
    Done->setText(tr("Done"));
    Close->setText(tr("Cancel"));
    InputAddr->setText(this->item(row, column)->text());
    diagTabelwidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    diagTabelwidget->setMinimumSize(400, 90);
    diagTabelwidget->setColumnCount(8);
    diagTabelwidget->setHorizontalHeaderLabels(QStringList() << tr("7") << tr("6") << tr("5") << tr("4") << tr("3") << tr("2") << tr("1") << tr("0"));
    diagTabelwidget->horizontalHeader()->setStretchLastSection(false);
    diagTabelwidget->setRowCount(1);
    diagTabelwidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    diagTabelwidget->verticalHeader()->setVisible(false);
    QVariant s = this->item(row, column)->data(Qt::DisplayRole);
    std::bitset<8> temp(s.toInt());

    for (int i = 0; i < 8; i++)
    {
        diagTabelwidget->setItem(0, i, new QTableWidgetItem(QString::number(temp[7 - i], 16)));
    }
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    QRegExp regx("^[a-fA-F0-9]{0,2}$");
    InputAddr->setValidator(new QRegExpValidator(regx, diag));
    dialogvlayout->addWidget(Done);
    dialogvlayout->addWidget(Close);
    dialogvlayout_2->addWidget(diagTabelwidget);
    dialogvlayout_2->addWidget(InputAddr);
    dialoghlayout->addLayout(dialogvlayout_2);
    dialoghlayout->addLayout(dialogvlayout);
    connect(Done, &QPushButton::clicked, diag, &QDialog::accept);
    connect(Close, &QPushButton::clicked, diag, &QDialog::reject);
    connect(InputAddr, &QLineEdit::textChanged, diagTabelwidget, [&](const QString& x)
    {
        std::bitset<8> TableData(x.toUInt(nullptr, 16));
        for (int i = 0; i < 8; i++)
        {
            QVariant v;
            uint a = TableData[7 - i];
            v.setValue(a);
            QTableWidgetItem *item = diagTabelwidget->item(0, i);
            item->setData(Qt::DisplayRole, v);
        }
    });
    connect(diagTabelwidget, &QTableWidget::itemClicked, diagTabelwidget, [&](QTableWidgetItem *item)
    {
       uint num = item->data(Qt::DisplayRole).toUInt();
       num = !num;
       QVariant v;
       v.setValue(num);
       item->setData(Qt::DisplayRole, v);
       std::bitset<8> TableData(InputAddr->text().toUInt(nullptr, 16));
       TableData[7 - item->column()] = num;
       InputAddr->setText(QString::number(TableData.to_ulong(), 16).toUpper());
    });
    connect(diagTabelwidget, &QTableWidget::itemDoubleClicked, diagTabelwidget, [&](QTableWidgetItem *item)
    {
       uint num = item->data(Qt::DisplayRole).toUInt();
       num = !num;
       QVariant v;
       v.setValue(num);
       item->setData(Qt::DisplayRole, v);
    });
    if(diag->exec() == QDialog::Accepted && !InputAddr->text().isEmpty())
    {
        uint data = InputAddr->text().toUInt(nullptr, 16);
        uint baseaddr = this->IOBase + row * 16 + column;
        qDebug() << "Base Addr"<< baseaddr;
		SV_ASSIST::Ring0::WrIOPort(baseaddr, 1, data);
    }


}

void RW_Qtablewidget::SetMemoryData(int row, int column)
{
    QDialog *diag = new QDialog(this);
    QVBoxLayout* dialogvlayout = new QVBoxLayout();
    QVBoxLayout* dialogvlayout_2 = new QVBoxLayout();
    QHBoxLayout* dialoghlayout = new QHBoxLayout(diag);
    QTableWidget *diagTabelwidget = new QTableWidget(diag);
    QPushButton* Done = new QPushButton(diag);
    QPushButton* Close = new QPushButton(diag);
    QLineEdit* InputAddr = new QLineEdit(diag);
    Done->setText(tr("Done"));
    Close->setText(tr("Cancel"));
    InputAddr->setText(this->item(row, column)->text());
    diagTabelwidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    diagTabelwidget->setMinimumSize(400, 90);
    diagTabelwidget->setColumnCount(8);
    diagTabelwidget->setHorizontalHeaderLabels(QStringList() << tr("7") << tr("6") << tr("5") << tr("4") << tr("3") << tr("2") << tr("1") << tr("0"));
    diagTabelwidget->horizontalHeader()->setStretchLastSection(false);
    diagTabelwidget->setRowCount(1);
    diagTabelwidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    diagTabelwidget->verticalHeader()->setVisible(false);
    QVariant s = this->item(row, column)->data(Qt::DisplayRole);
    std::bitset<8> temp(s.toInt());
    for (int i = 0; i < 8; i++)
    {
        diagTabelwidget->setItem(0, i, new QTableWidgetItem(QString::number(temp[7 - i], 16)));
    }
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    QRegExp regx("^[a-fA-F0-9]{0,2}$");
    InputAddr->setValidator(new QRegExpValidator(regx, diag));
    dialogvlayout->addWidget(Done);
    dialogvlayout->addWidget(Close);
    dialogvlayout_2->addWidget(diagTabelwidget);
    dialogvlayout_2->addWidget(InputAddr);
    dialoghlayout->addLayout(dialogvlayout_2);
    dialoghlayout->addLayout(dialogvlayout);
    connect(Done, &QPushButton::clicked, diag, &QDialog::accept);
    connect(Close, &QPushButton::clicked, diag, &QDialog::reject);
    connect(InputAddr, &QLineEdit::textChanged, diagTabelwidget, [&](const QString& x)
    {
        std::bitset<8> TableData(x.toUInt(nullptr, 16));
        for (int i = 0; i < 8; i++)
        {
            QVariant v;
            uint a = TableData[7 - i];
            v.setValue(a);
            QTableWidgetItem *item = diagTabelwidget->item(0, i);
            item->setData(Qt::DisplayRole, v);
        }
    });
    connect(diagTabelwidget, &QTableWidget::itemClicked, diagTabelwidget, [&](QTableWidgetItem *item)
    {
        uint num = item->data(Qt::DisplayRole).toUInt();
        num = !num;
        QVariant v;
        v.setValue(num);
        item->setData(Qt::DisplayRole, v);
        std::bitset<8> TableData(InputAddr->text().toUInt(nullptr, 16));
        TableData[7 - item->column()] = num;
        InputAddr->setText(QString::number(TableData.to_ulong(), 16).toUpper());
    });
    connect(diagTabelwidget, &QTableWidget::itemDoubleClicked, diagTabelwidget, [&](QTableWidgetItem *item)
    {
        uint num = item->data(Qt::DisplayRole).toUInt();
        num = !num;
        QVariant v;
        v.setValue(num);
        item->setData(Qt::DisplayRole, v);
    });
    if(diag->exec() == QDialog::Accepted && !InputAddr->text().isEmpty())
    {
        uint data = InputAddr->text().toUInt(nullptr, 16);
        uint baseaddr = this->IOBase + row * 16 + column;
        qDebug() << "Base Addr"<< baseaddr;
		SV_ASSIST::Ring0::WrMemory(baseaddr, 1, data);
    }
}


