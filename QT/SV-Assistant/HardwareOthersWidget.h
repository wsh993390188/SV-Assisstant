#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPalette>
#include <QVector>
#include <QString>
#include <QToolButton>

struct HardwareOthersBaseInfo
{
	QHBoxLayout* layout;
	QLabel* BaseLabel;
	QSpacerItem *horizontalSpace;
	QSpacerItem *BeginhorizontalSpace;
	HardwareOthersBaseInfo(QWidget* father);
};


class HardwareOthersWidget : public QWidget
{
	Q_OBJECT

public:
	HardwareOthersWidget(QWidget *parent = nullptr);
	~HardwareOthersWidget();
private:
	QVBoxLayout * mainlayout;
	QVector<HardwareOthersBaseInfo*> devs;
private:
	void Init();
};
