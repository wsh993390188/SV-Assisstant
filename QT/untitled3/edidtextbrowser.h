#ifndef EDIDTEXTBROWSER_H
#define EDIDTEXTBROWSER_H

#include <QTextBrowser>
#include <QApplication>
#include <Qdir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>
#include "EDID.h"

class edidtextbrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit edidtextbrowser(QWidget *parent = nullptr);
	void UpdateData();
signals:
	void SendEdidTitle(QString x);
    void SendEdidName(QString x);
public slots:
	void ReciveEdidTitle(QString x);
private:
    void updateuiedid();
    union EDIDManufacturer
    {
        struct
        {
            WORD _Third : 5;
            WORD _Second : 5;
            WORD _First : 5;
            WORD Reserved : 1;
        }bits;
        WORD __Manufacturer;
    };
	QMap<QString, QTextCursor*> location;
};

#endif // EDIDTEXTBROWSER_H
