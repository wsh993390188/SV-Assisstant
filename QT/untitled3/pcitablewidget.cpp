#include "pcitablewidget.h"

pcitablewidget::pcitablewidget(QWidget *parent) : QTableWidget(parent)
{
    this->setColumnCount(16);
    this->setHorizontalHeaderLabels(QStringList()<< tr("00")<< tr("01")<< tr("02")<< tr("03")<< tr("04")<< tr("05")<< tr("06")<< tr("07")<< tr("08")<< tr("09")<< tr("0A")<< tr("0B")<< tr("0C")<< tr("0D")<< tr("0E")<< tr("0F"));
    this->setRowCount(16);
    this->setVerticalHeaderLabels(QStringList()<< tr("00")<< tr("10")<< tr("20")<< tr("30")<< tr("40")<< tr("50")<< tr("60")<< tr("70")<< tr("80")<< tr("90")<< tr("A0")<< tr("B0")<< tr("C0")<< tr("D0")<< tr("E0")<< tr("F0"));
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setHidden(false);
    this->setContentsMargins(0,0,0,0);
    PCI_COMMON_CONFIG pci = {};
    ReadPci(0, 0, 0, pci);
    this->setRowCount(0);
    int row_count = -1;
    unsigned char a[0x100];
    memcpy(a, &pci, sizeof(pci));
    for(int i = 0; i <=0xFF; i++)
    {
        if(i%16 == 0)
        {
            row_count = this->rowCount();
            this->insertRow(row_count); //插入新行
            this->setVerticalHeaderItem(row_count, new QTableWidgetItem(QString(tr("%1").arg(QString::number(row_count, 16).toUpper())) + tr("0")));
        }
        this->setItem(row_count, i%16, new QTableWidgetItem(QString::number(a[i], 16).toUpper()));
    }
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void pcitablewidget::RecivePCIName(QString PCIName)
{
    char*  ch;
    QByteArray ba = PCIName.toLatin1(); // must
    ch=ba.data();
    int bus, dev,func;
    sscanf(ch, "bus %d dev %d func %d", &bus, &dev, &func);
    PCI_COMMON_CONFIG pci = {};
    ReadPci(bus, dev, func, pci);
    this->setRowCount(0);
    int row_count = -1;
    unsigned char a[0x100];
    memcpy(a, &pci, sizeof(pci));
    for(int i = 0; i <=0xFF; i++)
    {
        if(i%16 == 0)
        {
            row_count = this->rowCount();
            this->insertRow(row_count); //插入新行
            this->setVerticalHeaderItem(row_count, new QTableWidgetItem(tr("0%1").arg(QString::number(row_count, 16).toUpper())));
        }
        this->setItem(row_count, i%16, new QTableWidgetItem(QString::number(a[i], 16).toUpper()));
    }
}
