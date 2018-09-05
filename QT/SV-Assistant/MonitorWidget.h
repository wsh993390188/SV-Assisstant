#pragma once

#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <QString>
#include <QTextBrowser>

class MonitorWidget : public QWidget
{
	Q_OBJECT
public:
	MonitorWidget(QWidget *parent = nullptr);
	void Init();
	~MonitorWidget();
public slots:
	void UpdateEDID();
private:
	union EDIDManufacturer
	{
		struct
		{
			unsigned short _Third : 5;
			unsigned short _Second : 5;
			unsigned short _First : 5;
			unsigned short Reserved : 1;
		}bits;
		unsigned short __Manufacturer;
	};
	QComboBox * EDIDTitle;
	QTextBrowser * EDIDData;
	QVBoxLayout * mainlayout;
};