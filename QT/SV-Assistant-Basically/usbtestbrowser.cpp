#include "usbtestbrowser.h"
#include <QScrollBar>

usbtestbrowser::usbtestbrowser(QWidget *parent) : QTextBrowser(parent)
{
	this->setStyleSheet("QTextBrowser{color:#000000;font-family: \"consolas\"; font-size:14px;background-color: rgb(255, 255, 255);}");
	this->horizontalScrollBar()->setStyleSheet(
		"QScrollBar:horizontal{ background:#E4E4E4; max-height:12px;}"
		"QScrollBar::handle:horizontal{ background:#B6B6B6; min-height:50px; border-radius:6px; }"
		"QScrollBar::handle:horizontal:hover{ background:#00BB9E; }"
		"QScrollBar::handle:horizontal:pressed{ background:#00BB9E;}"
		"QScrollBar::add-page:horizontal{background:none;}"
		"QScrollBar::sub-page:horizontal{background:none;}"
		"QScrollBar::add-line:horizontal{background:none;}"
		"QScrollBar::sub-line:horizontal{background:none;}"
		"QScrollArea{border:0px; }");

	this->verticalScrollBar()->setStyleSheet(
		"QScrollBar:vertical{ background:#E4E4E4; padding:0px; border-radius:6px; max-width:12px;}"
		"QScrollBar::handle:vertical{ background:#B6B6B6; min-height:50px; border-radius:6px; }"
		"QScrollBar::handle:vertical:hover{	background:#00BB9E; }"
		"QScrollBar::handle:vertical:pressed{ background:#00BB9E;}"
		"QScrollBar::add-page:vertical{background:none;}"
		"QScrollBar::sub-page:vertical{background:none;}"
		"QScrollBar::add-line:vertical{background:none;}"
		"QScrollBar::sub-line:vertical{background:none;}"
		"QScrollArea{border:0px; }");
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
