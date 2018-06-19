#ifndef USBTESTBROWSER_H
#define USBTESTBROWSER_H

#include <QTextBrowser>
#include <QApplication>
#include <Qdir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class usbtestbrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit usbtestbrowser(QWidget *parent = nullptr);

signals:

public slots:
    void ReviceUsbInfo(QString msg);
};

#endif // USBTESTBROWSER_H
