#include "usbtestbrowser.h"

usbtestbrowser::usbtestbrowser(QWidget *parent) : QTextBrowser(parent)
{

}

void usbtestbrowser::ReviceUsbInfo(QString x)
{
    this->clear();
    QString currentdir = qApp->applicationDirPath();
    currentdir.append(tr("\\HardWare\\Usb"));
    QDir dir(currentdir);
    if(!dir.exists())
    {
        dir.mkpath(currentdir);//创建多级目录
    }

    QFile file(currentdir + tr("\\") + x);
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        while(in.atEnd() == 0)
        {
           this->setText(in.readAll());           
        }
        file.close();
    }
    else
    {
        qDebug() << "Read " << x << "Failed";
        this->setText(x);
    }

}
