#pragma once

#include <QListWidget>
#include <QScrollBar>
#include <QTableWidget>
#include <QVector>
#include <QString>
#include <string>
#include <vector>
#include "lib\Hardware\NetworkLib.h"

class NetworkListWidget : public QListWidget
{
	Q_OBJECT
public:
	NetworkListWidget(QWidget *parent = nullptr);
	void Init();

	~NetworkListWidget();
signals:
	void SendNetStruct(const SV_ASSIST::Net::NetStruct& data);
public slots:
	void ReceiveItemText(const QListWidgetItem *const item);
private:
	QVector<SV_ASSIST::Net::NetStruct> netdata;
};

class NetworkTableWidget : public QTableWidget
{
	Q_OBJECT
public:
	NetworkTableWidget(QWidget *parent = nullptr);
	void Init();
	~NetworkTableWidget();
public slots:
	void ReceiveNetdata(const SV_ASSIST::Net::NetStruct& data);
};