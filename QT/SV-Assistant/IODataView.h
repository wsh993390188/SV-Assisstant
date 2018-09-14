#pragma once

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPalette>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QTimerEvent>
#include <QScrollBar>
#include <QVector>
#include <QMutex>
#include <memory>

class IODataView : public QTableWidget
{
	Q_OBJECT
public:
	explicit IODataView(QWidget *parent = nullptr);
	void SetBaseAddr(unsigned long Base_Addr);
	~IODataView();
protected:
	void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;	
private:
	void Init();
	void UpdateData();
private:
	QMutex mutex;
	QVector<std::shared_ptr<QTableWidgetItem>> datas;
	unsigned long volatile Base_Addr;
	size_t timeID;
};

class IOIndexDataView : public QTableWidget
{
	Q_OBJECT
public:
	explicit IOIndexDataView(QWidget *parent = nullptr);
	void SetBaseAddr(uint Index, uint Data);
	~IOIndexDataView();
protected:
	void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
	void Init();
	void UpdateData();
private:
	QMutex mutex;
	QVector<std::shared_ptr<QTableWidgetItem>> datas;
	uint volatile Index;
	uint volatile Data;
	size_t timeID;
};

