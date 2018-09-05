#include "smbiostreewidget.h"

smbiostreewidget::smbiostreewidget(QWidget *parent) : QTreeWidget(parent), father(new QTreeWidgetItem(this, QStringList(tr("SMBIOS"))))
{
    this->setHeaderHidden(true);
}

void smbiostreewidget::UpdateData()
{
	SV_ASSIST::DMI_SMBIOS::Updatesmbios();
	currentdir.append(tr("\\HardWare\\DMI"));
	QDir dir(currentdir);
	if (!dir.exists())
	{
		qDebug() << currentdir << tr("No Exists");
		return;
	}
	QFile file(currentdir.append(tr("\\smbios.txt")));
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Can not open smbios.txt";
        return;
    }
    QTextStream in(&file);
    while(in.atEnd() == 0)
    {
       QString str = in.readLine();
       if(str.startsWith(QChar('[')))
       {
           QTreeWidgetItem *text = new QTreeWidgetItem(father, QStringList(str));
           Q_UNUSED(text);
       }
    }
    file.close();
    emit SMBIOSReady(true, currentdir);
}

void smbiostreewidget::SetCurrentdir(const QString x)
{
	currentdir = x;
}

smbiostreewidget::~smbiostreewidget()
{
	delete father;
	father = nullptr;
}


