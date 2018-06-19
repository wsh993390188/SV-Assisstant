#ifndef SMBIOSTEXTBROWSER_H
#define SMBIOSTEXTBROWSER_H

#include <QTextBrowser>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QTreeWidgetItem>

class smbiostextbrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit smbiostextbrowser(QWidget *parent = nullptr);

signals:

public slots:
    void ReciveSMBIOSReady(bool ready);
    void ReciveSMBIOSloc(QTreeWidgetItem *item, int columns);
};

#endif // SMBIOSTEXTBROWSER_H
