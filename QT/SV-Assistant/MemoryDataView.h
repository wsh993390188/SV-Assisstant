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
#include <QHeaderView>
#include <QTimerEvent>
#include <QScrollBar>
#include <QVector>
#include <QMutex>
#include <windows.h>
#include <memory>

class MemoryDataView : public QTableWidget
{
	Q_OBJECT
public:
	MemoryDataView(QWidget *parent);
	void SetBaseAddr(ULARGE_INTEGER Base_Addr);
	~MemoryDataView();
protected:
	void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;	
private:
	void Init();
	void UpdateData();
private:
	QMutex mutex;
	QVector<std::shared_ptr<QTableWidgetItem>> datas;
	quint64 volatile Base_Addr;
	size_t timeID;
};
