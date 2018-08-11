#include "DiskWidget.h"

DiskWidget::DiskWidget()
{
	QPalette pal(this->palette());
	//设置背景white色
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

DiskWidget::DiskWidget(QWidget *parent)
	: QWidget(parent)
{
	QPalette pal(this->palette());
	//设置背景white色
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->Init();
}

void DiskWidget::Init()
{

}

DiskWidget::~DiskWidget()
{
}
