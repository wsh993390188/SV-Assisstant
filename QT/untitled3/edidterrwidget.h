#ifndef EDIDTERRWIDGET_H
#define EDIDTERRWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <Qlist>
#include <QString>
#include <QChar>
#include <QStringList>
#include <string>

class edidtreewidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit edidtreewidget(QWidget *parent = nullptr);

signals:
	void SendEdidTitle(QString x);
public slots:
	void ReciveEdidTitle(QString x);
private:
	QList<QTreeWidgetItem*> items;
	int currentEdid;
};

#endif // EDIDTERRWIDGET_H
