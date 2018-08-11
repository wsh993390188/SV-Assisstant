#include "MonitorWidget.h"

MonitorWidget::MonitorWidget()
{
	QPalette pal(this->palette());
	//设置背景white色
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

MonitorWidget::MonitorWidget(QWidget *parent)
	: QWidget(parent)
{
	QPalette pal(this->palette());
	//设置背景white色
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

void MonitorWidget::Init()
{

}

MonitorWidget::~MonitorWidget()
{
}
