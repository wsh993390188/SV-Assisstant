#ifndef PCITABLEWIDGET_H
#define PCITABLEWIDGET_H

#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <QByteArray>
#include <QString>
#include "Driverdll.h"

class pcitablewidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit pcitablewidget(QWidget *parent = nullptr);

signals:

public slots:
    void RecivePCIName(QString PCIName);
};

#endif // PCITABLEWIDGET_H
