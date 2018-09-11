#pragma once

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPalette>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QTimerEvent>
#include <QScrollBar>
#include <QVector>
#include <QMutex>
#include <memory>

class IODataView : public QTableView
{
	Q_OBJECT
public:
	IODataView(QWidget *parent);
	void SetBaseAddr(unsigned long Base_Addr);
	~IODataView();
protected:
	void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;	
private:
	void Init();
	void UpdateData();
private:
	QMutex mutex;
	QVector<std::shared_ptr<QStandardItem>> datas;
	QStandardItemModel *model;
	unsigned long volatile Base_Addr;
	size_t timeID;
};
