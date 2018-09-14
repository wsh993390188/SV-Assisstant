#pragma once

#include <QWidget>
#include <QComboBox>
#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <QByteArray>
#include <QString>
#include <QGridLayout>
#include <QTextBrowser>
#include <QVector>
#include <QScrollBar>
#include <QMutex>
#include <QTimerEvent>
#include <QTimer>
#include <windows.h>
#include <memory>

struct PCIBDF
{
	USHORT bus;
	USHORT dev;
	USHORT func;
};

class PCITableWidget : public QTableWidget
{
	Q_OBJECT
public:
	explicit PCITableWidget(QWidget *parent = nullptr);
protected:
	void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
public slots:
	void RecivePCIName(PCIBDF PCIName);
private:
	void Init();
	void UpdateData();
private:
	QMutex mutex;
	QVector<std::shared_ptr<QTableWidgetItem>> datas;
	size_t timeID;
	ULONG bus;
	ULONG dev;
	ULONG func;
};

class PCIWidget : public QWidget
{
	Q_OBJECT
public:
	explicit PCIWidget(QWidget *parent = nullptr);
	~PCIWidget();
signals:
	void SendPCIName(PCIBDF PCIName);
private:
	void Init();
private:
	QComboBox * pcicombobox;
	QGridLayout * layout;
	PCITableWidget * pcitable;
	QTextBrowser * Browser;
	QVector<PCIBDF>	bdf;
};
