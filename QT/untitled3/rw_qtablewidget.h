#ifndef RW_QTABLEWIDGET_H
#define RW_QTABLEWIDGET_H

#include <QTableWidget>
#include <QStringList>
#include <QFile>
#include <QHeaderView>
#include <bitset>
#include "Driverdll.h"

class RW_Qtablewidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit RW_Qtablewidget(QWidget *parent = nullptr);
    void setIOMapping();
    void setMemoryMapping();
signals:

public slots:
    void ReceiveIOBase(unsigned int IOBase);
    void ReceiveMemoryBase(unsigned int MemBase);
    void SetIOData(int row, int column);
    void SetMemoryData(int row, int column);
private:
    void InitStyle();
    void Inittitle();
private:
    DWORD IOBase;
    DWORD64 MemBase;
};

#endif // RW_QTABLEWIDGET_H
