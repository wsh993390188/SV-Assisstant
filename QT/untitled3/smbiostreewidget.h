#ifndef SMBIOSTREEWIDGET_H
#define SMBIOSTREEWIDGET_H

#include <QDir>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QString>
#include <QChar>
#include <QStringList>
#include "smbioslib.h"

class smbiostreewidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit smbiostreewidget(QWidget *parent = nullptr);
    void UpdateData();
	void SetCurrentdir(const QString x);
signals:
    void SMBIOSReady(bool, QString);
public slots:
private:
	QString currentdir;
	QTreeWidgetItem *father;
	~smbiostreewidget();
};

#endif // SMBIOSTREEWIDGET_H
