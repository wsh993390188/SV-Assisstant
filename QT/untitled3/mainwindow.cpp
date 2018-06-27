#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "diskselectbutton.h"
#include "cpuusageqcharts.h"
#include "NetworkLib.h"
#include "CAudio.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    gputimeID(0), timeID(0)
{
    QString dirName = qApp->applicationDirPath();
    dirName.append(tr("\\HardWare"));
    this->cleardir(dirName);
    ui->setupUi(this);
    this->Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::cleardir(const QString &dirName)
{
    QDir directory(dirName);
    if(!directory.exists())
    {
       return true;
    }
    QString srcPath = QDir::toNativeSeparators(dirName);
        if (!srcPath.endsWith(QDir::separator()))
            srcPath += QDir::separator();

        QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
        bool error = false;
        for (QStringList::size_type i=0; i != fileNames.size(); ++i)
        {
            QString filePath = srcPath + fileNames.at(i);
            QFileInfo fileInfo(filePath);
            if (fileInfo.isFile() || fileInfo.isSymLink())
            {
                QFile::setPermissions(filePath, QFile::WriteOwner);
                if (!QFile::remove(filePath))
                {
                    qDebug() << "remove file" << filePath << " faild!";
                    error = true;
                }
            }
            else if (fileInfo.isDir())
            {
                if (!cleardir(filePath))
                {
                    error = true;
                }
            }
        }

        if (!directory.rmdir(QDir::toNativeSeparators(directory.path())))
        {
            qDebug() << "remove dir" << directory.path() << " faild!";
            error = true;
        }

        return !error;
}

void MainWindow::Init()
{
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    ui->disktableWidget->setColumnCount(6);
    ui->disktableWidget->setHorizontalHeaderLabels(QStringList()<<"ID"<<"Attribute Name"<<"Current"<<"Worst"<<"Threshold"<<"Raw Values");
    connect(ui->maincpupushButton, &QPushButton::clicked, this, [&]{ui->mainrightstackedWidget->setCurrentIndex(0);});
    connect(ui->CPUbutton, &QToolButton::clicked, this, [&]{ui->hardwarestackedWidget->setCurrentIndex(0);});
    connect(ui->memorybutton, &QToolButton::clicked, this, [&]{ui->hardwarestackedWidget->setCurrentIndex(1);});
    connect(ui->GPUbutton, &QToolButton::clicked, this, [&]{ui->hardwarestackedWidget->setCurrentIndex(2);});
    connect(ui->EDIDbutton, &QToolButton::clicked, this, [&]{ui->hardwarestackedWidget->setCurrentIndex(3);});
    connect(ui->USBbutton, &QToolButton::clicked, this, [&]{ui->hardwarestackedWidget->setCurrentIndex(4);});
    connect(ui->Diskbutton, &QToolButton::clicked, this, [&]{ui->hardwarestackedWidget->setCurrentIndex(5);});
    connect(ui->SMBIOSbutton, &QToolButton::clicked, this, [&]{ui->hardwarestackedWidget->setCurrentIndex(6);});
    connect(ui->othersbutton, &QToolButton::clicked, this, [&]{ui->hardwarestackedWidget->setCurrentIndex(7);});
    connect(ui->rwIobutton, &QToolButton::clicked, this, [&]{ui->advencedhardwarestackedWidget->setCurrentIndex(0);});
    connect(ui->rwmemorybutton, &QToolButton::clicked, this, [&]{ui->advencedhardwarestackedWidget->setCurrentIndex(1);});
    connect(ui->rwmsrbutton, &QToolButton::clicked, this,[&]{ui->advencedhardwarestackedWidget->setCurrentIndex(2);});
    connect(ui->PCIbutton, &QToolButton::clicked, this, [&]{ui->advencedhardwarestackedWidget->setCurrentIndex(3);});
    connect(ui->comprehensivetoolButton, &QToolButton::clicked, this, [&]{ui->MainstackedWidget->setCurrentIndex(0);});
    connect(ui->hardwaretoolButton, &QToolButton::clicked, this, [&]{ui->MainstackedWidget->setCurrentIndex(1);});
    connect(ui->advencedhardwaretoolButton, &QToolButton::clicked, this, [&]
    {   QMessageBox::warning(NULL, "warning", "It is dangerious Operation! You really know what you are doing!!!", QMessageBox::Yes, QMessageBox::Yes);
        ui->MainstackedWidget->setCurrentIndex(2);
    });
    connect(ui->usbtreeWidget, &usbtreewidget::sendUsbInfo, ui->usbtextBrowser, &usbtestbrowser::ReviceUsbInfo);
    connect(ui->smbiostreeWidget,&smbiostreewidget::SMBIOSReady, ui->smbiostextBrowser, &smbiostextbrowser::ReciveSMBIOSReady);
    connect(ui->smbiostreeWidget, &smbiostreewidget::itemClicked, ui->smbiostextBrowser, &smbiostextbrowser::ReciveSMBIOSloc);
    connect(ui->minpushButton, &QPushButton::clicked, this, [&]{this->showMinimized();});

    connect(ui->rwiotableWidget, &RW_Qtablewidget::cellDoubleClicked, ui->rwiotableWidget, &RW_Qtablewidget::SetIOData);
    connect(ui->rwmemorytableWidget, &RW_Qtablewidget::cellDoubleClicked, ui->rwmemorytableWidget, &RW_Qtablewidget::SetMemoryData);
    connect(ui->iobasepushButton, &RWBaseButton::SendBaseAddr, ui->rwiotableWidget, &RW_Qtablewidget::ReceiveIOBase);
    connect(ui->memorybasepushButton, &RWBaseButton::SendBaseAddr, ui->rwmemorytableWidget, &RW_Qtablewidget::ReceiveMemoryBase);
    connect(ui->pcicomboBox, &pcicombobox::SendPCIName, ui->pcitableWidget, &pcitablewidget::RecivePCIName);

    connect(ui->edidtextBrowser, &edidtextbrowser::SendEdidTitle, ui->edidtreeWidget, &edidtreewidget::ReciveEdidTitle);
    connect(ui->edidtreeWidget, &edidtreewidget::SendEdidTitle, ui->edidtextBrowser, &edidtextbrowser::ReciveEdidTitle);
    connect(ui->edidtextBrowser, &edidtextbrowser::SendEdidName, this, [&](const QString x){ ui->mainmonitorlabel->setText(x);});

    connect(ui->msreadpushButton, &QPushButton::clicked, this, [&]
    {
        if(!ui->MSRADDRlineEdit->text().isEmpty())
        {
            uint ThreadAffinity = 0;
            if(!ui->MSRcorenumlineEdit->text().isEmpty())
                ThreadAffinity = ui->MSRcorenumlineEdit->text().toUInt();
            QString a = ui->MSRADDRlineEdit->text();
            DWORD64 Data = 0;
            if(SV_ASSIST::Ring0::RdMsrTx(a.toUInt(nullptr, 16), Data, ThreadAffinity))
            {
                ui->MSRDATAlineEdit->setText(QString::number(Data, 16).toUpper());
                int row_count = ui->rwmsrtableWidget->rowCount();
                ui->rwmsrtableWidget->insertRow(row_count); //插入新行
                ui->rwmsrtableWidget->setVerticalHeaderItem(row_count, new QTableWidgetItem(QString::number(row_count)));
                ui->rwmsrtableWidget->setItem(row_count, 0, new QTableWidgetItem(a));
                ui->rwmsrtableWidget->setItem(row_count, 1, new QTableWidgetItem(QString::number(Data, 16).toUpper()));
            }
        }
    });
    connect(ui->mstwritepushButton, &QPushButton::clicked, this, [&]
    {
        if(!ui->MSRADDRlineEdit->text().isEmpty() && !ui->MSRDATAlineEdit->text().isEmpty())
        {
            uint ThreadAffinity = 0;
            if(!ui->MSRcorenumlineEdit->text().isEmpty())
                ThreadAffinity = ui->MSRcorenumlineEdit->text().toUInt();
            QString a = ui->MSRADDRlineEdit->text();
            QString b = ui->MSRADDRlineEdit->text();
			SV_ASSIST::Ring0::WrMsrTx(a.toUInt(nullptr, 16), b.toUInt(nullptr, 16), ThreadAffinity);
        }
    });
    ui->msrcorenumpushButton->hide();
    QRegExp regx("^[a-fA-F0-9]{0,8}$");
    ui->MSRADDRlineEdit->setValidator(new QRegExpValidator(regx));
    ui->MSRDATAlineEdit->setValidator(new QRegExpValidator(regx));
    ui->rwmsrtableWidget->setColumnCount(2);
    ui->rwmsrtableWidget->setHorizontalHeaderLabels(QStringList()<< tr("Address")<< tr("Data"));
    ui->rwmsrtableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->rwmsrtableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->send();
	this->OtherUpdateData();
    this->diskupdate();
    this->gpuupdate();
	ui->smbiostreeWidget->UpdateData();
    ui->edidtextBrowser->UpdateData();
    timeID = this->startTimer(1000);
    gputimeID = this->startTimer(1000);

}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timeID && ui->rwIopage->isVisible())
    {
        ui->rwiotableWidget->setIOMapping();
    }
    else if(event->timerId() == this->timeID && ui->rwmemorypage->isVisible())
    {
        ui->rwmemorytableWidget->setMemoryMapping();
    }
    else if(event->timerId() == gputimeID && ui->gpupage->isVisible())
    {
        SV_ASSIST::GPU::UpdateDate();
        this->gpuupdate();
    }
}

void MainWindow::send()
{
    std::string t = SV_ASSIST::CPU::GetCPUName();
    QString cpuname(t.c_str());
    ui->cpunamelineEdit->setText(QString(t.c_str()));
    ui->maincpulabel->setText(QString(t.c_str()));
    int num = SV_ASSIST::CPU::GetFamily();
    QString x = QString("%1").arg(num);
    ui->familylineEdit->setText(QString::number(num, 10));
    num = SV_ASSIST::CPU::GetExtFamily();
    ui->extfamilylineEdit->setText(QString::number(num, 10));
    num = SV_ASSIST::CPU::GetModel();
    ui->modellineEdit->setText(QString::number(num, 10));
    num = SV_ASSIST::CPU::GetExtModel();
    ui->extmodellineEdit->setText(QString::number(num, 10));
    num = SV_ASSIST::CPU::GetRevision();
    ui->revisionlineEdit->setText(QString::number(num, 16).toUpper());
    num = SV_ASSIST::CPU::GetStepping();
    ui->steppinglineEdit->setText(QString::number(num, 10));
    t = SV_ASSIST::CPU::GetManufacturer();
    ui->manufacturelineEdit->setText(QString(t.c_str()));
    num = SV_ASSIST::CPU::GetCurrentClockSpeed();
    ui->currentclocklineEdit->setText(QString::number(num, 10) + tr("Mhz"));
    num = SV_ASSIST::CPU::GetExtClock();
    ui->busspeedlineEdit->setText(QString::number(num, 10) + tr("Mhz"));
    t = SV_ASSIST::CPU::GetProcessorID();
    ui->processoridlineEdit->setText(QString(t.c_str()));
    t = SV_ASSIST::CPU::GetSocketDesignation();
    ui->PackagelineEdit->setText(QString(t.c_str()));
    const Cache_info *a = SV_ASSIST::CPU::GetCache();
    if(cpuname.contains(tr("Intel")))
    {
        ui->l1datalineEdit->setText(QString::number(a[0].Cache_Size/1024, 10) + tr(" KB"));
        ui->l1datawaylineEdit->setText(QString::number(a[0].Cache_Ways, 10) + tr(" Ways"));
        ui->l1instlineEdit->setText(QString::number(a[1].Cache_Size/1024, 10) + tr(" KB"));
        ui->l1instwaylineEdit->setText(QString::number(a[1].Cache_Ways, 10) + tr(" Ways"));
        ui->l2datalineEdit->setText(QString::number(a[2].Cache_Size/1024, 10) + tr(" KB"));
        ui->l2datawaylineEdit->setText(QString::number(a[2].Cache_Ways, 10) + tr(" Ways"));
        ui->l3datalineEdit->setText(QString::number(a[3].Cache_Size/1024/1024, 10) + tr(" MB"));
        ui->l3datawaylineEdit->setText(QString::number(a[3].Cache_Ways, 10) + tr(" Ways"));
    }
    else
    {
        ui->l1datalineEdit->setText(QString::number(a[0].Cache_Size, 10) + tr(" KB"));
        ui->l1datawaylineEdit->setText(QString::number(a[0].Cache_Ways, 10) + tr(" Ways"));
        ui->l1instlineEdit->setText(QString::number(a[1].Cache_Size, 10) + tr(" KB"));
        ui->l1instwaylineEdit->setText(QString::number(a[1].Cache_Ways, 10) + tr(" Ways"));
        ui->l2datalineEdit->setText(QString::number(a[2].Cache_Size, 10) + tr(" MB"));
        ui->l2datawaylineEdit->setText(QString::number(a[2].Cache_Ways, 10) + tr(" Ways"));
        ui->l3datalineEdit->setText(QString::number(a[3].Cache_Size, 10) + tr(" MB"));
        ui->l3datawaylineEdit->setText(QString::number(a[3].Cache_Ways, 10) + tr(" Ways"));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //屏蔽全选，这里最好不要使用Ctrl+A，因为不一点全选都被设置为它，所以使用系统的比较好
    if(event->matches(QKeySequence::SelectAll))
    {
       return ;
    }

   //屏蔽复制，和全选一样，这里最好不要使用Ctrl+A

    else if(event->matches(QKeySequence::Copy))
    {
       return ;
    }

   //屏蔽粘贴
    else if(event->matches(QKeySequence::Paste))
    {
       return ;
    }
//    //设置Home、End键直接将光标位置设到最前、最后，而不被选中
//    else if(event->key() == Qt::Key_Home)
//    {
//       this->setCursorPosition(0);
//    }
//    else if(event->key() == Qt::Key_End)
//    {
//       this->setCursorPosition(this->text().length());
//    }
    else
    {
       QWidget::keyPressEvent(event);
    }
}

void MainWindow::diskupdate()
{
    SV_ASSIST::Storage::updatediskinfo();
    QString currentdir = qApp->applicationDirPath();
    currentdir.append(tr("\\HardWare\\Disk"));
    QDir dir(currentdir);
    if(!dir.exists())
    {
       qDebug() << currentdir << tr("No Exists");
       return;
    }
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int file_count = list.count();
    if(file_count <= 2)
    {
        return;
    }

    QStringList string_name;
    for(int i = 0; i < file_count; i++)
    {
        ui->disktableWidget->setRowCount(0);
        if(i == 0 || i == 1)    continue;
        QFileInfo file_info = list.at(i);
        QString absolute_file_path = file_info.absoluteFilePath();
        string_name.append(file_info.fileName());
        QDir dirinfo(absolute_file_path);
        if(!dirinfo.exists())
        {
           qDebug() << dirinfo << tr("No Exists");
           return;
        }
        dirinfo.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList infolist = dirinfo.entryInfoList();
        int file_count = infolist.count();
        if(file_count <= 0)
        {
            return;
        }
        diskselectbutton * diskbutton = new diskselectbutton(this);
        diskbutton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        for(auto& j : infolist)
        {
            if(j.fileName() == tr("DiskName"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                QString disknamestring = in.readLine();
                diskbutton->setText(disknamestring);
                ui->maindisklabel->setText(disknamestring);
                ui->disknamelabel->setText(disknamestring);
                this->DiskButtonSelect.insert(disknamestring, diskbutton);
                file.close();
            }
            else if(j.fileName() == tr("DiskSize"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->disksizelineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("Firmware"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskfirenamelineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("Interface"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskinterfacelineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("PowerOnCount"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskpoweroncountlineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("PowerOnHours"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskpoweronhourslineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("SerialNumber"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskserialnumlineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("Standard"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskstandardlineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("TransferMode"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->disktransfermodelineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("Temperature"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->disktemperaturelabel->setText(tr("Temperature:") + in.readLine());
                file.close();
            }
            else
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                int i = 0;
                int row_count = ui->disktableWidget->rowCount(); //获取表单行数
                ui->disktableWidget->insertRow(row_count); //插入新行
                ui->disktableWidget->setItem(row_count, i++,new QTableWidgetItem(j.fileName().toUpper()));
                ui->disktableWidget->setItem(row_count, i++,new QTableWidgetItem("Zhanwei"));
                QTextStream in(&file);
                while(in.atEnd() == 0)
                {
                    QString tempstream = in.readLine().toUpper();
                    ui->disktableWidget->setItem(row_count, i++,new QTableWidgetItem(tempstream));
                }
                file.close();
            }
        }
        ui->diskselecthorizontalLayout->addWidget(diskbutton);
        connect(diskbutton, &diskselectbutton::SendButtonName, this, &MainWindow::on_disktoolButton_clicked);
    }
    ui->diskselecthorizontalLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

//    ui->disktableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);   //禁止修改
}

void MainWindow::gpuupdate()
{
	std::string x;
    x = SV_ASSIST::GPU::GPUName();
    ui->gpunamelineEdit->setText(QString(x.c_str()));
    ui->maingpulabel->setText(QString(x.c_str()));
    x = SV_ASSIST::GPU::GPUBIOSVersion();
    ui->gpubiosversionlineEdit->setText(QString(x.c_str()));
    double y = 0;
    int yy = 0;
    y = SV_ASSIST::GPU::GPUtemperature();
    ui->gputemperaturelineEdit->setText(QString::number(y));
    SV_ASSIST::GPU::FanTypes type;
    yy = SV_ASSIST::GPU::GPUFans(type);
    QString fanstypestring;
    (type == SV_ASSIST::GPU::FanTypes::RPM) ? fanstypestring = tr(" RPM") : fanstypestring = tr(" %");
    ui->gpufanslineEdit->setText(QString::number(yy) + fanstypestring);
    y = SV_ASSIST::GPU::GPUCurrentClock();
    ui->gpucurrentclocklineEdit->setText(QString::number(y) + tr(" Mhz"));
    y = SV_ASSIST::GPU::GPUMemoryClock();
    ui->gpumemoryclocklineEdit->setText(QString::number(y) + tr(" Mhz"));
    double z;
    z = y = SV_ASSIST::GPU::GPUPhysicalMemory();
    ui->gpuphysicalmemorylineEdit->setText(QString::number(y) + tr(" MB"));
    y = SV_ASSIST::GPU::GPUVirtualMemory();
    ui->gpusharedsystemmemorylineEdit->setText(QString::number(y) + tr(" MB"));
    y = SV_ASSIST::GPU::GetGPUAvailableMemory();
    ui->gpuavailablememorylineEdit->setText(QString::number(y) + tr(" MB"));
    ui->gpuusedmemorylineEdit->setText(QString::number(z-y) + tr(" MB"));
}

void MainWindow::OtherUpdateData()
{
	ui->AudiotableWidget->setColumnCount(5);
	ui->AudiotableWidget->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("ID") << tr("State") << tr("Desc") << tr("Audioif"));
	ui->AudiotableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->AudiotableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->AudiotableWidget->setHidden(false);
	ui->AudiotableWidget->setContentsMargins(0, 0, 0, 0);

	ui->NetworktableWidget->setColumnCount(4);
	ui->NetworktableWidget->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("MediaType") << tr("State") << tr("DeviceName"));
	ui->NetworktableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->NetworktableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->NetworktableWidget->setHidden(false);
	ui->NetworktableWidget->setContentsMargins(0, 0, 0, 0);
	SV_ASSIST::AUDIO::Exec();
	auto asd = SV_ASSIST::AUDIO::GetOutputAudio();
	if (!asd.empty())
	{
		QString name = {};
		for (const auto& i : asd)
		{
			name = QString::fromStdWString(i.name);
			if(name.contains(tr("NVIDIA")) || name.contains(tr("AMD")))
				break;
		}
		ui->mainaudiolabel->setText(name);
		int row_count = -1;
		int count = 0;
		for (const auto& i : asd)
		{
			row_count = ui->AudiotableWidget->rowCount();
			ui->AudiotableWidget->insertRow(row_count); //插入新行
			ui->AudiotableWidget->setVerticalHeaderItem(row_count, new QTableWidgetItem(QString::number(++count)));
			ui->AudiotableWidget->setItem(row_count, 0, new QTableWidgetItem(QString::fromStdWString(i.name)));
			ui->AudiotableWidget->setItem(row_count, 1, new QTableWidgetItem(QString::fromStdWString(i.id)));
			ui->AudiotableWidget->setItem(row_count, 2, new QTableWidgetItem(QString::fromStdWString(i.state)));
			ui->AudiotableWidget->setItem(row_count, 3, new QTableWidgetItem(QString::fromStdWString(i.desc)));
			ui->AudiotableWidget->setItem(row_count, 4, new QTableWidgetItem(QString::fromStdWString(i.audioif)));
		}
	}
	SV_ASSIST::Net::Exec();
	auto a = SV_ASSIST::Net::GetData();
	this->updateMemory();
	if (!a.empty())
	{
		ui->mainnetlabel->setText(QString::fromStdWString(a.at(0).DeviceName));
		int row_count = -1;
		int count = 0;
		for (const auto& i : a)
		{
			row_count = ui->NetworktableWidget->rowCount();
			ui->NetworktableWidget->insertRow(row_count); //插入新行
			ui->NetworktableWidget->setVerticalHeaderItem(row_count, new QTableWidgetItem(QString::number(++count)));
			ui->NetworktableWidget->setItem(row_count, 0, new QTableWidgetItem(QString::fromStdWString(i.Name)));
			ui->NetworktableWidget->setItem(row_count, 1, new QTableWidgetItem(QString::fromStdWString(i.MediaType)));
			ui->NetworktableWidget->setItem(row_count, 2, new QTableWidgetItem(QString::fromStdWString(i.state)));
			ui->NetworktableWidget->setItem(row_count, 3, new QTableWidgetItem(QString::fromStdWString(i.DeviceName)));
		}
	}
}

void MainWindow::updateMemory()
{
    DDR3_INFO ddr3 = {};
	USHORT VendorID = SV_ASSIST::Ring0::GetPCIVendorID();
	USHORT SMBASE = 0;
	if(SV_ASSIST::Ring0::GetSMbusBaseAddr(VendorID, SMBASE))
    //GetSMbusBaseAddr();
	{
        if(SV_ASSIST::Ring0::ReadSPD(SMBASE, 0xA0, ddr3))
        {
            char a[20] = {};
            memcpy(a, ddr3.PartNumber, 18);
            ui->mempartnumberlineEdit->setText(QString(a).toUpper());

            switch (ddr3.sDRAMType) {
            case 0x01:
                ui->memtypelineEdit->setText(tr("Standard FPM DRAM"));
                break;
            case 0x02:
                ui->memtypelineEdit->setText(tr("EDO"));
                break;
            case 0x03:
                ui->memtypelineEdit->setText(tr("Pipelined Nibble"));
                break;
            case 0x04:
                ui->memtypelineEdit->setText(tr("SDRAM"));
                break;
            case 0x05:
                ui->memtypelineEdit->setText(tr("ROM"));
                break;
            case 0x06:
                ui->memtypelineEdit->setText(tr("DDR SGRAM"));
                break;
            case 0x07:
                ui->memtypelineEdit->setText(tr("DDR SDRAM"));
                break;
            case 0x08:
                ui->memtypelineEdit->setText(tr("DDR2 SDRAM"));
                break;
            case 0x09:
                ui->memtypelineEdit->setText(tr("DDR2 SDRAM FB-DIMM"));
                break;
            case 0x0A:
                ui->memtypelineEdit->setText(tr("DDR2 SDRAM FB-DIMM PROBE"));
                break;
            case 0x0B:
                ui->memtypelineEdit->setText(tr("DDR3 SDRAM"));
                break;
            case 0x0C:
                ui->memtypelineEdit->setText(tr("DDR4 SDRAM"));
                break;
            default:
                ui->memtypelineEdit->setText(tr("unknown"));
                break;
            }

            ZeroMemory(a, 20);
            memcpy(a, ddr3.SerialNumber, 4);
			qlonglong serialnum = *reinterpret_cast<qlonglong*>(a);
            ui->memserialnumberlineEdit->setText(QString::number(serialnum,16).toUpper());
            int year = ddr3.ManufacturingDate[0], week = ddr3.ManufacturingDate[1];
            ui->memweekyearlineEdit->setText(QString(tr("%1\\%2").arg(week).arg(year)));

            int capacity = ddr3.sSDRAMBank_Density.bits.Capacity,Ranks = ddr3.sModuleOrganization.bits.Number_of_Ranks,\
                Primarybuswidth = ddr3.sBusWidth.bits.Primary_bus_width,SDRAMWidth = ddr3.sModuleOrganization.bits.SDRAM_Depth_Width;
				Ranks++;
				switch(capacity)
				{
					case 0:
						break;
					case 0x01:
						break;
					case 0x02:
					capacity = 1;
						break;
					case 0x03:
					capacity = 2;
						break;
					case 0x04:
					capacity = 3;
						break;
					case 0x05:
					capacity = 8;
						break;
					case 0x06:
					capacity = 16;
						break;
					default:
						break;
				}
				switch(SDRAMWidth)
				{
					case 0:
					SDRAMWidth = 4;
						break;
					case 0x01:
					SDRAMWidth = 8;
						break;
					case 0x02:
					SDRAMWidth = 16;
						break;
					case 0x03:
					SDRAMWidth = 32;
						break;
					default:
						break;
				}
				switch(Primarybuswidth)
				{
					case 0:
					Primarybuswidth = 8;
						break;
					case 0x01:
					Primarybuswidth = 16;
						break;
					case 0x02:
					Primarybuswidth = 32;
						break;
					case 0x03:
					Primarybuswidth = 64;
						break;
					default:
						break;
				}
			
			
            double DDRSize = capacity / 8.0 * Primarybuswidth / SDRAMWidth * Ranks;
            ui->memmodelsizelineEdit->setText(QString::number(DDRSize) + tr("GByte"));
        }
	}
	else
	{
		qDebug() << "Get Memory Info Wrong!";
	}
}

void MainWindow::on_disktoolButton_clicked(QString x)
{
    qDebug() << x;
    QString currentdir = qApp->applicationDirPath();
    currentdir.append(tr("\\HardWare\\Disk"));
    QDir dir(currentdir);
    if(!dir.exists())
    {
       qDebug() << currentdir << tr("No Exists");
       return;
    }
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int file_count = list.count();
    if(file_count <= 2)
    {
        return;
    }
    for(int i = 0; i < file_count; i++)
    {
        if(i == 0 || i == 1)    continue;
        QFileInfo file_info = list.at(i);
        QString absolute_file_path = file_info.absoluteFilePath();
        QString file_path = file_info.fileName();
        if(file_path != x)  continue;
        QDir dirinfo(absolute_file_path);
        if(!dirinfo.exists())
        {
           qDebug() << dirinfo << tr("No Exists");
           break;
        }
        dirinfo.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList infolist = dirinfo.entryInfoList();
        int file_count = infolist.count();
        if(file_count <= 0)
        {
           break;
        }
        ui->disktableWidget->setRowCount(0);
        for(auto& j : infolist)
        {
            if(j.fileName() == tr("DiskName"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                QString disknamestring = in.readLine();
                ui->disknamelabel->setText(disknamestring);
                file.close();
            }
            else if(j.fileName() == tr("DiskSize"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->disksizelineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("Firmware"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskfirenamelineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("Interface"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskinterfacelineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("PowerOnCount"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskpoweroncountlineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("PowerOnHours"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskpoweronhourslineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("SerialNumber"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskserialnumlineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("Standard"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->diskstandardlineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("TransferMode"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->disktransfermodelineEdit->setText(in.readLine());
                file.close();
            }
            else if(j.fileName() == tr("Temperature"))
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                QTextStream in(&file);
                ui->disktemperaturelabel->setText(tr("Temperature:") + in.readLine());
                file.close();
            }
            else
            {
                QFile file(j.absoluteFilePath());
                if(!file.open(QFile::ReadOnly | QFile::Text))
                {
                    qDebug() << "Can not open" << j.fileName();
                    continue;
                }
                int i = 0;
                int row_count = ui->disktableWidget->rowCount(); //获取表单行数
                ui->disktableWidget->insertRow(row_count); //插入新行
                ui->disktableWidget->setItem(row_count, i++,new QTableWidgetItem(j.fileName().toUpper()));
                ui->disktableWidget->setItem(row_count, i++,new QTableWidgetItem("Zhanwei"));
                QTextStream in(&file);
                while(in.atEnd() == 0)
                {
                    QString tempstream = in.readLine().toUpper();
                    ui->disktableWidget->setItem(row_count, i++,new QTableWidgetItem(tempstream));
                }
                file.close();
            }
        }
        break;
    }
}
