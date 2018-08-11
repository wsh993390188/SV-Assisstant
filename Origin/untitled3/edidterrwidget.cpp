#include "edidterrwidget.h"

edidtreewidget::edidtreewidget(QWidget *parent) : QTreeWidget(parent), currentEdid(0)
{
    this->setHeaderHidden(true);
	connect(this, &QTreeWidget::itemClicked, this, [&](QTreeWidgetItem * item, int column)
	{	Q_UNUSED(column)	emit SendEdidTitle(item->text(0)); });
}

void edidtreewidget::ReciveEdidTitle(QString x)
{
	if (x.contains(tr("EDID")))
	{
		std::wstring a = x.toStdWString();
		swscanf(a.c_str(), L"EDID ( Extended Display Identification Data) Report #%d", &currentEdid);
		QTreeWidgetItem *text = new QTreeWidgetItem(this, QStringList(tr("EDID #%1").arg(currentEdid)));
		items.append(text);
	}
	else
	{

		if (currentEdid > 0 && currentEdid <= items.size())
		{
			QTreeWidgetItem *parent = items.at(currentEdid - 1);
			QTreeWidgetItem *text = new QTreeWidgetItem(parent, QStringList(x));
		}

	}
}
