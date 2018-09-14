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
#include <vector>
#include <memory>
#include "lib\Hardware\SuperIOStruct.h"

class SIOTableWidget : public QTableWidget
{
	Q_OBJECT
public:
	explicit SIOTableWidget(QWidget *parent = nullptr);
public slots:
	void ReciveSIO(const int Index);
private:
	void Init();
private:
	std::vector<Super_IO_Data> datas;
	QVector<std::shared_ptr<QTableWidgetItem>> items;
};

class SIOWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SIOWidget(QWidget *parent = nullptr);
	~SIOWidget();
signals:
	void SendPCIName(const int Index);
private:
	void Init();
private:

	QComboBox * siocombobox;
	QGridLayout * layout;
	SIOTableWidget * siotable;
	QTextBrowser * Browser;
};
