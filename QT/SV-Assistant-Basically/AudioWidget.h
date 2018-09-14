#pragma once

#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QItemDelegate>

class ReadOnlyDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	ReadOnlyDelegate(QObject *parent = 0) : QItemDelegate(parent) { }
	QWidget *createEditor(QWidget*parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const
	{
		return NULL;
	}
};

class AudioWidget : public QTableWidget
{
	Q_OBJECT

public:
	AudioWidget(QWidget *parent = nullptr);
	void Init();
	~AudioWidget();
};
