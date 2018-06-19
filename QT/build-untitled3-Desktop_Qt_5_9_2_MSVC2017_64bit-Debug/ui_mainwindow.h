/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "closebutton.h"
#include "cpuusageqcharts.h"
#include "edidterrwidget.h"
#include "edidtextbrowser.h"
#include "mainctrlbutton.h"
#include "pcicombobox.h"
#include "pcitablewidget.h"
#include "rw_qtablewidget.h"
#include "rwbasebutton.h"
#include "settingbutton.h"
#include "smbiostextbrowser.h"
#include "smbiostreewidget.h"
#include "sv_hardware_pushbutton.h"
#include "usbtestbrowser.h"
#include "usbtreewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_18;
    QWidget *titlewidget;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *comprehensivetoolButton;
    QToolButton *hardwaretoolButton;
    QToolButton *advencedhardwaretoolButton;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_20;
    QHBoxLayout *maincontrolhorizontalLayout;
    settingbutton *settingtoolButton;
    mainctrlbutton *minpushButton;
    closebutton *closepushButton;
    QSpacerItem *verticalSpacer;
    QStackedWidget *MainstackedWidget;
    QWidget *comprehensivepage;
    QHBoxLayout *horizontalLayout_33;
    QFrame *mainleftframe;
    QVBoxLayout *verticalLayout_22;
    QVBoxLayout *mainleftframeheadverticalLayout;
    QHBoxLayout *maincomputertypehorizontalLayout;
    QLabel *maincomputertypelabel;
    QLabel *maincomputertypedatalabel;
    QHBoxLayout *mainsystemhorizontalLayout;
    QLabel *mainsystemlabel;
    QLabel *mainsystemdatalabel;
    QVBoxLayout *mainleftframefootverticalLayout;
    QHBoxLayout *maincpuhorizontalLayout;
    QPushButton *maincpupushButton;
    QLabel *maincpulabel;
    QHBoxLayout *mainboardhorizontalLayout;
    QPushButton *mainboardpushButton;
    QLabel *mainboardlabel;
    QHBoxLayout *mainmemoryhorizontalLayout;
    QPushButton *mainmemorypushButton;
    QLabel *mainmemorylabel;
    QHBoxLayout *maindiskhorizontalLayout;
    QPushButton *maindiskpushButton;
    QLabel *maindisklabel;
    QHBoxLayout *maindiskhorizontalLayout_2;
    QPushButton *maingpupushButton;
    QLabel *maingpulabel;
    QHBoxLayout *mainmonitorhorizontalLayout;
    QPushButton *mainmonitorpushButton;
    QLabel *mainmonitorlabel;
    QHBoxLayout *mainaudiohorizontalLayout;
    QPushButton *mainaudiopushButton;
    QLabel *mainaudiolabel;
    QHBoxLayout *mainnethorizontalLayout;
    QPushButton *mainnetpushButton;
    QLabel *mainnetlabel;
    QStackedWidget *mainrightstackedWidget;
    QWidget *mainrightstackedWidgetcpuPage;
    QVBoxLayout *mainrightstackedWidgetcpuPageverticalLayout;
    QSpacerItem *cpuusageverticalSpacer;
    cpuusageQcharts *cpuusagewidget;
    QWidget *hardwarepage;
    QHBoxLayout *horizontalLayout_10;
    QWidget *functionwidget;
    QVBoxLayout *hardwarebuttonverticalLayout;
    SV_HARDWARE_PUSHBUTTON *CPUbutton;
    SV_HARDWARE_PUSHBUTTON *memorybutton;
    SV_HARDWARE_PUSHBUTTON *GPUbutton;
    SV_HARDWARE_PUSHBUTTON *EDIDbutton;
    SV_HARDWARE_PUSHBUTTON *USBbutton;
    SV_HARDWARE_PUSHBUTTON *Diskbutton;
    SV_HARDWARE_PUSHBUTTON *SMBIOSbutton;
    SV_HARDWARE_PUSHBUTTON *othersbutton;
    QStackedWidget *hardwarestackedWidget;
    QWidget *CPUPage;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *processorgroupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *packagehorizontalLayout;
    QLabel *Package;
    QSpacerItem *PackagelefthorizontalSpacer;
    QLineEdit *PackagelineEdit;
    QSpacerItem *PackagerighthorizontalSpacer;
    QHBoxLayout *manufacturehorizontalLayout;
    QLabel *Manufacture;
    QSpacerItem *manufacturlefthorizontalSpacer;
    QLineEdit *manufacturelineEdit;
    QSpacerItem *manufacturerighthorizontalSpacer;
    QHBoxLayout *processoridhorizontalLayout;
    QLabel *processorid;
    QSpacerItem *processoridlefthorizontalSpacer;
    QLineEdit *processoridlineEdit;
    QSpacerItem *processoridhorizontalSpacer;
    QHBoxLayout *InstructionshorizontalLayout;
    QLabel *Instructions;
    QSpacerItem *InstructionshorizontalSpacer;
    QLineEdit *InstructionslineEdit;
    QVBoxLayout *fmsverticalLayout;
    QHBoxLayout *fmshorizontalLayout;
    QLabel *Family;
    QSpacerItem *familylefthorizontalSpacer;
    QLineEdit *familylineEdit;
    QSpacerItem *familyrighthorizontalSpacer;
    QLabel *Model;
    QSpacerItem *modellefthorizontalSpacer;
    QLineEdit *modellineEdit;
    QSpacerItem *modelrighthorizontalSpacer;
    QLabel *stepping;
    QSpacerItem *steplefthorizontalSpacer;
    QLineEdit *steppinglineEdit;
    QSpacerItem *steprighthorizontalSpacer;
    QHBoxLayout *technologyhorizontalLayout;
    QLabel *Techonology;
    QSpacerItem *technologylefthorizontalSpacer;
    QLineEdit *technologylineEdit;
    QSpacerItem *technologyrighthorizontalSpacer;
    QHBoxLayout *extfmrhorizontalLayout;
    QLabel *Extfamily;
    QSpacerItem *extfamilylefthorizontalSpacer;
    QLineEdit *extfamilylineEdit;
    QSpacerItem *extfamilyrighthorizontalSpacer;
    QLabel *extmodel;
    QSpacerItem *extmodelefthorizontalSpacer;
    QLineEdit *extmodellineEdit;
    QSpacerItem *extmodelrighthorizontalSpacer;
    QLabel *revision;
    QSpacerItem *revisionlefthorizontalSpacer;
    QLineEdit *revisionlineEdit;
    QSpacerItem *revisionrifhthorizontalSpacer;
    QHBoxLayout *cpunamelayout;
    QLabel *CPUNAME;
    QSpacerItem *cpunamelefthorizontalSpacer;
    QLineEdit *cpunamelineEdit;
    QSpacerItem *cpunamerighthorizontalSpacer;
    QLabel *cpuiconlabel;
    QHBoxLayout *horizontalLayout_8;
    QLabel *CPUNAME_7;
    QSpacerItem *horizontalSpacer_67;
    QLineEdit *cpunamelineEdit_7;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *codenamehorizontalLayout;
    QLabel *CodeName;
    QSpacerItem *codenamelefthorizontalSpacer;
    QLineEdit *codenamelineEdit;
    QSpacerItem *codenametighthorizontalSpacer;
    QHBoxLayout *cpucacheclockhorizontalLayout;
    QGroupBox *clockgroupBox;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *currentclockhorizontalLayout;
    QLabel *CurrentClock;
    QLineEdit *currentclocklineEdit;
    QSpacerItem *currentclockhorizontalSpacer;
    QHBoxLayout *buspeedhorizontalLayout;
    QLabel *busspeed;
    QLineEdit *busspeedlineEdit;
    QSpacerItem *busspeedhorizontalSpacer;
    QHBoxLayout *horizontalLayout_18;
    QLabel *CPUNAME_16;
    QLineEdit *cpunamelineEdit_16;
    QSpacerItem *horizontalSpacer_16;
    QHBoxLayout *horizontalLayout_19;
    QLabel *CPUNAME_17;
    QLineEdit *cpunamelineEdit_17;
    QSpacerItem *horizontalSpacer_17;
    QGroupBox *cachegroupBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *l1insthorizontalLayout;
    QLabel *L1inst;
    QLineEdit *l1instlineEdit;
    QSpacerItem *l1instlefthorizontalSpacer;
    QLineEdit *l1instwaylineEdit;
    QSpacerItem *l1instrighthorizontalSpacer;
    QHBoxLayout *l1datahorizontalLayout;
    QLabel *l1data;
    QLineEdit *l1datalineEdit;
    QSpacerItem *l1datalefthorizontalSpacer;
    QLineEdit *l1datawaylineEdit;
    QSpacerItem *l1datarighthorizontalSpacer;
    QHBoxLayout *l2datahorizontalLayout;
    QLabel *l2data;
    QLineEdit *l2datalineEdit;
    QSpacerItem *l2datalefthorizontalSpacer;
    QLineEdit *l2datawaylineEdit;
    QSpacerItem *l2datarighthorizontalSpacer;
    QHBoxLayout *l3horizontalLayout;
    QLabel *l3data;
    QLineEdit *l3datalineEdit;
    QSpacerItem *l3datalefthorizontalSpacer;
    QLineEdit *l3datawaylineEdit;
    QSpacerItem *l3datarighthorizontalSpacer;
    QWidget *MemoryPage;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *memoryinfogroupBox;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_39;
    QScrollArea *memmoryslotscrollArea;
    QWidget *slotscrollAreaWidgetContents;
    QHBoxLayout *memtypehorizontalLayout;
    QLabel *memtype;
    QLineEdit *memtypelineEdit;
    QSpacerItem *memtypehorizontalSpacer;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_20;
    QLabel *memModelSize;
    QLineEdit *memmodelsizelineEdit;
    QSpacerItem *horizontalSpacer_18;
    QHBoxLayout *horizontalLayout_21;
    QLabel *memmaxbrandwidth;
    QLineEdit *memmaxbrandwidthlineEdit;
    QSpacerItem *horizontalSpacer_19;
    QHBoxLayout *horizontalLayout_22;
    QLabel *memmanufacturer;
    QLineEdit *memmanufacturerlineEdit;
    QSpacerItem *horizontalSpacer_20;
    QHBoxLayout *horizontalLayout_23;
    QLabel *mempartnumber;
    QLineEdit *mempartnumberlineEdit;
    QSpacerItem *horizontalSpacer_21;
    QHBoxLayout *horizontalLayout_24;
    QLabel *memserialnumber;
    QLineEdit *memserialnumberlineEdit;
    QSpacerItem *horizontalSpacer_22;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_25;
    QLabel *memCorrection;
    QLineEdit *memcorrectionlineEdit;
    QSpacerItem *horizontalSpacer_23;
    QHBoxLayout *horizontalLayout_38;
    QLabel *memRegistered;
    QLineEdit *memregisteredlineEdit;
    QSpacerItem *horizontalSpacer_44;
    QHBoxLayout *horizontalLayout_26;
    QLabel *memRanks;
    QLineEdit *memrankslineEdit;
    QSpacerItem *horizontalSpacer_24;
    QHBoxLayout *horizontalLayout_36;
    QLabel *memspdext;
    QLineEdit *memspdextlineEdit;
    QSpacerItem *horizontalSpacer_42;
    QHBoxLayout *horizontalLayout_37;
    QLabel *memweekyear;
    QLineEdit *memweekyearlineEdit;
    QSpacerItem *horizontalSpacer_43;
    QGroupBox *timingtablegroupBox;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_41;
    QLabel *ModelSize_2;
    QLineEdit *modelsizelineEdit_2;
    QSpacerItem *horizontalSpacer_46;
    QHBoxLayout *horizontalLayout_42;
    QLabel *maxbrandwidth_3;
    QLineEdit *maxbrandwidthlineEdit_3;
    QSpacerItem *horizontalSpacer_47;
    QHBoxLayout *horizontalLayout_43;
    QLabel *memmanufacturer_3;
    QLineEdit *memmanufacturerlineEdit_3;
    QSpacerItem *horizontalSpacer_48;
    QHBoxLayout *horizontalLayout_44;
    QLabel *mempartnumber_3;
    QLineEdit *mempartnumberlineEdit_3;
    QSpacerItem *horizontalSpacer_49;
    QHBoxLayout *horizontalLayout_45;
    QLabel *memserialnumber_3;
    QLineEdit *memserialnumberlineEdit_3;
    QSpacerItem *horizontalSpacer_50;
    QVBoxLayout *verticalLayout_12;
    QHBoxLayout *horizontalLayout_46;
    QLabel *Correction_2;
    QLineEdit *correctionlineEdit_2;
    QSpacerItem *horizontalSpacer_51;
    QHBoxLayout *horizontalLayout_47;
    QLabel *Registered_2;
    QLineEdit *registeredlineEdit_2;
    QSpacerItem *horizontalSpacer_52;
    QHBoxLayout *horizontalLayout_48;
    QLabel *Ranks_2;
    QLineEdit *rankslineEdit_2;
    QSpacerItem *horizontalSpacer_53;
    QHBoxLayout *horizontalLayout_49;
    QLabel *spdext_2;
    QLineEdit *spdextlineEdit_2;
    QSpacerItem *horizontalSpacer_54;
    QHBoxLayout *horizontalLayout_50;
    QLabel *weekyear_2;
    QLineEdit *weekyearlineEdit_2;
    QSpacerItem *horizontalSpacer_55;
    QWidget *gpupage;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *gpupageverticalLayout_1;
    QVBoxLayout *gpupageverticalLayout_1_2;
    QHBoxLayout *gpunamehorizontalLayout;
    QLabel *gpuname;
    QLineEdit *gpunamelineEdit;
    QSpacerItem *gpunamehorizontalSpacer;
    QHBoxLayout *gpubiosversionhorizontalLayout;
    QLabel *gpubiosversion;
    QLineEdit *gpubiosversionlineEdit;
    QSpacerItem *gpubiosversionhorizontalSpacer;
    QVBoxLayout *gpupageverticalLayout_1_1;
    QHBoxLayout *gpucurrentclockhorizontalLayout;
    QLabel *gpucurrentclock;
    QLineEdit *gpucurrentclocklineEdit;
    QSpacerItem *gpucurrentclockhorizontalSpacer;
    QHBoxLayout *gpudedicatedmemoryhorizontalLayout;
    QLabel *gpuavailablememory;
    QLineEdit *gpuavailablememorylineEdit;
    QSpacerItem *gpudedicatedmemoryhorizontalSpacer;
    QHBoxLayout *gpusystemmemoryhorizontalLayout;
    QLabel *gpuvirtualmemory;
    QLineEdit *gpuusedmemorylineEdit;
    QSpacerItem *gpusystemmemoryhorizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *gpupageverticalLayout_2;
    QVBoxLayout *gpupageverticalLayout_2_2;
    QHBoxLayout *gputemperaturehorizontalLayout;
    QLabel *gputemperature;
    QLineEdit *gputemperaturelineEdit;
    QSpacerItem *gputemperaturehorizontalSpacer;
    QHBoxLayout *gpufanshorizontalLayout;
    QLabel *gpufans;
    QLineEdit *gpufanslineEdit;
    QSpacerItem *gpufanshorizontalSpacer;
    QVBoxLayout *gpupageverticalLayout_2_1;
    QHBoxLayout *gpumemoryclockhorizontalLayout;
    QLabel *gpumemoryclock;
    QLineEdit *gpumemoryclocklineEdit;
    QSpacerItem *gpumemoryclockhorizontalSpacer;
    QHBoxLayout *gpuphysicalmemoryhorizontalLayout;
    QLabel *gpuphysicalmemory;
    QLineEdit *gpuphysicalmemorylineEdit;
    QSpacerItem *gpuphysicalmemoryhorizontalSpacer;
    QHBoxLayout *gpuvirtualmemoryhorizontalLayout;
    QLabel *gpusharedsystemmemory;
    QLineEdit *gpusharedsystemmemorylineEdit;
    QSpacerItem *gpuvirtualmemoryhorizontalSpacer;
    QSpacerItem *verticalSpacer_3;
    QWidget *edidpage;
    QVBoxLayout *verticalLayout_13;
    QSplitter *edidsplitteedidr;
    edidtreewidget *edidtreeWidget;
    edidtextbrowser *edidtextBrowser;
    QWidget *usbpage;
    QHBoxLayout *horizontalLayout_2;
    QSplitter *usbsplitter;
    usbtreewidget *usbtreeWidget;
    usbtestbrowser *usbtextBrowser;
    QWidget *diskpage;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *diskselecthorizontalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *disktemperaturelabel;
    QLabel *disknamelabel;
    QGroupBox *diskgroupBox;
    QHBoxLayout *horizontalLayout_31;
    QHBoxLayout *horizontalLayout_30;
    QVBoxLayout *verticalLayout_16;
    QHBoxLayout *diskfirenamehorizontalLayout;
    QLabel *diskfirename;
    QLineEdit *diskfirenamelineEdit;
    QSpacerItem *diskfirenamehorizontalSpacer;
    QHBoxLayout *diskserialnumhorizontalLayout;
    QLabel *diskserialnum;
    QLineEdit *diskserialnumlineEdit;
    QSpacerItem *diskserialnumhorizontalSpacer;
    QHBoxLayout *diskinterfacehorizontalLayout;
    QLabel *diskinterface;
    QLineEdit *diskinterfacelineEdit;
    QSpacerItem *diskinterfacehorizontalSpacer;
    QHBoxLayout *disktransfermodehorizontalLayout;
    QLabel *disktransfermode;
    QLineEdit *disktransfermodelineEdit;
    QSpacerItem *disktransfermodehorizontalSpacer;
    QVBoxLayout *verticalLayout_17;
    QHBoxLayout *diskpoweroncounthorizontalLayout;
    QLabel *diskpoweroncount;
    QLineEdit *diskpoweroncountlineEdit;
    QSpacerItem *diskpoweroncounthorizontalSpacer;
    QHBoxLayout *diskpoweronhourshorizontalLayout;
    QLabel *diskpoweronhours;
    QLineEdit *diskpoweronhourslineEdit;
    QSpacerItem *diskpoweronhourshorizontalSpacer;
    QHBoxLayout *diskStandardhorizontalLayout;
    QLabel *diskStandard;
    QLineEdit *diskstandardlineEdit;
    QSpacerItem *diskStandardhorizontalSpacer;
    QHBoxLayout *disksizehorizontalLayout;
    QLabel *disksize;
    QLineEdit *disksizelineEdit;
    QSpacerItem *disksizehorizontalSpacer;
    QTableWidget *disktableWidget;
    QWidget *smbiospage;
    QVBoxLayout *verticalLayout_14;
    QSplitter *smbiossplitter;
    smbiostreewidget *smbiostreeWidget;
    smbiostextbrowser *smbiostextBrowser;
    QWidget *otherspage;
    QWidget *advencedhardwarepage;
    QHBoxLayout *advencedhardwarehorizontalLayout;
    QWidget *advencedhardwareselectwidget;
    QVBoxLayout *verticalLayout_5;
    SV_HARDWARE_PUSHBUTTON *rwIobutton;
    SV_HARDWARE_PUSHBUTTON *rwmemorybutton;
    SV_HARDWARE_PUSHBUTTON *rwmsrbutton;
    SV_HARDWARE_PUSHBUTTON *PCIbutton;
    QSpacerItem *advencedhardwareselectverticalSpacer;
    QStackedWidget *advencedhardwarestackedWidget;
    QWidget *rwIopage;
    QVBoxLayout *verticalLayout_15;
    RWBaseButton *iobasepushButton;
    RW_Qtablewidget *rwiotableWidget;
    QWidget *rwmemorypage;
    QVBoxLayout *verticalLayout_19;
    RWBaseButton *memorybasepushButton;
    RW_Qtablewidget *rwmemorytableWidget;
    QWidget *rwmsrpage;
    QVBoxLayout *verticalLayout_21;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_4;
    QLabel *msraddrlabel;
    QLineEdit *MSRADDRlineEdit;
    QPushButton *msreadpushButton;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_2;
    QLabel *msrdatalabel;
    QLineEdit *MSRDATAlineEdit;
    QPushButton *mstwritepushButton;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_11;
    QSpacerItem *horizontalSpacer_6;
    QLabel *msrcorenumlabel;
    QLineEdit *MSRcorenumlineEdit;
    QPushButton *msrcorenumpushButton;
    QSpacerItem *horizontalSpacer_7;
    QTableWidget *rwmsrtableWidget;
    QWidget *PCIpage;
    QVBoxLayout *verticalLayout;
    pcicombobox *pcicomboBox;
    QHBoxLayout *PCIhorizontalLayout;
    pcitablewidget *pcitableWidget;
    QTextBrowser *pcitextBrowser;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::WindowModal);
        MainWindow->resize(900, 620);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(849, 465));
        MainWindow->setMaximumSize(QSize(16777215, 16777215));
        MainWindow->setMouseTracking(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/Main/activity_monitor.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QStringLiteral(""));
        MainWindow->setToolButtonStyle(Qt::ToolButtonTextOnly);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMouseTracking(true);
        centralWidget->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_18 = new QVBoxLayout(centralWidget);
        verticalLayout_18->setSpacing(0);
        verticalLayout_18->setContentsMargins(11, 11, 11, 11);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        verticalLayout_18->setContentsMargins(0, 0, 0, 0);
        titlewidget = new QWidget(centralWidget);
        titlewidget->setObjectName(QStringLiteral("titlewidget"));
        titlewidget->setMinimumSize(QSize(0, 0));
        titlewidget->setMaximumSize(QSize(16777215, 16777215));
        titlewidget->setMouseTracking(true);
        titlewidget->setStyleSheet(QStringLiteral("QWidget#titlewidget {border-image: url(:/resources/Main/background.jpg);}"));
        horizontalLayout_3 = new QHBoxLayout(titlewidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(5, 0, 5, 0);
        comprehensivetoolButton = new QToolButton(titlewidget);
        comprehensivetoolButton->setObjectName(QStringLiteral("comprehensivetoolButton"));
        sizePolicy.setHeightForWidth(comprehensivetoolButton->sizePolicy().hasHeightForWidth());
        comprehensivetoolButton->setSizePolicy(sizePolicy);
        comprehensivetoolButton->setMinimumSize(QSize(50, 50));
        comprehensivetoolButton->setMaximumSize(QSize(16777215, 16777215));
        comprehensivetoolButton->setStyleSheet(QStringLiteral("border-image: url(:/resources/Main/Synthesize.png);"));

        horizontalLayout_3->addWidget(comprehensivetoolButton);

        hardwaretoolButton = new QToolButton(titlewidget);
        hardwaretoolButton->setObjectName(QStringLiteral("hardwaretoolButton"));
        sizePolicy.setHeightForWidth(hardwaretoolButton->sizePolicy().hasHeightForWidth());
        hardwaretoolButton->setSizePolicy(sizePolicy);
        hardwaretoolButton->setMinimumSize(QSize(50, 50));
        hardwaretoolButton->setMaximumSize(QSize(16777215, 16777215));
        hardwaretoolButton->setStyleSheet(QStringLiteral("border-image: url(:/resources/Main/HardWareInfo.png);"));

        horizontalLayout_3->addWidget(hardwaretoolButton);

        advencedhardwaretoolButton = new QToolButton(titlewidget);
        advencedhardwaretoolButton->setObjectName(QStringLiteral("advencedhardwaretoolButton"));
        sizePolicy.setHeightForWidth(advencedhardwaretoolButton->sizePolicy().hasHeightForWidth());
        advencedhardwaretoolButton->setSizePolicy(sizePolicy);
        advencedhardwaretoolButton->setMinimumSize(QSize(50, 50));
        advencedhardwaretoolButton->setMaximumSize(QSize(16777215, 16777215));
        advencedhardwaretoolButton->setLayoutDirection(Qt::LeftToRight);
        advencedhardwaretoolButton->setStyleSheet(QStringLiteral("border-image: url(:/resources/Main/Modification.png);"));

        horizontalLayout_3->addWidget(advencedhardwaretoolButton);

        horizontalSpacer = new QSpacerItem(552, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(0);
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        maincontrolhorizontalLayout = new QHBoxLayout();
        maincontrolhorizontalLayout->setSpacing(6);
        maincontrolhorizontalLayout->setObjectName(QStringLiteral("maincontrolhorizontalLayout"));
        settingtoolButton = new settingbutton(titlewidget);
        settingtoolButton->setObjectName(QStringLiteral("settingtoolButton"));
        sizePolicy.setHeightForWidth(settingtoolButton->sizePolicy().hasHeightForWidth());
        settingtoolButton->setSizePolicy(sizePolicy);
        settingtoolButton->setMinimumSize(QSize(30, 30));
        settingtoolButton->setMaximumSize(QSize(16777215, 16777215));

        maincontrolhorizontalLayout->addWidget(settingtoolButton);

        minpushButton = new mainctrlbutton(titlewidget);
        minpushButton->setObjectName(QStringLiteral("minpushButton"));
        sizePolicy.setHeightForWidth(minpushButton->sizePolicy().hasHeightForWidth());
        minpushButton->setSizePolicy(sizePolicy);
        minpushButton->setMinimumSize(QSize(30, 30));
        minpushButton->setMaximumSize(QSize(16777215, 16777215));

        maincontrolhorizontalLayout->addWidget(minpushButton);

        closepushButton = new closebutton(titlewidget);
        closepushButton->setObjectName(QStringLiteral("closepushButton"));
        sizePolicy.setHeightForWidth(closepushButton->sizePolicy().hasHeightForWidth());
        closepushButton->setSizePolicy(sizePolicy);
        closepushButton->setMinimumSize(QSize(30, 30));

        maincontrolhorizontalLayout->addWidget(closepushButton);


        verticalLayout_20->addLayout(maincontrolhorizontalLayout);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_20->addItem(verticalSpacer);


        horizontalLayout_3->addLayout(verticalLayout_20);


        verticalLayout_18->addWidget(titlewidget);

        MainstackedWidget = new QStackedWidget(centralWidget);
        MainstackedWidget->setObjectName(QStringLiteral("MainstackedWidget"));
        MainstackedWidget->setMouseTracking(true);
        comprehensivepage = new QWidget();
        comprehensivepage->setObjectName(QStringLiteral("comprehensivepage"));
        comprehensivepage->setMouseTracking(true);
        horizontalLayout_33 = new QHBoxLayout(comprehensivepage);
        horizontalLayout_33->setSpacing(6);
        horizontalLayout_33->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_33->setObjectName(QStringLiteral("horizontalLayout_33"));
        horizontalLayout_33->setContentsMargins(0, 7, 0, -1);
        mainleftframe = new QFrame(comprehensivepage);
        mainleftframe->setObjectName(QStringLiteral("mainleftframe"));
        mainleftframe->setMaximumSize(QSize(16777215, 16777215));
        mainleftframe->setFrameShape(QFrame::StyledPanel);
        mainleftframe->setFrameShadow(QFrame::Raised);
        verticalLayout_22 = new QVBoxLayout(mainleftframe);
        verticalLayout_22->setSpacing(6);
        verticalLayout_22->setContentsMargins(11, 11, 11, 11);
        verticalLayout_22->setObjectName(QStringLiteral("verticalLayout_22"));
        mainleftframeheadverticalLayout = new QVBoxLayout();
        mainleftframeheadverticalLayout->setSpacing(6);
        mainleftframeheadverticalLayout->setObjectName(QStringLiteral("mainleftframeheadverticalLayout"));
        maincomputertypehorizontalLayout = new QHBoxLayout();
        maincomputertypehorizontalLayout->setSpacing(6);
        maincomputertypehorizontalLayout->setObjectName(QStringLiteral("maincomputertypehorizontalLayout"));
        maincomputertypelabel = new QLabel(mainleftframe);
        maincomputertypelabel->setObjectName(QStringLiteral("maincomputertypelabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(maincomputertypelabel->sizePolicy().hasHeightForWidth());
        maincomputertypelabel->setSizePolicy(sizePolicy1);

        maincomputertypehorizontalLayout->addWidget(maincomputertypelabel);

        maincomputertypedatalabel = new QLabel(mainleftframe);
        maincomputertypedatalabel->setObjectName(QStringLiteral("maincomputertypedatalabel"));
        sizePolicy1.setHeightForWidth(maincomputertypedatalabel->sizePolicy().hasHeightForWidth());
        maincomputertypedatalabel->setSizePolicy(sizePolicy1);

        maincomputertypehorizontalLayout->addWidget(maincomputertypedatalabel);


        mainleftframeheadverticalLayout->addLayout(maincomputertypehorizontalLayout);

        mainsystemhorizontalLayout = new QHBoxLayout();
        mainsystemhorizontalLayout->setSpacing(6);
        mainsystemhorizontalLayout->setObjectName(QStringLiteral("mainsystemhorizontalLayout"));
        mainsystemlabel = new QLabel(mainleftframe);
        mainsystemlabel->setObjectName(QStringLiteral("mainsystemlabel"));
        sizePolicy1.setHeightForWidth(mainsystemlabel->sizePolicy().hasHeightForWidth());
        mainsystemlabel->setSizePolicy(sizePolicy1);

        mainsystemhorizontalLayout->addWidget(mainsystemlabel);

        mainsystemdatalabel = new QLabel(mainleftframe);
        mainsystemdatalabel->setObjectName(QStringLiteral("mainsystemdatalabel"));
        sizePolicy1.setHeightForWidth(mainsystemdatalabel->sizePolicy().hasHeightForWidth());
        mainsystemdatalabel->setSizePolicy(sizePolicy1);

        mainsystemhorizontalLayout->addWidget(mainsystemdatalabel);


        mainleftframeheadverticalLayout->addLayout(mainsystemhorizontalLayout);


        verticalLayout_22->addLayout(mainleftframeheadverticalLayout);

        mainleftframefootverticalLayout = new QVBoxLayout();
        mainleftframefootverticalLayout->setSpacing(6);
        mainleftframefootverticalLayout->setObjectName(QStringLiteral("mainleftframefootverticalLayout"));
        maincpuhorizontalLayout = new QHBoxLayout();
        maincpuhorizontalLayout->setSpacing(6);
        maincpuhorizontalLayout->setObjectName(QStringLiteral("maincpuhorizontalLayout"));
        maincpupushButton = new QPushButton(mainleftframe);
        maincpupushButton->setObjectName(QStringLiteral("maincpupushButton"));
        sizePolicy.setHeightForWidth(maincpupushButton->sizePolicy().hasHeightForWidth());
        maincpupushButton->setSizePolicy(sizePolicy);
        maincpupushButton->setMaximumSize(QSize(50, 16777215));

        maincpuhorizontalLayout->addWidget(maincpupushButton);

        maincpulabel = new QLabel(mainleftframe);
        maincpulabel->setObjectName(QStringLiteral("maincpulabel"));

        maincpuhorizontalLayout->addWidget(maincpulabel);


        mainleftframefootverticalLayout->addLayout(maincpuhorizontalLayout);

        mainboardhorizontalLayout = new QHBoxLayout();
        mainboardhorizontalLayout->setSpacing(6);
        mainboardhorizontalLayout->setObjectName(QStringLiteral("mainboardhorizontalLayout"));
        mainboardpushButton = new QPushButton(mainleftframe);
        mainboardpushButton->setObjectName(QStringLiteral("mainboardpushButton"));
        sizePolicy.setHeightForWidth(mainboardpushButton->sizePolicy().hasHeightForWidth());
        mainboardpushButton->setSizePolicy(sizePolicy);
        mainboardpushButton->setMaximumSize(QSize(50, 16777215));

        mainboardhorizontalLayout->addWidget(mainboardpushButton);

        mainboardlabel = new QLabel(mainleftframe);
        mainboardlabel->setObjectName(QStringLiteral("mainboardlabel"));

        mainboardhorizontalLayout->addWidget(mainboardlabel);


        mainleftframefootverticalLayout->addLayout(mainboardhorizontalLayout);

        mainmemoryhorizontalLayout = new QHBoxLayout();
        mainmemoryhorizontalLayout->setSpacing(6);
        mainmemoryhorizontalLayout->setObjectName(QStringLiteral("mainmemoryhorizontalLayout"));
        mainmemorypushButton = new QPushButton(mainleftframe);
        mainmemorypushButton->setObjectName(QStringLiteral("mainmemorypushButton"));
        sizePolicy.setHeightForWidth(mainmemorypushButton->sizePolicy().hasHeightForWidth());
        mainmemorypushButton->setSizePolicy(sizePolicy);
        mainmemorypushButton->setMaximumSize(QSize(50, 16777215));

        mainmemoryhorizontalLayout->addWidget(mainmemorypushButton);

        mainmemorylabel = new QLabel(mainleftframe);
        mainmemorylabel->setObjectName(QStringLiteral("mainmemorylabel"));

        mainmemoryhorizontalLayout->addWidget(mainmemorylabel);


        mainleftframefootverticalLayout->addLayout(mainmemoryhorizontalLayout);

        maindiskhorizontalLayout = new QHBoxLayout();
        maindiskhorizontalLayout->setSpacing(6);
        maindiskhorizontalLayout->setObjectName(QStringLiteral("maindiskhorizontalLayout"));
        maindiskpushButton = new QPushButton(mainleftframe);
        maindiskpushButton->setObjectName(QStringLiteral("maindiskpushButton"));
        sizePolicy.setHeightForWidth(maindiskpushButton->sizePolicy().hasHeightForWidth());
        maindiskpushButton->setSizePolicy(sizePolicy);
        maindiskpushButton->setMaximumSize(QSize(50, 16777215));

        maindiskhorizontalLayout->addWidget(maindiskpushButton);

        maindisklabel = new QLabel(mainleftframe);
        maindisklabel->setObjectName(QStringLiteral("maindisklabel"));

        maindiskhorizontalLayout->addWidget(maindisklabel);


        mainleftframefootverticalLayout->addLayout(maindiskhorizontalLayout);

        maindiskhorizontalLayout_2 = new QHBoxLayout();
        maindiskhorizontalLayout_2->setSpacing(6);
        maindiskhorizontalLayout_2->setObjectName(QStringLiteral("maindiskhorizontalLayout_2"));
        maingpupushButton = new QPushButton(mainleftframe);
        maingpupushButton->setObjectName(QStringLiteral("maingpupushButton"));
        sizePolicy.setHeightForWidth(maingpupushButton->sizePolicy().hasHeightForWidth());
        maingpupushButton->setSizePolicy(sizePolicy);
        maingpupushButton->setMaximumSize(QSize(50, 16777215));

        maindiskhorizontalLayout_2->addWidget(maingpupushButton);

        maingpulabel = new QLabel(mainleftframe);
        maingpulabel->setObjectName(QStringLiteral("maingpulabel"));

        maindiskhorizontalLayout_2->addWidget(maingpulabel);


        mainleftframefootverticalLayout->addLayout(maindiskhorizontalLayout_2);

        mainmonitorhorizontalLayout = new QHBoxLayout();
        mainmonitorhorizontalLayout->setSpacing(6);
        mainmonitorhorizontalLayout->setObjectName(QStringLiteral("mainmonitorhorizontalLayout"));
        mainmonitorpushButton = new QPushButton(mainleftframe);
        mainmonitorpushButton->setObjectName(QStringLiteral("mainmonitorpushButton"));
        sizePolicy.setHeightForWidth(mainmonitorpushButton->sizePolicy().hasHeightForWidth());
        mainmonitorpushButton->setSizePolicy(sizePolicy);
        mainmonitorpushButton->setMaximumSize(QSize(50, 16777215));

        mainmonitorhorizontalLayout->addWidget(mainmonitorpushButton);

        mainmonitorlabel = new QLabel(mainleftframe);
        mainmonitorlabel->setObjectName(QStringLiteral("mainmonitorlabel"));

        mainmonitorhorizontalLayout->addWidget(mainmonitorlabel);


        mainleftframefootverticalLayout->addLayout(mainmonitorhorizontalLayout);

        mainaudiohorizontalLayout = new QHBoxLayout();
        mainaudiohorizontalLayout->setSpacing(6);
        mainaudiohorizontalLayout->setObjectName(QStringLiteral("mainaudiohorizontalLayout"));
        mainaudiopushButton = new QPushButton(mainleftframe);
        mainaudiopushButton->setObjectName(QStringLiteral("mainaudiopushButton"));
        sizePolicy.setHeightForWidth(mainaudiopushButton->sizePolicy().hasHeightForWidth());
        mainaudiopushButton->setSizePolicy(sizePolicy);
        mainaudiopushButton->setMaximumSize(QSize(50, 16777215));

        mainaudiohorizontalLayout->addWidget(mainaudiopushButton);

        mainaudiolabel = new QLabel(mainleftframe);
        mainaudiolabel->setObjectName(QStringLiteral("mainaudiolabel"));

        mainaudiohorizontalLayout->addWidget(mainaudiolabel);


        mainleftframefootverticalLayout->addLayout(mainaudiohorizontalLayout);

        mainnethorizontalLayout = new QHBoxLayout();
        mainnethorizontalLayout->setSpacing(6);
        mainnethorizontalLayout->setObjectName(QStringLiteral("mainnethorizontalLayout"));
        mainnetpushButton = new QPushButton(mainleftframe);
        mainnetpushButton->setObjectName(QStringLiteral("mainnetpushButton"));
        sizePolicy.setHeightForWidth(mainnetpushButton->sizePolicy().hasHeightForWidth());
        mainnetpushButton->setSizePolicy(sizePolicy);
        mainnetpushButton->setMaximumSize(QSize(50, 16777215));
        mainnetpushButton->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));

        mainnethorizontalLayout->addWidget(mainnetpushButton);

        mainnetlabel = new QLabel(mainleftframe);
        mainnetlabel->setObjectName(QStringLiteral("mainnetlabel"));

        mainnethorizontalLayout->addWidget(mainnetlabel);


        mainleftframefootverticalLayout->addLayout(mainnethorizontalLayout);


        verticalLayout_22->addLayout(mainleftframefootverticalLayout);


        horizontalLayout_33->addWidget(mainleftframe);

        mainrightstackedWidget = new QStackedWidget(comprehensivepage);
        mainrightstackedWidget->setObjectName(QStringLiteral("mainrightstackedWidget"));
        mainrightstackedWidget->setMinimumSize(QSize(500, 0));
        mainrightstackedWidget->setFrameShape(QFrame::StyledPanel);
        mainrightstackedWidget->setFrameShadow(QFrame::Raised);
        mainrightstackedWidgetcpuPage = new QWidget();
        mainrightstackedWidgetcpuPage->setObjectName(QStringLiteral("mainrightstackedWidgetcpuPage"));
        mainrightstackedWidgetcpuPageverticalLayout = new QVBoxLayout(mainrightstackedWidgetcpuPage);
        mainrightstackedWidgetcpuPageverticalLayout->setSpacing(6);
        mainrightstackedWidgetcpuPageverticalLayout->setContentsMargins(11, 11, 11, 11);
        mainrightstackedWidgetcpuPageverticalLayout->setObjectName(QStringLiteral("mainrightstackedWidgetcpuPageverticalLayout"));
        cpuusageverticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        mainrightstackedWidgetcpuPageverticalLayout->addItem(cpuusageverticalSpacer);

        cpuusagewidget = new cpuusageQcharts(mainrightstackedWidgetcpuPage);
        cpuusagewidget->setObjectName(QStringLiteral("cpuusagewidget"));

        mainrightstackedWidgetcpuPageverticalLayout->addWidget(cpuusagewidget);

        mainrightstackedWidget->addWidget(mainrightstackedWidgetcpuPage);

        horizontalLayout_33->addWidget(mainrightstackedWidget);

        MainstackedWidget->addWidget(comprehensivepage);
        hardwarepage = new QWidget();
        hardwarepage->setObjectName(QStringLiteral("hardwarepage"));
        hardwarepage->setMouseTracking(true);
        horizontalLayout_10 = new QHBoxLayout(hardwarepage);
        horizontalLayout_10->setSpacing(5);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        functionwidget = new QWidget(hardwarepage);
        functionwidget->setObjectName(QStringLiteral("functionwidget"));
        functionwidget->setMaximumSize(QSize(16777215, 16777215));
        functionwidget->setMouseTracking(true);
        hardwarebuttonverticalLayout = new QVBoxLayout(functionwidget);
        hardwarebuttonverticalLayout->setSpacing(0);
        hardwarebuttonverticalLayout->setContentsMargins(11, 11, 11, 11);
        hardwarebuttonverticalLayout->setObjectName(QStringLiteral("hardwarebuttonverticalLayout"));
        hardwarebuttonverticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        hardwarebuttonverticalLayout->setContentsMargins(0, 0, 0, 0);
        CPUbutton = new SV_HARDWARE_PUSHBUTTON(functionwidget);
        CPUbutton->setObjectName(QStringLiteral("CPUbutton"));
        sizePolicy.setHeightForWidth(CPUbutton->sizePolicy().hasHeightForWidth());
        CPUbutton->setSizePolicy(sizePolicy);

        hardwarebuttonverticalLayout->addWidget(CPUbutton);

        memorybutton = new SV_HARDWARE_PUSHBUTTON(functionwidget);
        memorybutton->setObjectName(QStringLiteral("memorybutton"));
        sizePolicy.setHeightForWidth(memorybutton->sizePolicy().hasHeightForWidth());
        memorybutton->setSizePolicy(sizePolicy);

        hardwarebuttonverticalLayout->addWidget(memorybutton);

        GPUbutton = new SV_HARDWARE_PUSHBUTTON(functionwidget);
        GPUbutton->setObjectName(QStringLiteral("GPUbutton"));
        sizePolicy.setHeightForWidth(GPUbutton->sizePolicy().hasHeightForWidth());
        GPUbutton->setSizePolicy(sizePolicy);

        hardwarebuttonverticalLayout->addWidget(GPUbutton);

        EDIDbutton = new SV_HARDWARE_PUSHBUTTON(functionwidget);
        EDIDbutton->setObjectName(QStringLiteral("EDIDbutton"));
        sizePolicy.setHeightForWidth(EDIDbutton->sizePolicy().hasHeightForWidth());
        EDIDbutton->setSizePolicy(sizePolicy);

        hardwarebuttonverticalLayout->addWidget(EDIDbutton);

        USBbutton = new SV_HARDWARE_PUSHBUTTON(functionwidget);
        USBbutton->setObjectName(QStringLiteral("USBbutton"));
        sizePolicy.setHeightForWidth(USBbutton->sizePolicy().hasHeightForWidth());
        USBbutton->setSizePolicy(sizePolicy);

        hardwarebuttonverticalLayout->addWidget(USBbutton);

        Diskbutton = new SV_HARDWARE_PUSHBUTTON(functionwidget);
        Diskbutton->setObjectName(QStringLiteral("Diskbutton"));
        sizePolicy.setHeightForWidth(Diskbutton->sizePolicy().hasHeightForWidth());
        Diskbutton->setSizePolicy(sizePolicy);

        hardwarebuttonverticalLayout->addWidget(Diskbutton);

        SMBIOSbutton = new SV_HARDWARE_PUSHBUTTON(functionwidget);
        SMBIOSbutton->setObjectName(QStringLiteral("SMBIOSbutton"));
        sizePolicy.setHeightForWidth(SMBIOSbutton->sizePolicy().hasHeightForWidth());
        SMBIOSbutton->setSizePolicy(sizePolicy);

        hardwarebuttonverticalLayout->addWidget(SMBIOSbutton);

        othersbutton = new SV_HARDWARE_PUSHBUTTON(functionwidget);
        othersbutton->setObjectName(QStringLiteral("othersbutton"));
        sizePolicy.setHeightForWidth(othersbutton->sizePolicy().hasHeightForWidth());
        othersbutton->setSizePolicy(sizePolicy);

        hardwarebuttonverticalLayout->addWidget(othersbutton);


        horizontalLayout_10->addWidget(functionwidget);

        hardwarestackedWidget = new QStackedWidget(hardwarepage);
        hardwarestackedWidget->setObjectName(QStringLiteral("hardwarestackedWidget"));
        hardwarestackedWidget->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        CPUPage = new QWidget();
        CPUPage->setObjectName(QStringLiteral("CPUPage"));
        CPUPage->setStyleSheet(QStringLiteral(""));
        verticalLayout_2 = new QVBoxLayout(CPUPage);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        processorgroupBox = new QGroupBox(CPUPage);
        processorgroupBox->setObjectName(QStringLiteral("processorgroupBox"));
        processorgroupBox->setMinimumSize(QSize(501, 161));
        processorgroupBox->setMaximumSize(QSize(1051, 271));
        gridLayout = new QGridLayout(processorgroupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        packagehorizontalLayout = new QHBoxLayout();
        packagehorizontalLayout->setSpacing(6);
        packagehorizontalLayout->setObjectName(QStringLiteral("packagehorizontalLayout"));
        Package = new QLabel(processorgroupBox);
        Package->setObjectName(QStringLiteral("Package"));

        packagehorizontalLayout->addWidget(Package);

        PackagelefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        packagehorizontalLayout->addItem(PackagelefthorizontalSpacer);

        PackagelineEdit = new QLineEdit(processorgroupBox);
        PackagelineEdit->setObjectName(QStringLiteral("PackagelineEdit"));
        PackagelineEdit->setMinimumSize(QSize(150, 0));
        PackagelineEdit->setMaximumSize(QSize(150, 16777215));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(8);
        PackagelineEdit->setFont(font);
        PackagelineEdit->setFocusPolicy(Qt::NoFocus);
        PackagelineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        PackagelineEdit->setFrame(true);
        PackagelineEdit->setAlignment(Qt::AlignCenter);
        PackagelineEdit->setDragEnabled(false);
        PackagelineEdit->setReadOnly(true);
        PackagelineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        packagehorizontalLayout->addWidget(PackagelineEdit);

        PackagerighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        packagehorizontalLayout->addItem(PackagerighthorizontalSpacer);


        gridLayout->addLayout(packagehorizontalLayout, 3, 0, 1, 1);

        manufacturehorizontalLayout = new QHBoxLayout();
        manufacturehorizontalLayout->setSpacing(6);
        manufacturehorizontalLayout->setObjectName(QStringLiteral("manufacturehorizontalLayout"));
        Manufacture = new QLabel(processorgroupBox);
        Manufacture->setObjectName(QStringLiteral("Manufacture"));

        manufacturehorizontalLayout->addWidget(Manufacture);

        manufacturlefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        manufacturehorizontalLayout->addItem(manufacturlefthorizontalSpacer);

        manufacturelineEdit = new QLineEdit(processorgroupBox);
        manufacturelineEdit->setObjectName(QStringLiteral("manufacturelineEdit"));
        manufacturelineEdit->setMinimumSize(QSize(150, 0));
        manufacturelineEdit->setMaximumSize(QSize(150, 16777215));
        manufacturelineEdit->setFont(font);
        manufacturelineEdit->setFocusPolicy(Qt::NoFocus);
        manufacturelineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        manufacturelineEdit->setFrame(true);
        manufacturelineEdit->setAlignment(Qt::AlignCenter);
        manufacturelineEdit->setDragEnabled(false);
        manufacturelineEdit->setReadOnly(true);
        manufacturelineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        manufacturehorizontalLayout->addWidget(manufacturelineEdit);

        manufacturerighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        manufacturehorizontalLayout->addItem(manufacturerighthorizontalSpacer);


        gridLayout->addLayout(manufacturehorizontalLayout, 1, 0, 1, 1);

        processoridhorizontalLayout = new QHBoxLayout();
        processoridhorizontalLayout->setSpacing(6);
        processoridhorizontalLayout->setObjectName(QStringLiteral("processoridhorizontalLayout"));
        processorid = new QLabel(processorgroupBox);
        processorid->setObjectName(QStringLiteral("processorid"));

        processoridhorizontalLayout->addWidget(processorid);

        processoridlefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        processoridhorizontalLayout->addItem(processoridlefthorizontalSpacer);

        processoridlineEdit = new QLineEdit(processorgroupBox);
        processoridlineEdit->setObjectName(QStringLiteral("processoridlineEdit"));
        processoridlineEdit->setMinimumSize(QSize(150, 0));
        processoridlineEdit->setMaximumSize(QSize(300, 16777215));
        processoridlineEdit->setFont(font);
        processoridlineEdit->setFocusPolicy(Qt::NoFocus);
        processoridlineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        processoridlineEdit->setFrame(true);
        processoridlineEdit->setAlignment(Qt::AlignCenter);
        processoridlineEdit->setDragEnabled(false);
        processoridlineEdit->setReadOnly(true);
        processoridlineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        processoridhorizontalLayout->addWidget(processoridlineEdit);

        processoridhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        processoridhorizontalLayout->addItem(processoridhorizontalSpacer);


        gridLayout->addLayout(processoridhorizontalLayout, 1, 1, 1, 1);

        InstructionshorizontalLayout = new QHBoxLayout();
        InstructionshorizontalLayout->setSpacing(6);
        InstructionshorizontalLayout->setObjectName(QStringLiteral("InstructionshorizontalLayout"));
        Instructions = new QLabel(processorgroupBox);
        Instructions->setObjectName(QStringLiteral("Instructions"));

        InstructionshorizontalLayout->addWidget(Instructions);

        InstructionshorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        InstructionshorizontalLayout->addItem(InstructionshorizontalSpacer);

        InstructionslineEdit = new QLineEdit(processorgroupBox);
        InstructionslineEdit->setObjectName(QStringLiteral("InstructionslineEdit"));
        InstructionslineEdit->setMinimumSize(QSize(150, 0));
        InstructionslineEdit->setFont(font);
        InstructionslineEdit->setFocusPolicy(Qt::NoFocus);
        InstructionslineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        InstructionslineEdit->setFrame(true);
        InstructionslineEdit->setAlignment(Qt::AlignCenter);
        InstructionslineEdit->setDragEnabled(false);
        InstructionslineEdit->setReadOnly(true);
        InstructionslineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        InstructionshorizontalLayout->addWidget(InstructionslineEdit);


        gridLayout->addLayout(InstructionshorizontalLayout, 5, 0, 1, 3);

        fmsverticalLayout = new QVBoxLayout();
        fmsverticalLayout->setSpacing(6);
        fmsverticalLayout->setObjectName(QStringLiteral("fmsverticalLayout"));
        fmshorizontalLayout = new QHBoxLayout();
        fmshorizontalLayout->setSpacing(6);
        fmshorizontalLayout->setObjectName(QStringLiteral("fmshorizontalLayout"));
        Family = new QLabel(processorgroupBox);
        Family->setObjectName(QStringLiteral("Family"));

        fmshorizontalLayout->addWidget(Family);

        familylefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        fmshorizontalLayout->addItem(familylefthorizontalSpacer);

        familylineEdit = new QLineEdit(processorgroupBox);
        familylineEdit->setObjectName(QStringLiteral("familylineEdit"));
        familylineEdit->setMinimumSize(QSize(50, 0));
        familylineEdit->setMaximumSize(QSize(150, 16777215));
        familylineEdit->setFont(font);
        familylineEdit->setFocusPolicy(Qt::NoFocus);
        familylineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        familylineEdit->setFrame(true);
        familylineEdit->setAlignment(Qt::AlignCenter);
        familylineEdit->setDragEnabled(false);
        familylineEdit->setReadOnly(true);
        familylineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        fmshorizontalLayout->addWidget(familylineEdit);

        familyrighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        fmshorizontalLayout->addItem(familyrighthorizontalSpacer);

        Model = new QLabel(processorgroupBox);
        Model->setObjectName(QStringLiteral("Model"));

        fmshorizontalLayout->addWidget(Model);

        modellefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        fmshorizontalLayout->addItem(modellefthorizontalSpacer);

        modellineEdit = new QLineEdit(processorgroupBox);
        modellineEdit->setObjectName(QStringLiteral("modellineEdit"));
        modellineEdit->setMinimumSize(QSize(50, 0));
        modellineEdit->setMaximumSize(QSize(150, 16777215));
        modellineEdit->setFont(font);
        modellineEdit->setFocusPolicy(Qt::NoFocus);
        modellineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        modellineEdit->setFrame(true);
        modellineEdit->setAlignment(Qt::AlignCenter);
        modellineEdit->setDragEnabled(false);
        modellineEdit->setReadOnly(true);
        modellineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        fmshorizontalLayout->addWidget(modellineEdit);

        modelrighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        fmshorizontalLayout->addItem(modelrighthorizontalSpacer);

        stepping = new QLabel(processorgroupBox);
        stepping->setObjectName(QStringLiteral("stepping"));

        fmshorizontalLayout->addWidget(stepping);

        steplefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        fmshorizontalLayout->addItem(steplefthorizontalSpacer);

        steppinglineEdit = new QLineEdit(processorgroupBox);
        steppinglineEdit->setObjectName(QStringLiteral("steppinglineEdit"));
        steppinglineEdit->setMinimumSize(QSize(50, 0));
        steppinglineEdit->setMaximumSize(QSize(150, 16777215));
        steppinglineEdit->setFont(font);
        steppinglineEdit->setFocusPolicy(Qt::NoFocus);
        steppinglineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        steppinglineEdit->setFrame(true);
        steppinglineEdit->setAlignment(Qt::AlignCenter);
        steppinglineEdit->setDragEnabled(false);
        steppinglineEdit->setReadOnly(true);
        steppinglineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        fmshorizontalLayout->addWidget(steppinglineEdit);

        steprighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        fmshorizontalLayout->addItem(steprighthorizontalSpacer);


        fmsverticalLayout->addLayout(fmshorizontalLayout);


        gridLayout->addLayout(fmsverticalLayout, 3, 1, 1, 2);

        technologyhorizontalLayout = new QHBoxLayout();
        technologyhorizontalLayout->setSpacing(6);
        technologyhorizontalLayout->setObjectName(QStringLiteral("technologyhorizontalLayout"));
        Techonology = new QLabel(processorgroupBox);
        Techonology->setObjectName(QStringLiteral("Techonology"));

        technologyhorizontalLayout->addWidget(Techonology);

        technologylefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        technologyhorizontalLayout->addItem(technologylefthorizontalSpacer);

        technologylineEdit = new QLineEdit(processorgroupBox);
        technologylineEdit->setObjectName(QStringLiteral("technologylineEdit"));
        technologylineEdit->setMinimumSize(QSize(150, 0));
        technologylineEdit->setMaximumSize(QSize(150, 16777215));
        technologylineEdit->setFont(font);
        technologylineEdit->setFocusPolicy(Qt::NoFocus);
        technologylineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        technologylineEdit->setFrame(true);
        technologylineEdit->setAlignment(Qt::AlignCenter);
        technologylineEdit->setDragEnabled(false);
        technologylineEdit->setReadOnly(true);
        technologylineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        technologyhorizontalLayout->addWidget(technologylineEdit);

        technologyrighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        technologyhorizontalLayout->addItem(technologyrighthorizontalSpacer);


        gridLayout->addLayout(technologyhorizontalLayout, 4, 0, 1, 1);

        extfmrhorizontalLayout = new QHBoxLayout();
        extfmrhorizontalLayout->setSpacing(6);
        extfmrhorizontalLayout->setObjectName(QStringLiteral("extfmrhorizontalLayout"));
        Extfamily = new QLabel(processorgroupBox);
        Extfamily->setObjectName(QStringLiteral("Extfamily"));

        extfmrhorizontalLayout->addWidget(Extfamily);

        extfamilylefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        extfmrhorizontalLayout->addItem(extfamilylefthorizontalSpacer);

        extfamilylineEdit = new QLineEdit(processorgroupBox);
        extfamilylineEdit->setObjectName(QStringLiteral("extfamilylineEdit"));
        extfamilylineEdit->setMinimumSize(QSize(50, 0));
        extfamilylineEdit->setMaximumSize(QSize(150, 16777215));
        extfamilylineEdit->setFont(font);
        extfamilylineEdit->setFocusPolicy(Qt::NoFocus);
        extfamilylineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        extfamilylineEdit->setFrame(true);
        extfamilylineEdit->setAlignment(Qt::AlignCenter);
        extfamilylineEdit->setDragEnabled(false);
        extfamilylineEdit->setReadOnly(true);
        extfamilylineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        extfmrhorizontalLayout->addWidget(extfamilylineEdit);

        extfamilyrighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        extfmrhorizontalLayout->addItem(extfamilyrighthorizontalSpacer);

        extmodel = new QLabel(processorgroupBox);
        extmodel->setObjectName(QStringLiteral("extmodel"));

        extfmrhorizontalLayout->addWidget(extmodel);

        extmodelefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        extfmrhorizontalLayout->addItem(extmodelefthorizontalSpacer);

        extmodellineEdit = new QLineEdit(processorgroupBox);
        extmodellineEdit->setObjectName(QStringLiteral("extmodellineEdit"));
        extmodellineEdit->setMinimumSize(QSize(50, 0));
        extmodellineEdit->setMaximumSize(QSize(150, 16777215));
        extmodellineEdit->setFont(font);
        extmodellineEdit->setFocusPolicy(Qt::NoFocus);
        extmodellineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        extmodellineEdit->setFrame(true);
        extmodellineEdit->setAlignment(Qt::AlignCenter);
        extmodellineEdit->setDragEnabled(false);
        extmodellineEdit->setReadOnly(true);
        extmodellineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        extfmrhorizontalLayout->addWidget(extmodellineEdit);

        extmodelrighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        extfmrhorizontalLayout->addItem(extmodelrighthorizontalSpacer);

        revision = new QLabel(processorgroupBox);
        revision->setObjectName(QStringLiteral("revision"));

        extfmrhorizontalLayout->addWidget(revision);

        revisionlefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        extfmrhorizontalLayout->addItem(revisionlefthorizontalSpacer);

        revisionlineEdit = new QLineEdit(processorgroupBox);
        revisionlineEdit->setObjectName(QStringLiteral("revisionlineEdit"));
        revisionlineEdit->setMinimumSize(QSize(50, 0));
        revisionlineEdit->setMaximumSize(QSize(150, 16777215));
        revisionlineEdit->setFont(font);
        revisionlineEdit->setFocusPolicy(Qt::NoFocus);
        revisionlineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        revisionlineEdit->setFrame(true);
        revisionlineEdit->setAlignment(Qt::AlignCenter);
        revisionlineEdit->setDragEnabled(false);
        revisionlineEdit->setReadOnly(true);
        revisionlineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        extfmrhorizontalLayout->addWidget(revisionlineEdit);

        revisionrifhthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        extfmrhorizontalLayout->addItem(revisionrifhthorizontalSpacer);


        gridLayout->addLayout(extfmrhorizontalLayout, 4, 1, 1, 2);

        cpunamelayout = new QHBoxLayout();
        cpunamelayout->setSpacing(6);
        cpunamelayout->setObjectName(QStringLiteral("cpunamelayout"));
        CPUNAME = new QLabel(processorgroupBox);
        CPUNAME->setObjectName(QStringLiteral("CPUNAME"));

        cpunamelayout->addWidget(CPUNAME);

        cpunamelefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        cpunamelayout->addItem(cpunamelefthorizontalSpacer);

        cpunamelineEdit = new QLineEdit(processorgroupBox);
        cpunamelineEdit->setObjectName(QStringLiteral("cpunamelineEdit"));
        cpunamelineEdit->setMinimumSize(QSize(150, 0));
        cpunamelineEdit->setMaximumSize(QSize(16777215, 16777215));
        cpunamelineEdit->setFont(font);
        cpunamelineEdit->setMouseTracking(false);
        cpunamelineEdit->setFocusPolicy(Qt::NoFocus);
        cpunamelineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        cpunamelineEdit->setAcceptDrops(false);
        cpunamelineEdit->setText(QStringLiteral(""));
        cpunamelineEdit->setFrame(true);
        cpunamelineEdit->setAlignment(Qt::AlignCenter);
        cpunamelineEdit->setDragEnabled(false);
        cpunamelineEdit->setReadOnly(true);
        cpunamelineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        cpunamelayout->addWidget(cpunamelineEdit);

        cpunamerighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        cpunamelayout->addItem(cpunamerighthorizontalSpacer);


        gridLayout->addLayout(cpunamelayout, 0, 0, 1, 2);

        cpuiconlabel = new QLabel(processorgroupBox);
        cpuiconlabel->setObjectName(QStringLiteral("cpuiconlabel"));

        gridLayout->addWidget(cpuiconlabel, 0, 2, 2, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        CPUNAME_7 = new QLabel(processorgroupBox);
        CPUNAME_7->setObjectName(QStringLiteral("CPUNAME_7"));

        horizontalLayout_8->addWidget(CPUNAME_7);

        horizontalSpacer_67 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_67);

        cpunamelineEdit_7 = new QLineEdit(processorgroupBox);
        cpunamelineEdit_7->setObjectName(QStringLiteral("cpunamelineEdit_7"));
        cpunamelineEdit_7->setEnabled(false);
        cpunamelineEdit_7->setMinimumSize(QSize(150, 0));
        cpunamelineEdit_7->setMaximumSize(QSize(300, 16777215));
        cpunamelineEdit_7->setFont(font);
        cpunamelineEdit_7->setFocusPolicy(Qt::NoFocus);
        cpunamelineEdit_7->setContextMenuPolicy(Qt::NoContextMenu);
        cpunamelineEdit_7->setFrame(true);
        cpunamelineEdit_7->setAlignment(Qt::AlignCenter);
        cpunamelineEdit_7->setDragEnabled(false);
        cpunamelineEdit_7->setReadOnly(true);
        cpunamelineEdit_7->setCursorMoveStyle(Qt::LogicalMoveStyle);

        horizontalLayout_8->addWidget(cpunamelineEdit_7);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_8);


        gridLayout->addLayout(horizontalLayout_8, 2, 1, 1, 2);

        codenamehorizontalLayout = new QHBoxLayout();
        codenamehorizontalLayout->setSpacing(6);
        codenamehorizontalLayout->setObjectName(QStringLiteral("codenamehorizontalLayout"));
        CodeName = new QLabel(processorgroupBox);
        CodeName->setObjectName(QStringLiteral("CodeName"));

        codenamehorizontalLayout->addWidget(CodeName);

        codenamelefthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        codenamehorizontalLayout->addItem(codenamelefthorizontalSpacer);

        codenamelineEdit = new QLineEdit(processorgroupBox);
        codenamelineEdit->setObjectName(QStringLiteral("codenamelineEdit"));
        codenamelineEdit->setMinimumSize(QSize(150, 0));
        codenamelineEdit->setMaximumSize(QSize(150, 16777215));
        codenamelineEdit->setFont(font);
        codenamelineEdit->setFocusPolicy(Qt::NoFocus);
        codenamelineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        codenamelineEdit->setFrame(true);
        codenamelineEdit->setAlignment(Qt::AlignCenter);
        codenamelineEdit->setDragEnabled(false);
        codenamelineEdit->setReadOnly(true);
        codenamelineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        codenamehorizontalLayout->addWidget(codenamelineEdit);

        codenametighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        codenamehorizontalLayout->addItem(codenametighthorizontalSpacer);


        gridLayout->addLayout(codenamehorizontalLayout, 2, 0, 1, 1);


        verticalLayout_2->addWidget(processorgroupBox);

        cpucacheclockhorizontalLayout = new QHBoxLayout();
        cpucacheclockhorizontalLayout->setSpacing(6);
        cpucacheclockhorizontalLayout->setObjectName(QStringLiteral("cpucacheclockhorizontalLayout"));
        clockgroupBox = new QGroupBox(CPUPage);
        clockgroupBox->setObjectName(QStringLiteral("clockgroupBox"));
        verticalLayout_4 = new QVBoxLayout(clockgroupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        currentclockhorizontalLayout = new QHBoxLayout();
        currentclockhorizontalLayout->setSpacing(6);
        currentclockhorizontalLayout->setObjectName(QStringLiteral("currentclockhorizontalLayout"));
        CurrentClock = new QLabel(clockgroupBox);
        CurrentClock->setObjectName(QStringLiteral("CurrentClock"));

        currentclockhorizontalLayout->addWidget(CurrentClock);

        currentclocklineEdit = new QLineEdit(clockgroupBox);
        currentclocklineEdit->setObjectName(QStringLiteral("currentclocklineEdit"));
        currentclocklineEdit->setMinimumSize(QSize(150, 0));
        currentclocklineEdit->setMaximumSize(QSize(150, 16777215));
        currentclocklineEdit->setFont(font);
        currentclocklineEdit->setFocusPolicy(Qt::NoFocus);
        currentclocklineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        currentclocklineEdit->setFrame(true);
        currentclocklineEdit->setAlignment(Qt::AlignCenter);
        currentclocklineEdit->setDragEnabled(false);
        currentclocklineEdit->setReadOnly(true);
        currentclocklineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        currentclockhorizontalLayout->addWidget(currentclocklineEdit);

        currentclockhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        currentclockhorizontalLayout->addItem(currentclockhorizontalSpacer);


        verticalLayout_4->addLayout(currentclockhorizontalLayout);

        buspeedhorizontalLayout = new QHBoxLayout();
        buspeedhorizontalLayout->setSpacing(6);
        buspeedhorizontalLayout->setObjectName(QStringLiteral("buspeedhorizontalLayout"));
        busspeed = new QLabel(clockgroupBox);
        busspeed->setObjectName(QStringLiteral("busspeed"));

        buspeedhorizontalLayout->addWidget(busspeed);

        busspeedlineEdit = new QLineEdit(clockgroupBox);
        busspeedlineEdit->setObjectName(QStringLiteral("busspeedlineEdit"));
        busspeedlineEdit->setEnabled(false);
        busspeedlineEdit->setMinimumSize(QSize(150, 0));
        busspeedlineEdit->setMaximumSize(QSize(150, 16777215));
        busspeedlineEdit->setFont(font);
        busspeedlineEdit->setFocusPolicy(Qt::NoFocus);
        busspeedlineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        busspeedlineEdit->setFrame(true);
        busspeedlineEdit->setEchoMode(QLineEdit::Normal);
        busspeedlineEdit->setAlignment(Qt::AlignCenter);
        busspeedlineEdit->setDragEnabled(false);
        busspeedlineEdit->setReadOnly(true);
        busspeedlineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        buspeedhorizontalLayout->addWidget(busspeedlineEdit);

        busspeedhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        buspeedhorizontalLayout->addItem(busspeedhorizontalSpacer);


        verticalLayout_4->addLayout(buspeedhorizontalLayout);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        CPUNAME_16 = new QLabel(clockgroupBox);
        CPUNAME_16->setObjectName(QStringLiteral("CPUNAME_16"));

        horizontalLayout_18->addWidget(CPUNAME_16);

        cpunamelineEdit_16 = new QLineEdit(clockgroupBox);
        cpunamelineEdit_16->setObjectName(QStringLiteral("cpunamelineEdit_16"));
        cpunamelineEdit_16->setMinimumSize(QSize(150, 0));
        cpunamelineEdit_16->setMaximumSize(QSize(150, 16777215));
        cpunamelineEdit_16->setFont(font);
        cpunamelineEdit_16->setFocusPolicy(Qt::NoFocus);
        cpunamelineEdit_16->setContextMenuPolicy(Qt::DefaultContextMenu);
        cpunamelineEdit_16->setFrame(true);
        cpunamelineEdit_16->setAlignment(Qt::AlignCenter);
        cpunamelineEdit_16->setDragEnabled(false);
        cpunamelineEdit_16->setReadOnly(true);
        cpunamelineEdit_16->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_18->addWidget(cpunamelineEdit_16);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_18->addItem(horizontalSpacer_16);


        verticalLayout_4->addLayout(horizontalLayout_18);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        CPUNAME_17 = new QLabel(clockgroupBox);
        CPUNAME_17->setObjectName(QStringLiteral("CPUNAME_17"));

        horizontalLayout_19->addWidget(CPUNAME_17);

        cpunamelineEdit_17 = new QLineEdit(clockgroupBox);
        cpunamelineEdit_17->setObjectName(QStringLiteral("cpunamelineEdit_17"));
        cpunamelineEdit_17->setMinimumSize(QSize(150, 0));
        cpunamelineEdit_17->setMaximumSize(QSize(150, 16777215));
        cpunamelineEdit_17->setFont(font);
        cpunamelineEdit_17->setFocusPolicy(Qt::NoFocus);
        cpunamelineEdit_17->setContextMenuPolicy(Qt::DefaultContextMenu);
        cpunamelineEdit_17->setFrame(true);
        cpunamelineEdit_17->setAlignment(Qt::AlignCenter);
        cpunamelineEdit_17->setDragEnabled(false);
        cpunamelineEdit_17->setReadOnly(true);
        cpunamelineEdit_17->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_19->addWidget(cpunamelineEdit_17);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_19->addItem(horizontalSpacer_17);


        verticalLayout_4->addLayout(horizontalLayout_19);


        cpucacheclockhorizontalLayout->addWidget(clockgroupBox);

        cachegroupBox = new QGroupBox(CPUPage);
        cachegroupBox->setObjectName(QStringLiteral("cachegroupBox"));
        cachegroupBox->setMaximumSize(QSize(16777203, 16777215));
        verticalLayout_3 = new QVBoxLayout(cachegroupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        l1insthorizontalLayout = new QHBoxLayout();
        l1insthorizontalLayout->setSpacing(6);
        l1insthorizontalLayout->setObjectName(QStringLiteral("l1insthorizontalLayout"));
        L1inst = new QLabel(cachegroupBox);
        L1inst->setObjectName(QStringLiteral("L1inst"));

        l1insthorizontalLayout->addWidget(L1inst);

        l1instlineEdit = new QLineEdit(cachegroupBox);
        l1instlineEdit->setObjectName(QStringLiteral("l1instlineEdit"));
        l1instlineEdit->setMinimumSize(QSize(150, 0));
        l1instlineEdit->setMaximumSize(QSize(150, 16777215));
        l1instlineEdit->setFont(font);
        l1instlineEdit->setFocusPolicy(Qt::NoFocus);
        l1instlineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        l1instlineEdit->setFrame(true);
        l1instlineEdit->setAlignment(Qt::AlignCenter);
        l1instlineEdit->setDragEnabled(false);
        l1instlineEdit->setReadOnly(true);
        l1instlineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        l1insthorizontalLayout->addWidget(l1instlineEdit);

        l1instlefthorizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        l1insthorizontalLayout->addItem(l1instlefthorizontalSpacer);

        l1instwaylineEdit = new QLineEdit(cachegroupBox);
        l1instwaylineEdit->setObjectName(QStringLiteral("l1instwaylineEdit"));
        l1instwaylineEdit->setMinimumSize(QSize(150, 0));
        l1instwaylineEdit->setMaximumSize(QSize(150, 16777215));
        l1instwaylineEdit->setFont(font);
        l1instwaylineEdit->setFocusPolicy(Qt::NoFocus);
        l1instwaylineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        l1instwaylineEdit->setFrame(true);
        l1instwaylineEdit->setAlignment(Qt::AlignCenter);
        l1instwaylineEdit->setDragEnabled(false);
        l1instwaylineEdit->setReadOnly(true);
        l1instwaylineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        l1insthorizontalLayout->addWidget(l1instwaylineEdit);

        l1instrighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        l1insthorizontalLayout->addItem(l1instrighthorizontalSpacer);


        verticalLayout_3->addLayout(l1insthorizontalLayout);

        l1datahorizontalLayout = new QHBoxLayout();
        l1datahorizontalLayout->setSpacing(6);
        l1datahorizontalLayout->setObjectName(QStringLiteral("l1datahorizontalLayout"));
        l1data = new QLabel(cachegroupBox);
        l1data->setObjectName(QStringLiteral("l1data"));

        l1datahorizontalLayout->addWidget(l1data);

        l1datalineEdit = new QLineEdit(cachegroupBox);
        l1datalineEdit->setObjectName(QStringLiteral("l1datalineEdit"));
        l1datalineEdit->setMinimumSize(QSize(150, 0));
        l1datalineEdit->setMaximumSize(QSize(150, 16777215));
        l1datalineEdit->setFont(font);
        l1datalineEdit->setFocusPolicy(Qt::NoFocus);
        l1datalineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        l1datalineEdit->setFrame(true);
        l1datalineEdit->setAlignment(Qt::AlignCenter);
        l1datalineEdit->setDragEnabled(false);
        l1datalineEdit->setReadOnly(true);
        l1datalineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        l1datahorizontalLayout->addWidget(l1datalineEdit);

        l1datalefthorizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        l1datahorizontalLayout->addItem(l1datalefthorizontalSpacer);

        l1datawaylineEdit = new QLineEdit(cachegroupBox);
        l1datawaylineEdit->setObjectName(QStringLiteral("l1datawaylineEdit"));
        l1datawaylineEdit->setMinimumSize(QSize(150, 0));
        l1datawaylineEdit->setMaximumSize(QSize(150, 16777215));
        l1datawaylineEdit->setFont(font);
        l1datawaylineEdit->setFocusPolicy(Qt::NoFocus);
        l1datawaylineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        l1datawaylineEdit->setFrame(true);
        l1datawaylineEdit->setAlignment(Qt::AlignCenter);
        l1datawaylineEdit->setDragEnabled(false);
        l1datawaylineEdit->setReadOnly(true);
        l1datawaylineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        l1datahorizontalLayout->addWidget(l1datawaylineEdit);

        l1datarighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        l1datahorizontalLayout->addItem(l1datarighthorizontalSpacer);


        verticalLayout_3->addLayout(l1datahorizontalLayout);

        l2datahorizontalLayout = new QHBoxLayout();
        l2datahorizontalLayout->setSpacing(6);
        l2datahorizontalLayout->setObjectName(QStringLiteral("l2datahorizontalLayout"));
        l2data = new QLabel(cachegroupBox);
        l2data->setObjectName(QStringLiteral("l2data"));

        l2datahorizontalLayout->addWidget(l2data);

        l2datalineEdit = new QLineEdit(cachegroupBox);
        l2datalineEdit->setObjectName(QStringLiteral("l2datalineEdit"));
        l2datalineEdit->setMinimumSize(QSize(150, 0));
        l2datalineEdit->setMaximumSize(QSize(150, 16777215));
        l2datalineEdit->setFont(font);
        l2datalineEdit->setFocusPolicy(Qt::NoFocus);
        l2datalineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        l2datalineEdit->setFrame(true);
        l2datalineEdit->setAlignment(Qt::AlignCenter);
        l2datalineEdit->setDragEnabled(false);
        l2datalineEdit->setReadOnly(true);
        l2datalineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        l2datahorizontalLayout->addWidget(l2datalineEdit);

        l2datalefthorizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        l2datahorizontalLayout->addItem(l2datalefthorizontalSpacer);

        l2datawaylineEdit = new QLineEdit(cachegroupBox);
        l2datawaylineEdit->setObjectName(QStringLiteral("l2datawaylineEdit"));
        l2datawaylineEdit->setMinimumSize(QSize(150, 0));
        l2datawaylineEdit->setMaximumSize(QSize(150, 16777215));
        l2datawaylineEdit->setFont(font);
        l2datawaylineEdit->setFocusPolicy(Qt::NoFocus);
        l2datawaylineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        l2datawaylineEdit->setFrame(true);
        l2datawaylineEdit->setAlignment(Qt::AlignCenter);
        l2datawaylineEdit->setDragEnabled(false);
        l2datawaylineEdit->setReadOnly(true);
        l2datawaylineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        l2datahorizontalLayout->addWidget(l2datawaylineEdit);

        l2datarighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        l2datahorizontalLayout->addItem(l2datarighthorizontalSpacer);


        verticalLayout_3->addLayout(l2datahorizontalLayout);

        l3horizontalLayout = new QHBoxLayout();
        l3horizontalLayout->setSpacing(6);
        l3horizontalLayout->setObjectName(QStringLiteral("l3horizontalLayout"));
        l3data = new QLabel(cachegroupBox);
        l3data->setObjectName(QStringLiteral("l3data"));

        l3horizontalLayout->addWidget(l3data);

        l3datalineEdit = new QLineEdit(cachegroupBox);
        l3datalineEdit->setObjectName(QStringLiteral("l3datalineEdit"));
        l3datalineEdit->setMinimumSize(QSize(150, 0));
        l3datalineEdit->setMaximumSize(QSize(150, 16777215));
        l3datalineEdit->setFont(font);
        l3datalineEdit->setFocusPolicy(Qt::NoFocus);
        l3datalineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        l3datalineEdit->setFrame(true);
        l3datalineEdit->setAlignment(Qt::AlignCenter);
        l3datalineEdit->setDragEnabled(false);
        l3datalineEdit->setReadOnly(true);
        l3datalineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        l3horizontalLayout->addWidget(l3datalineEdit);

        l3datalefthorizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        l3horizontalLayout->addItem(l3datalefthorizontalSpacer);

        l3datawaylineEdit = new QLineEdit(cachegroupBox);
        l3datawaylineEdit->setObjectName(QStringLiteral("l3datawaylineEdit"));
        l3datawaylineEdit->setMinimumSize(QSize(150, 0));
        l3datawaylineEdit->setMaximumSize(QSize(150, 16777215));
        l3datawaylineEdit->setFont(font);
        l3datawaylineEdit->setMouseTracking(false);
        l3datawaylineEdit->setFocusPolicy(Qt::NoFocus);
        l3datawaylineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        l3datawaylineEdit->setFrame(true);
        l3datawaylineEdit->setAlignment(Qt::AlignCenter);
        l3datawaylineEdit->setDragEnabled(false);
        l3datawaylineEdit->setReadOnly(true);
        l3datawaylineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        l3horizontalLayout->addWidget(l3datawaylineEdit);

        l3datarighthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        l3horizontalLayout->addItem(l3datarighthorizontalSpacer);


        verticalLayout_3->addLayout(l3horizontalLayout);


        cpucacheclockhorizontalLayout->addWidget(cachegroupBox);


        verticalLayout_2->addLayout(cpucacheclockhorizontalLayout);

        hardwarestackedWidget->addWidget(CPUPage);
        MemoryPage = new QWidget();
        MemoryPage->setObjectName(QStringLiteral("MemoryPage"));
        verticalLayout_6 = new QVBoxLayout(MemoryPage);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        memoryinfogroupBox = new QGroupBox(MemoryPage);
        memoryinfogroupBox->setObjectName(QStringLiteral("memoryinfogroupBox"));
        memoryinfogroupBox->setMinimumSize(QSize(0, 324));
        memoryinfogroupBox->setMaximumSize(QSize(1666666, 324));
        verticalLayout_7 = new QVBoxLayout(memoryinfogroupBox);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_39 = new QHBoxLayout();
        horizontalLayout_39->setSpacing(6);
        horizontalLayout_39->setObjectName(QStringLiteral("horizontalLayout_39"));
        memmoryslotscrollArea = new QScrollArea(memoryinfogroupBox);
        memmoryslotscrollArea->setObjectName(QStringLiteral("memmoryslotscrollArea"));
        memmoryslotscrollArea->setMinimumSize(QSize(61, 16));
        memmoryslotscrollArea->setMaximumSize(QSize(61, 16));
        memmoryslotscrollArea->setWidgetResizable(true);
        slotscrollAreaWidgetContents = new QWidget();
        slotscrollAreaWidgetContents->setObjectName(QStringLiteral("slotscrollAreaWidgetContents"));
        slotscrollAreaWidgetContents->setGeometry(QRect(0, 0, 98, 28));
        memmoryslotscrollArea->setWidget(slotscrollAreaWidgetContents);

        horizontalLayout_39->addWidget(memmoryslotscrollArea);

        memtypehorizontalLayout = new QHBoxLayout();
        memtypehorizontalLayout->setSpacing(6);
        memtypehorizontalLayout->setObjectName(QStringLiteral("memtypehorizontalLayout"));
        memtype = new QLabel(memoryinfogroupBox);
        memtype->setObjectName(QStringLiteral("memtype"));

        memtypehorizontalLayout->addWidget(memtype);

        memtypelineEdit = new QLineEdit(memoryinfogroupBox);
        memtypelineEdit->setObjectName(QStringLiteral("memtypelineEdit"));
        memtypelineEdit->setMinimumSize(QSize(150, 0));
        memtypelineEdit->setMaximumSize(QSize(150, 16777215));
        memtypelineEdit->setFont(font);
        memtypelineEdit->setFocusPolicy(Qt::NoFocus);
        memtypelineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        memtypelineEdit->setFrame(true);
        memtypelineEdit->setDragEnabled(false);
        memtypelineEdit->setReadOnly(true);
        memtypelineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        memtypehorizontalLayout->addWidget(memtypelineEdit);

        memtypehorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        memtypehorizontalLayout->addItem(memtypehorizontalSpacer);


        horizontalLayout_39->addLayout(memtypehorizontalLayout);


        verticalLayout_7->addLayout(horizontalLayout_39);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        memModelSize = new QLabel(memoryinfogroupBox);
        memModelSize->setObjectName(QStringLiteral("memModelSize"));

        horizontalLayout_20->addWidget(memModelSize);

        memmodelsizelineEdit = new QLineEdit(memoryinfogroupBox);
        memmodelsizelineEdit->setObjectName(QStringLiteral("memmodelsizelineEdit"));
        memmodelsizelineEdit->setMinimumSize(QSize(150, 0));
        memmodelsizelineEdit->setMaximumSize(QSize(150, 16777215));
        memmodelsizelineEdit->setFont(font);
        memmodelsizelineEdit->setFocusPolicy(Qt::NoFocus);
        memmodelsizelineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        memmodelsizelineEdit->setFrame(true);
        memmodelsizelineEdit->setDragEnabled(false);
        memmodelsizelineEdit->setReadOnly(true);
        memmodelsizelineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_20->addWidget(memmodelsizelineEdit);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_18);


        verticalLayout_9->addLayout(horizontalLayout_20);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        memmaxbrandwidth = new QLabel(memoryinfogroupBox);
        memmaxbrandwidth->setObjectName(QStringLiteral("memmaxbrandwidth"));

        horizontalLayout_21->addWidget(memmaxbrandwidth);

        memmaxbrandwidthlineEdit = new QLineEdit(memoryinfogroupBox);
        memmaxbrandwidthlineEdit->setObjectName(QStringLiteral("memmaxbrandwidthlineEdit"));
        memmaxbrandwidthlineEdit->setMinimumSize(QSize(150, 0));
        memmaxbrandwidthlineEdit->setMaximumSize(QSize(150, 16777215));
        memmaxbrandwidthlineEdit->setFont(font);
        memmaxbrandwidthlineEdit->setFocusPolicy(Qt::NoFocus);
        memmaxbrandwidthlineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        memmaxbrandwidthlineEdit->setFrame(true);
        memmaxbrandwidthlineEdit->setDragEnabled(false);
        memmaxbrandwidthlineEdit->setReadOnly(true);
        memmaxbrandwidthlineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_21->addWidget(memmaxbrandwidthlineEdit);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_21->addItem(horizontalSpacer_19);


        verticalLayout_9->addLayout(horizontalLayout_21);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        memmanufacturer = new QLabel(memoryinfogroupBox);
        memmanufacturer->setObjectName(QStringLiteral("memmanufacturer"));

        horizontalLayout_22->addWidget(memmanufacturer);

        memmanufacturerlineEdit = new QLineEdit(memoryinfogroupBox);
        memmanufacturerlineEdit->setObjectName(QStringLiteral("memmanufacturerlineEdit"));
        memmanufacturerlineEdit->setMinimumSize(QSize(150, 0));
        memmanufacturerlineEdit->setMaximumSize(QSize(150, 16777215));
        memmanufacturerlineEdit->setFont(font);
        memmanufacturerlineEdit->setFocusPolicy(Qt::NoFocus);
        memmanufacturerlineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        memmanufacturerlineEdit->setFrame(true);
        memmanufacturerlineEdit->setDragEnabled(false);
        memmanufacturerlineEdit->setReadOnly(true);
        memmanufacturerlineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_22->addWidget(memmanufacturerlineEdit);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_22->addItem(horizontalSpacer_20);


        verticalLayout_9->addLayout(horizontalLayout_22);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        mempartnumber = new QLabel(memoryinfogroupBox);
        mempartnumber->setObjectName(QStringLiteral("mempartnumber"));

        horizontalLayout_23->addWidget(mempartnumber);

        mempartnumberlineEdit = new QLineEdit(memoryinfogroupBox);
        mempartnumberlineEdit->setObjectName(QStringLiteral("mempartnumberlineEdit"));
        mempartnumberlineEdit->setMinimumSize(QSize(150, 0));
        mempartnumberlineEdit->setMaximumSize(QSize(150, 16777215));
        mempartnumberlineEdit->setFont(font);
        mempartnumberlineEdit->setFocusPolicy(Qt::NoFocus);
        mempartnumberlineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        mempartnumberlineEdit->setFrame(true);
        mempartnumberlineEdit->setDragEnabled(false);
        mempartnumberlineEdit->setReadOnly(true);
        mempartnumberlineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_23->addWidget(mempartnumberlineEdit);

        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_23->addItem(horizontalSpacer_21);


        verticalLayout_9->addLayout(horizontalLayout_23);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setSpacing(6);
        horizontalLayout_24->setObjectName(QStringLiteral("horizontalLayout_24"));
        memserialnumber = new QLabel(memoryinfogroupBox);
        memserialnumber->setObjectName(QStringLiteral("memserialnumber"));

        horizontalLayout_24->addWidget(memserialnumber);

        memserialnumberlineEdit = new QLineEdit(memoryinfogroupBox);
        memserialnumberlineEdit->setObjectName(QStringLiteral("memserialnumberlineEdit"));
        memserialnumberlineEdit->setMinimumSize(QSize(150, 0));
        memserialnumberlineEdit->setMaximumSize(QSize(150, 16777215));
        memserialnumberlineEdit->setFont(font);
        memserialnumberlineEdit->setFocusPolicy(Qt::NoFocus);
        memserialnumberlineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        memserialnumberlineEdit->setFrame(true);
        memserialnumberlineEdit->setDragEnabled(false);
        memserialnumberlineEdit->setReadOnly(true);
        memserialnumberlineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_24->addWidget(memserialnumberlineEdit);

        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_24->addItem(horizontalSpacer_22);


        verticalLayout_9->addLayout(horizontalLayout_24);


        horizontalLayout_9->addLayout(verticalLayout_9);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        horizontalLayout_25 = new QHBoxLayout();
        horizontalLayout_25->setSpacing(6);
        horizontalLayout_25->setObjectName(QStringLiteral("horizontalLayout_25"));
        memCorrection = new QLabel(memoryinfogroupBox);
        memCorrection->setObjectName(QStringLiteral("memCorrection"));

        horizontalLayout_25->addWidget(memCorrection);

        memcorrectionlineEdit = new QLineEdit(memoryinfogroupBox);
        memcorrectionlineEdit->setObjectName(QStringLiteral("memcorrectionlineEdit"));
        memcorrectionlineEdit->setMinimumSize(QSize(150, 0));
        memcorrectionlineEdit->setMaximumSize(QSize(150, 16777215));
        memcorrectionlineEdit->setFont(font);
        memcorrectionlineEdit->setFocusPolicy(Qt::NoFocus);
        memcorrectionlineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        memcorrectionlineEdit->setFrame(true);
        memcorrectionlineEdit->setDragEnabled(false);
        memcorrectionlineEdit->setReadOnly(true);
        memcorrectionlineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_25->addWidget(memcorrectionlineEdit);

        horizontalSpacer_23 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_25->addItem(horizontalSpacer_23);


        verticalLayout_8->addLayout(horizontalLayout_25);

        horizontalLayout_38 = new QHBoxLayout();
        horizontalLayout_38->setSpacing(6);
        horizontalLayout_38->setObjectName(QStringLiteral("horizontalLayout_38"));
        memRegistered = new QLabel(memoryinfogroupBox);
        memRegistered->setObjectName(QStringLiteral("memRegistered"));

        horizontalLayout_38->addWidget(memRegistered);

        memregisteredlineEdit = new QLineEdit(memoryinfogroupBox);
        memregisteredlineEdit->setObjectName(QStringLiteral("memregisteredlineEdit"));
        memregisteredlineEdit->setMinimumSize(QSize(150, 0));
        memregisteredlineEdit->setMaximumSize(QSize(150, 16777215));
        memregisteredlineEdit->setFont(font);
        memregisteredlineEdit->setFocusPolicy(Qt::NoFocus);
        memregisteredlineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        memregisteredlineEdit->setFrame(true);
        memregisteredlineEdit->setDragEnabled(false);
        memregisteredlineEdit->setReadOnly(true);
        memregisteredlineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_38->addWidget(memregisteredlineEdit);

        horizontalSpacer_44 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_38->addItem(horizontalSpacer_44);


        verticalLayout_8->addLayout(horizontalLayout_38);

        horizontalLayout_26 = new QHBoxLayout();
        horizontalLayout_26->setSpacing(6);
        horizontalLayout_26->setObjectName(QStringLiteral("horizontalLayout_26"));
        memRanks = new QLabel(memoryinfogroupBox);
        memRanks->setObjectName(QStringLiteral("memRanks"));

        horizontalLayout_26->addWidget(memRanks);

        memrankslineEdit = new QLineEdit(memoryinfogroupBox);
        memrankslineEdit->setObjectName(QStringLiteral("memrankslineEdit"));
        memrankslineEdit->setMinimumSize(QSize(150, 0));
        memrankslineEdit->setMaximumSize(QSize(150, 16777215));
        memrankslineEdit->setFont(font);
        memrankslineEdit->setFocusPolicy(Qt::NoFocus);
        memrankslineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        memrankslineEdit->setFrame(true);
        memrankslineEdit->setDragEnabled(false);
        memrankslineEdit->setReadOnly(true);
        memrankslineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_26->addWidget(memrankslineEdit);

        horizontalSpacer_24 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_26->addItem(horizontalSpacer_24);


        verticalLayout_8->addLayout(horizontalLayout_26);

        horizontalLayout_36 = new QHBoxLayout();
        horizontalLayout_36->setSpacing(6);
        horizontalLayout_36->setObjectName(QStringLiteral("horizontalLayout_36"));
        memspdext = new QLabel(memoryinfogroupBox);
        memspdext->setObjectName(QStringLiteral("memspdext"));

        horizontalLayout_36->addWidget(memspdext);

        memspdextlineEdit = new QLineEdit(memoryinfogroupBox);
        memspdextlineEdit->setObjectName(QStringLiteral("memspdextlineEdit"));
        memspdextlineEdit->setMinimumSize(QSize(150, 0));
        memspdextlineEdit->setMaximumSize(QSize(150, 16777215));
        memspdextlineEdit->setFont(font);
        memspdextlineEdit->setFocusPolicy(Qt::NoFocus);
        memspdextlineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        memspdextlineEdit->setFrame(true);
        memspdextlineEdit->setDragEnabled(false);
        memspdextlineEdit->setReadOnly(true);
        memspdextlineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_36->addWidget(memspdextlineEdit);

        horizontalSpacer_42 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_36->addItem(horizontalSpacer_42);


        verticalLayout_8->addLayout(horizontalLayout_36);

        horizontalLayout_37 = new QHBoxLayout();
        horizontalLayout_37->setSpacing(6);
        horizontalLayout_37->setObjectName(QStringLiteral("horizontalLayout_37"));
        memweekyear = new QLabel(memoryinfogroupBox);
        memweekyear->setObjectName(QStringLiteral("memweekyear"));

        horizontalLayout_37->addWidget(memweekyear);

        memweekyearlineEdit = new QLineEdit(memoryinfogroupBox);
        memweekyearlineEdit->setObjectName(QStringLiteral("memweekyearlineEdit"));
        memweekyearlineEdit->setMinimumSize(QSize(150, 0));
        memweekyearlineEdit->setMaximumSize(QSize(150, 16777215));
        memweekyearlineEdit->setFont(font);
        memweekyearlineEdit->setFocusPolicy(Qt::NoFocus);
        memweekyearlineEdit->setContextMenuPolicy(Qt::DefaultContextMenu);
        memweekyearlineEdit->setFrame(true);
        memweekyearlineEdit->setDragEnabled(false);
        memweekyearlineEdit->setReadOnly(true);
        memweekyearlineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_37->addWidget(memweekyearlineEdit);

        horizontalSpacer_43 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_37->addItem(horizontalSpacer_43);


        verticalLayout_8->addLayout(horizontalLayout_37);


        horizontalLayout_9->addLayout(verticalLayout_8);


        verticalLayout_7->addLayout(horizontalLayout_9);


        verticalLayout_6->addWidget(memoryinfogroupBox);

        timingtablegroupBox = new QGroupBox(MemoryPage);
        timingtablegroupBox->setObjectName(QStringLiteral("timingtablegroupBox"));
        horizontalLayout_5 = new QHBoxLayout(timingtablegroupBox);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        horizontalLayout_41 = new QHBoxLayout();
        horizontalLayout_41->setSpacing(6);
        horizontalLayout_41->setObjectName(QStringLiteral("horizontalLayout_41"));
        ModelSize_2 = new QLabel(timingtablegroupBox);
        ModelSize_2->setObjectName(QStringLiteral("ModelSize_2"));

        horizontalLayout_41->addWidget(ModelSize_2);

        modelsizelineEdit_2 = new QLineEdit(timingtablegroupBox);
        modelsizelineEdit_2->setObjectName(QStringLiteral("modelsizelineEdit_2"));
        modelsizelineEdit_2->setMinimumSize(QSize(150, 0));
        modelsizelineEdit_2->setMaximumSize(QSize(150, 16777215));
        modelsizelineEdit_2->setFont(font);
        modelsizelineEdit_2->setFocusPolicy(Qt::NoFocus);
        modelsizelineEdit_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        modelsizelineEdit_2->setFrame(true);
        modelsizelineEdit_2->setDragEnabled(false);
        modelsizelineEdit_2->setReadOnly(true);
        modelsizelineEdit_2->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_41->addWidget(modelsizelineEdit_2);

        horizontalSpacer_46 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_41->addItem(horizontalSpacer_46);


        verticalLayout_11->addLayout(horizontalLayout_41);

        horizontalLayout_42 = new QHBoxLayout();
        horizontalLayout_42->setSpacing(6);
        horizontalLayout_42->setObjectName(QStringLiteral("horizontalLayout_42"));
        maxbrandwidth_3 = new QLabel(timingtablegroupBox);
        maxbrandwidth_3->setObjectName(QStringLiteral("maxbrandwidth_3"));

        horizontalLayout_42->addWidget(maxbrandwidth_3);

        maxbrandwidthlineEdit_3 = new QLineEdit(timingtablegroupBox);
        maxbrandwidthlineEdit_3->setObjectName(QStringLiteral("maxbrandwidthlineEdit_3"));
        maxbrandwidthlineEdit_3->setMinimumSize(QSize(150, 0));
        maxbrandwidthlineEdit_3->setMaximumSize(QSize(150, 16777215));
        maxbrandwidthlineEdit_3->setFont(font);
        maxbrandwidthlineEdit_3->setFocusPolicy(Qt::NoFocus);
        maxbrandwidthlineEdit_3->setContextMenuPolicy(Qt::DefaultContextMenu);
        maxbrandwidthlineEdit_3->setFrame(true);
        maxbrandwidthlineEdit_3->setDragEnabled(false);
        maxbrandwidthlineEdit_3->setReadOnly(true);
        maxbrandwidthlineEdit_3->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_42->addWidget(maxbrandwidthlineEdit_3);

        horizontalSpacer_47 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_42->addItem(horizontalSpacer_47);


        verticalLayout_11->addLayout(horizontalLayout_42);

        horizontalLayout_43 = new QHBoxLayout();
        horizontalLayout_43->setSpacing(6);
        horizontalLayout_43->setObjectName(QStringLiteral("horizontalLayout_43"));
        memmanufacturer_3 = new QLabel(timingtablegroupBox);
        memmanufacturer_3->setObjectName(QStringLiteral("memmanufacturer_3"));

        horizontalLayout_43->addWidget(memmanufacturer_3);

        memmanufacturerlineEdit_3 = new QLineEdit(timingtablegroupBox);
        memmanufacturerlineEdit_3->setObjectName(QStringLiteral("memmanufacturerlineEdit_3"));
        memmanufacturerlineEdit_3->setMinimumSize(QSize(150, 0));
        memmanufacturerlineEdit_3->setMaximumSize(QSize(150, 16777215));
        memmanufacturerlineEdit_3->setFont(font);
        memmanufacturerlineEdit_3->setFocusPolicy(Qt::NoFocus);
        memmanufacturerlineEdit_3->setContextMenuPolicy(Qt::DefaultContextMenu);
        memmanufacturerlineEdit_3->setFrame(true);
        memmanufacturerlineEdit_3->setDragEnabled(false);
        memmanufacturerlineEdit_3->setReadOnly(true);
        memmanufacturerlineEdit_3->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_43->addWidget(memmanufacturerlineEdit_3);

        horizontalSpacer_48 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_43->addItem(horizontalSpacer_48);


        verticalLayout_11->addLayout(horizontalLayout_43);

        horizontalLayout_44 = new QHBoxLayout();
        horizontalLayout_44->setSpacing(6);
        horizontalLayout_44->setObjectName(QStringLiteral("horizontalLayout_44"));
        mempartnumber_3 = new QLabel(timingtablegroupBox);
        mempartnumber_3->setObjectName(QStringLiteral("mempartnumber_3"));

        horizontalLayout_44->addWidget(mempartnumber_3);

        mempartnumberlineEdit_3 = new QLineEdit(timingtablegroupBox);
        mempartnumberlineEdit_3->setObjectName(QStringLiteral("mempartnumberlineEdit_3"));
        mempartnumberlineEdit_3->setMinimumSize(QSize(150, 0));
        mempartnumberlineEdit_3->setMaximumSize(QSize(150, 16777215));
        mempartnumberlineEdit_3->setFont(font);
        mempartnumberlineEdit_3->setFocusPolicy(Qt::NoFocus);
        mempartnumberlineEdit_3->setContextMenuPolicy(Qt::DefaultContextMenu);
        mempartnumberlineEdit_3->setFrame(true);
        mempartnumberlineEdit_3->setDragEnabled(false);
        mempartnumberlineEdit_3->setReadOnly(true);
        mempartnumberlineEdit_3->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_44->addWidget(mempartnumberlineEdit_3);

        horizontalSpacer_49 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_44->addItem(horizontalSpacer_49);


        verticalLayout_11->addLayout(horizontalLayout_44);

        horizontalLayout_45 = new QHBoxLayout();
        horizontalLayout_45->setSpacing(6);
        horizontalLayout_45->setObjectName(QStringLiteral("horizontalLayout_45"));
        memserialnumber_3 = new QLabel(timingtablegroupBox);
        memserialnumber_3->setObjectName(QStringLiteral("memserialnumber_3"));

        horizontalLayout_45->addWidget(memserialnumber_3);

        memserialnumberlineEdit_3 = new QLineEdit(timingtablegroupBox);
        memserialnumberlineEdit_3->setObjectName(QStringLiteral("memserialnumberlineEdit_3"));
        memserialnumberlineEdit_3->setMinimumSize(QSize(150, 0));
        memserialnumberlineEdit_3->setMaximumSize(QSize(150, 16777215));
        memserialnumberlineEdit_3->setFont(font);
        memserialnumberlineEdit_3->setFocusPolicy(Qt::NoFocus);
        memserialnumberlineEdit_3->setContextMenuPolicy(Qt::DefaultContextMenu);
        memserialnumberlineEdit_3->setFrame(true);
        memserialnumberlineEdit_3->setDragEnabled(false);
        memserialnumberlineEdit_3->setReadOnly(true);
        memserialnumberlineEdit_3->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_45->addWidget(memserialnumberlineEdit_3);

        horizontalSpacer_50 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_45->addItem(horizontalSpacer_50);


        verticalLayout_11->addLayout(horizontalLayout_45);


        horizontalLayout_5->addLayout(verticalLayout_11);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        horizontalLayout_46 = new QHBoxLayout();
        horizontalLayout_46->setSpacing(6);
        horizontalLayout_46->setObjectName(QStringLiteral("horizontalLayout_46"));
        Correction_2 = new QLabel(timingtablegroupBox);
        Correction_2->setObjectName(QStringLiteral("Correction_2"));

        horizontalLayout_46->addWidget(Correction_2);

        correctionlineEdit_2 = new QLineEdit(timingtablegroupBox);
        correctionlineEdit_2->setObjectName(QStringLiteral("correctionlineEdit_2"));
        correctionlineEdit_2->setMinimumSize(QSize(150, 0));
        correctionlineEdit_2->setMaximumSize(QSize(150, 16777215));
        correctionlineEdit_2->setFont(font);
        correctionlineEdit_2->setFocusPolicy(Qt::NoFocus);
        correctionlineEdit_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        correctionlineEdit_2->setFrame(true);
        correctionlineEdit_2->setDragEnabled(false);
        correctionlineEdit_2->setReadOnly(true);
        correctionlineEdit_2->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_46->addWidget(correctionlineEdit_2);

        horizontalSpacer_51 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_46->addItem(horizontalSpacer_51);


        verticalLayout_12->addLayout(horizontalLayout_46);

        horizontalLayout_47 = new QHBoxLayout();
        horizontalLayout_47->setSpacing(6);
        horizontalLayout_47->setObjectName(QStringLiteral("horizontalLayout_47"));
        Registered_2 = new QLabel(timingtablegroupBox);
        Registered_2->setObjectName(QStringLiteral("Registered_2"));

        horizontalLayout_47->addWidget(Registered_2);

        registeredlineEdit_2 = new QLineEdit(timingtablegroupBox);
        registeredlineEdit_2->setObjectName(QStringLiteral("registeredlineEdit_2"));
        registeredlineEdit_2->setMinimumSize(QSize(150, 0));
        registeredlineEdit_2->setMaximumSize(QSize(150, 16777215));
        registeredlineEdit_2->setFont(font);
        registeredlineEdit_2->setFocusPolicy(Qt::NoFocus);
        registeredlineEdit_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        registeredlineEdit_2->setFrame(true);
        registeredlineEdit_2->setDragEnabled(false);
        registeredlineEdit_2->setReadOnly(true);
        registeredlineEdit_2->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_47->addWidget(registeredlineEdit_2);

        horizontalSpacer_52 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_47->addItem(horizontalSpacer_52);


        verticalLayout_12->addLayout(horizontalLayout_47);

        horizontalLayout_48 = new QHBoxLayout();
        horizontalLayout_48->setSpacing(6);
        horizontalLayout_48->setObjectName(QStringLiteral("horizontalLayout_48"));
        Ranks_2 = new QLabel(timingtablegroupBox);
        Ranks_2->setObjectName(QStringLiteral("Ranks_2"));

        horizontalLayout_48->addWidget(Ranks_2);

        rankslineEdit_2 = new QLineEdit(timingtablegroupBox);
        rankslineEdit_2->setObjectName(QStringLiteral("rankslineEdit_2"));
        rankslineEdit_2->setMinimumSize(QSize(150, 0));
        rankslineEdit_2->setMaximumSize(QSize(150, 16777215));
        rankslineEdit_2->setFont(font);
        rankslineEdit_2->setFocusPolicy(Qt::NoFocus);
        rankslineEdit_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        rankslineEdit_2->setFrame(true);
        rankslineEdit_2->setDragEnabled(false);
        rankslineEdit_2->setReadOnly(true);
        rankslineEdit_2->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_48->addWidget(rankslineEdit_2);

        horizontalSpacer_53 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_48->addItem(horizontalSpacer_53);


        verticalLayout_12->addLayout(horizontalLayout_48);

        horizontalLayout_49 = new QHBoxLayout();
        horizontalLayout_49->setSpacing(6);
        horizontalLayout_49->setObjectName(QStringLiteral("horizontalLayout_49"));
        spdext_2 = new QLabel(timingtablegroupBox);
        spdext_2->setObjectName(QStringLiteral("spdext_2"));

        horizontalLayout_49->addWidget(spdext_2);

        spdextlineEdit_2 = new QLineEdit(timingtablegroupBox);
        spdextlineEdit_2->setObjectName(QStringLiteral("spdextlineEdit_2"));
        spdextlineEdit_2->setMinimumSize(QSize(150, 0));
        spdextlineEdit_2->setMaximumSize(QSize(150, 16777215));
        spdextlineEdit_2->setFont(font);
        spdextlineEdit_2->setFocusPolicy(Qt::NoFocus);
        spdextlineEdit_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        spdextlineEdit_2->setFrame(true);
        spdextlineEdit_2->setDragEnabled(false);
        spdextlineEdit_2->setReadOnly(true);
        spdextlineEdit_2->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_49->addWidget(spdextlineEdit_2);

        horizontalSpacer_54 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_49->addItem(horizontalSpacer_54);


        verticalLayout_12->addLayout(horizontalLayout_49);

        horizontalLayout_50 = new QHBoxLayout();
        horizontalLayout_50->setSpacing(6);
        horizontalLayout_50->setObjectName(QStringLiteral("horizontalLayout_50"));
        weekyear_2 = new QLabel(timingtablegroupBox);
        weekyear_2->setObjectName(QStringLiteral("weekyear_2"));

        horizontalLayout_50->addWidget(weekyear_2);

        weekyearlineEdit_2 = new QLineEdit(timingtablegroupBox);
        weekyearlineEdit_2->setObjectName(QStringLiteral("weekyearlineEdit_2"));
        weekyearlineEdit_2->setMinimumSize(QSize(150, 0));
        weekyearlineEdit_2->setMaximumSize(QSize(150, 16777215));
        weekyearlineEdit_2->setFont(font);
        weekyearlineEdit_2->setFocusPolicy(Qt::NoFocus);
        weekyearlineEdit_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        weekyearlineEdit_2->setFrame(true);
        weekyearlineEdit_2->setDragEnabled(false);
        weekyearlineEdit_2->setReadOnly(true);
        weekyearlineEdit_2->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_50->addWidget(weekyearlineEdit_2);

        horizontalSpacer_55 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        horizontalLayout_50->addItem(horizontalSpacer_55);


        verticalLayout_12->addLayout(horizontalLayout_50);


        horizontalLayout_5->addLayout(verticalLayout_12);


        verticalLayout_6->addWidget(timingtablegroupBox);

        hardwarestackedWidget->addWidget(MemoryPage);
        gpupage = new QWidget();
        gpupage->setObjectName(QStringLiteral("gpupage"));
        horizontalLayout = new QHBoxLayout(gpupage);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        gpupageverticalLayout_1 = new QVBoxLayout();
        gpupageverticalLayout_1->setSpacing(6);
        gpupageverticalLayout_1->setObjectName(QStringLiteral("gpupageverticalLayout_1"));
        gpupageverticalLayout_1_2 = new QVBoxLayout();
        gpupageverticalLayout_1_2->setSpacing(6);
        gpupageverticalLayout_1_2->setObjectName(QStringLiteral("gpupageverticalLayout_1_2"));
        gpunamehorizontalLayout = new QHBoxLayout();
        gpunamehorizontalLayout->setSpacing(6);
        gpunamehorizontalLayout->setObjectName(QStringLiteral("gpunamehorizontalLayout"));
        gpuname = new QLabel(gpupage);
        gpuname->setObjectName(QStringLiteral("gpuname"));

        gpunamehorizontalLayout->addWidget(gpuname);

        gpunamelineEdit = new QLineEdit(gpupage);
        gpunamelineEdit->setObjectName(QStringLiteral("gpunamelineEdit"));
        gpunamelineEdit->setMinimumSize(QSize(150, 0));
        gpunamelineEdit->setMaximumSize(QSize(300, 16777215));
        gpunamelineEdit->setFont(font);
        gpunamelineEdit->setFocusPolicy(Qt::NoFocus);
        gpunamelineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        gpunamelineEdit->setFrame(true);
        gpunamelineEdit->setAlignment(Qt::AlignCenter);
        gpunamelineEdit->setDragEnabled(false);
        gpunamelineEdit->setReadOnly(true);
        gpunamelineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gpunamehorizontalLayout->addWidget(gpunamelineEdit);

        gpunamehorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpunamehorizontalLayout->addItem(gpunamehorizontalSpacer);


        gpupageverticalLayout_1_2->addLayout(gpunamehorizontalLayout);

        gpubiosversionhorizontalLayout = new QHBoxLayout();
        gpubiosversionhorizontalLayout->setSpacing(6);
        gpubiosversionhorizontalLayout->setObjectName(QStringLiteral("gpubiosversionhorizontalLayout"));
        gpubiosversion = new QLabel(gpupage);
        gpubiosversion->setObjectName(QStringLiteral("gpubiosversion"));

        gpubiosversionhorizontalLayout->addWidget(gpubiosversion);

        gpubiosversionlineEdit = new QLineEdit(gpupage);
        gpubiosversionlineEdit->setObjectName(QStringLiteral("gpubiosversionlineEdit"));
        gpubiosversionlineEdit->setMinimumSize(QSize(150, 0));
        gpubiosversionlineEdit->setMaximumSize(QSize(300, 16777215));
        gpubiosversionlineEdit->setFont(font);
        gpubiosversionlineEdit->setFocusPolicy(Qt::NoFocus);
        gpubiosversionlineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        gpubiosversionlineEdit->setFrame(true);
        gpubiosversionlineEdit->setAlignment(Qt::AlignCenter);
        gpubiosversionlineEdit->setDragEnabled(false);
        gpubiosversionlineEdit->setReadOnly(true);
        gpubiosversionlineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gpubiosversionhorizontalLayout->addWidget(gpubiosversionlineEdit);

        gpubiosversionhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpubiosversionhorizontalLayout->addItem(gpubiosversionhorizontalSpacer);


        gpupageverticalLayout_1_2->addLayout(gpubiosversionhorizontalLayout);


        gpupageverticalLayout_1->addLayout(gpupageverticalLayout_1_2);

        gpupageverticalLayout_1_1 = new QVBoxLayout();
        gpupageverticalLayout_1_1->setSpacing(6);
        gpupageverticalLayout_1_1->setObjectName(QStringLiteral("gpupageverticalLayout_1_1"));
        gpucurrentclockhorizontalLayout = new QHBoxLayout();
        gpucurrentclockhorizontalLayout->setSpacing(6);
        gpucurrentclockhorizontalLayout->setObjectName(QStringLiteral("gpucurrentclockhorizontalLayout"));
        gpucurrentclock = new QLabel(gpupage);
        gpucurrentclock->setObjectName(QStringLiteral("gpucurrentclock"));

        gpucurrentclockhorizontalLayout->addWidget(gpucurrentclock);

        gpucurrentclocklineEdit = new QLineEdit(gpupage);
        gpucurrentclocklineEdit->setObjectName(QStringLiteral("gpucurrentclocklineEdit"));
        gpucurrentclocklineEdit->setMinimumSize(QSize(150, 0));
        gpucurrentclocklineEdit->setMaximumSize(QSize(300, 16777215));
        gpucurrentclocklineEdit->setFont(font);
        gpucurrentclocklineEdit->setFocusPolicy(Qt::NoFocus);
        gpucurrentclocklineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        gpucurrentclocklineEdit->setFrame(true);
        gpucurrentclocklineEdit->setAlignment(Qt::AlignCenter);
        gpucurrentclocklineEdit->setDragEnabled(false);
        gpucurrentclocklineEdit->setReadOnly(true);
        gpucurrentclocklineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gpucurrentclockhorizontalLayout->addWidget(gpucurrentclocklineEdit);

        gpucurrentclockhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpucurrentclockhorizontalLayout->addItem(gpucurrentclockhorizontalSpacer);


        gpupageverticalLayout_1_1->addLayout(gpucurrentclockhorizontalLayout);

        gpudedicatedmemoryhorizontalLayout = new QHBoxLayout();
        gpudedicatedmemoryhorizontalLayout->setSpacing(6);
        gpudedicatedmemoryhorizontalLayout->setObjectName(QStringLiteral("gpudedicatedmemoryhorizontalLayout"));
        gpuavailablememory = new QLabel(gpupage);
        gpuavailablememory->setObjectName(QStringLiteral("gpuavailablememory"));

        gpudedicatedmemoryhorizontalLayout->addWidget(gpuavailablememory);

        gpuavailablememorylineEdit = new QLineEdit(gpupage);
        gpuavailablememorylineEdit->setObjectName(QStringLiteral("gpuavailablememorylineEdit"));
        gpuavailablememorylineEdit->setMinimumSize(QSize(150, 0));
        gpuavailablememorylineEdit->setMaximumSize(QSize(300, 16777215));
        gpuavailablememorylineEdit->setFont(font);
        gpuavailablememorylineEdit->setFocusPolicy(Qt::NoFocus);
        gpuavailablememorylineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        gpuavailablememorylineEdit->setFrame(true);
        gpuavailablememorylineEdit->setAlignment(Qt::AlignCenter);
        gpuavailablememorylineEdit->setDragEnabled(false);
        gpuavailablememorylineEdit->setReadOnly(true);
        gpuavailablememorylineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gpudedicatedmemoryhorizontalLayout->addWidget(gpuavailablememorylineEdit);

        gpudedicatedmemoryhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpudedicatedmemoryhorizontalLayout->addItem(gpudedicatedmemoryhorizontalSpacer);


        gpupageverticalLayout_1_1->addLayout(gpudedicatedmemoryhorizontalLayout);

        gpusystemmemoryhorizontalLayout = new QHBoxLayout();
        gpusystemmemoryhorizontalLayout->setSpacing(6);
        gpusystemmemoryhorizontalLayout->setObjectName(QStringLiteral("gpusystemmemoryhorizontalLayout"));
        gpuvirtualmemory = new QLabel(gpupage);
        gpuvirtualmemory->setObjectName(QStringLiteral("gpuvirtualmemory"));

        gpusystemmemoryhorizontalLayout->addWidget(gpuvirtualmemory);

        gpuusedmemorylineEdit = new QLineEdit(gpupage);
        gpuusedmemorylineEdit->setObjectName(QStringLiteral("gpuusedmemorylineEdit"));
        gpuusedmemorylineEdit->setMinimumSize(QSize(150, 0));
        gpuusedmemorylineEdit->setMaximumSize(QSize(300, 16777215));
        gpuusedmemorylineEdit->setFont(font);
        gpuusedmemorylineEdit->setFocusPolicy(Qt::NoFocus);
        gpuusedmemorylineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        gpuusedmemorylineEdit->setFrame(true);
        gpuusedmemorylineEdit->setAlignment(Qt::AlignCenter);
        gpuusedmemorylineEdit->setDragEnabled(false);
        gpuusedmemorylineEdit->setReadOnly(true);
        gpuusedmemorylineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gpusystemmemoryhorizontalLayout->addWidget(gpuusedmemorylineEdit);

        gpusystemmemoryhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpusystemmemoryhorizontalLayout->addItem(gpusystemmemoryhorizontalSpacer);


        gpupageverticalLayout_1_1->addLayout(gpusystemmemoryhorizontalLayout);


        gpupageverticalLayout_1->addLayout(gpupageverticalLayout_1_1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gpupageverticalLayout_1->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(gpupageverticalLayout_1);

        gpupageverticalLayout_2 = new QVBoxLayout();
        gpupageverticalLayout_2->setSpacing(6);
        gpupageverticalLayout_2->setObjectName(QStringLiteral("gpupageverticalLayout_2"));
        gpupageverticalLayout_2_2 = new QVBoxLayout();
        gpupageverticalLayout_2_2->setSpacing(6);
        gpupageverticalLayout_2_2->setObjectName(QStringLiteral("gpupageverticalLayout_2_2"));
        gputemperaturehorizontalLayout = new QHBoxLayout();
        gputemperaturehorizontalLayout->setSpacing(6);
        gputemperaturehorizontalLayout->setObjectName(QStringLiteral("gputemperaturehorizontalLayout"));
        gputemperature = new QLabel(gpupage);
        gputemperature->setObjectName(QStringLiteral("gputemperature"));

        gputemperaturehorizontalLayout->addWidget(gputemperature);

        gputemperaturelineEdit = new QLineEdit(gpupage);
        gputemperaturelineEdit->setObjectName(QStringLiteral("gputemperaturelineEdit"));
        gputemperaturelineEdit->setMinimumSize(QSize(150, 0));
        gputemperaturelineEdit->setMaximumSize(QSize(300, 16777215));
        gputemperaturelineEdit->setFont(font);
        gputemperaturelineEdit->setFocusPolicy(Qt::NoFocus);
        gputemperaturelineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        gputemperaturelineEdit->setFrame(true);
        gputemperaturelineEdit->setAlignment(Qt::AlignCenter);
        gputemperaturelineEdit->setDragEnabled(false);
        gputemperaturelineEdit->setReadOnly(true);
        gputemperaturelineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gputemperaturehorizontalLayout->addWidget(gputemperaturelineEdit);

        gputemperaturehorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gputemperaturehorizontalLayout->addItem(gputemperaturehorizontalSpacer);


        gpupageverticalLayout_2_2->addLayout(gputemperaturehorizontalLayout);

        gpufanshorizontalLayout = new QHBoxLayout();
        gpufanshorizontalLayout->setSpacing(6);
        gpufanshorizontalLayout->setObjectName(QStringLiteral("gpufanshorizontalLayout"));
        gpufans = new QLabel(gpupage);
        gpufans->setObjectName(QStringLiteral("gpufans"));

        gpufanshorizontalLayout->addWidget(gpufans);

        gpufanslineEdit = new QLineEdit(gpupage);
        gpufanslineEdit->setObjectName(QStringLiteral("gpufanslineEdit"));
        gpufanslineEdit->setMinimumSize(QSize(150, 0));
        gpufanslineEdit->setMaximumSize(QSize(300, 16777215));
        gpufanslineEdit->setFont(font);
        gpufanslineEdit->setFocusPolicy(Qt::NoFocus);
        gpufanslineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        gpufanslineEdit->setFrame(true);
        gpufanslineEdit->setAlignment(Qt::AlignCenter);
        gpufanslineEdit->setDragEnabled(false);
        gpufanslineEdit->setReadOnly(true);
        gpufanslineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gpufanshorizontalLayout->addWidget(gpufanslineEdit);

        gpufanshorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpufanshorizontalLayout->addItem(gpufanshorizontalSpacer);


        gpupageverticalLayout_2_2->addLayout(gpufanshorizontalLayout);


        gpupageverticalLayout_2->addLayout(gpupageverticalLayout_2_2);

        gpupageverticalLayout_2_1 = new QVBoxLayout();
        gpupageverticalLayout_2_1->setSpacing(6);
        gpupageverticalLayout_2_1->setObjectName(QStringLiteral("gpupageverticalLayout_2_1"));
        gpumemoryclockhorizontalLayout = new QHBoxLayout();
        gpumemoryclockhorizontalLayout->setSpacing(6);
        gpumemoryclockhorizontalLayout->setObjectName(QStringLiteral("gpumemoryclockhorizontalLayout"));
        gpumemoryclock = new QLabel(gpupage);
        gpumemoryclock->setObjectName(QStringLiteral("gpumemoryclock"));

        gpumemoryclockhorizontalLayout->addWidget(gpumemoryclock);

        gpumemoryclocklineEdit = new QLineEdit(gpupage);
        gpumemoryclocklineEdit->setObjectName(QStringLiteral("gpumemoryclocklineEdit"));
        gpumemoryclocklineEdit->setMinimumSize(QSize(150, 0));
        gpumemoryclocklineEdit->setMaximumSize(QSize(300, 16777215));
        gpumemoryclocklineEdit->setFont(font);
        gpumemoryclocklineEdit->setFocusPolicy(Qt::NoFocus);
        gpumemoryclocklineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        gpumemoryclocklineEdit->setFrame(true);
        gpumemoryclocklineEdit->setAlignment(Qt::AlignCenter);
        gpumemoryclocklineEdit->setDragEnabled(false);
        gpumemoryclocklineEdit->setReadOnly(true);
        gpumemoryclocklineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gpumemoryclockhorizontalLayout->addWidget(gpumemoryclocklineEdit);

        gpumemoryclockhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpumemoryclockhorizontalLayout->addItem(gpumemoryclockhorizontalSpacer);


        gpupageverticalLayout_2_1->addLayout(gpumemoryclockhorizontalLayout);

        gpuphysicalmemoryhorizontalLayout = new QHBoxLayout();
        gpuphysicalmemoryhorizontalLayout->setSpacing(6);
        gpuphysicalmemoryhorizontalLayout->setObjectName(QStringLiteral("gpuphysicalmemoryhorizontalLayout"));
        gpuphysicalmemory = new QLabel(gpupage);
        gpuphysicalmemory->setObjectName(QStringLiteral("gpuphysicalmemory"));

        gpuphysicalmemoryhorizontalLayout->addWidget(gpuphysicalmemory);

        gpuphysicalmemorylineEdit = new QLineEdit(gpupage);
        gpuphysicalmemorylineEdit->setObjectName(QStringLiteral("gpuphysicalmemorylineEdit"));
        gpuphysicalmemorylineEdit->setMinimumSize(QSize(150, 0));
        gpuphysicalmemorylineEdit->setMaximumSize(QSize(300, 16777215));
        gpuphysicalmemorylineEdit->setFont(font);
        gpuphysicalmemorylineEdit->setFocusPolicy(Qt::NoFocus);
        gpuphysicalmemorylineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        gpuphysicalmemorylineEdit->setFrame(true);
        gpuphysicalmemorylineEdit->setAlignment(Qt::AlignCenter);
        gpuphysicalmemorylineEdit->setDragEnabled(false);
        gpuphysicalmemorylineEdit->setReadOnly(true);
        gpuphysicalmemorylineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gpuphysicalmemoryhorizontalLayout->addWidget(gpuphysicalmemorylineEdit);

        gpuphysicalmemoryhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpuphysicalmemoryhorizontalLayout->addItem(gpuphysicalmemoryhorizontalSpacer);


        gpupageverticalLayout_2_1->addLayout(gpuphysicalmemoryhorizontalLayout);

        gpuvirtualmemoryhorizontalLayout = new QHBoxLayout();
        gpuvirtualmemoryhorizontalLayout->setSpacing(6);
        gpuvirtualmemoryhorizontalLayout->setObjectName(QStringLiteral("gpuvirtualmemoryhorizontalLayout"));
        gpusharedsystemmemory = new QLabel(gpupage);
        gpusharedsystemmemory->setObjectName(QStringLiteral("gpusharedsystemmemory"));

        gpuvirtualmemoryhorizontalLayout->addWidget(gpusharedsystemmemory);

        gpusharedsystemmemorylineEdit = new QLineEdit(gpupage);
        gpusharedsystemmemorylineEdit->setObjectName(QStringLiteral("gpusharedsystemmemorylineEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(gpusharedsystemmemorylineEdit->sizePolicy().hasHeightForWidth());
        gpusharedsystemmemorylineEdit->setSizePolicy(sizePolicy2);
        gpusharedsystemmemorylineEdit->setMinimumSize(QSize(150, 0));
        gpusharedsystemmemorylineEdit->setMaximumSize(QSize(300, 16777215));
        gpusharedsystemmemorylineEdit->setFont(font);
        gpusharedsystemmemorylineEdit->setFocusPolicy(Qt::NoFocus);
        gpusharedsystemmemorylineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        gpusharedsystemmemorylineEdit->setFrame(true);
        gpusharedsystemmemorylineEdit->setAlignment(Qt::AlignCenter);
        gpusharedsystemmemorylineEdit->setDragEnabled(false);
        gpusharedsystemmemorylineEdit->setReadOnly(true);
        gpusharedsystemmemorylineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        gpuvirtualmemoryhorizontalLayout->addWidget(gpusharedsystemmemorylineEdit);

        gpuvirtualmemoryhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gpuvirtualmemoryhorizontalLayout->addItem(gpuvirtualmemoryhorizontalSpacer);


        gpupageverticalLayout_2_1->addLayout(gpuvirtualmemoryhorizontalLayout);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gpupageverticalLayout_2_1->addItem(verticalSpacer_3);


        gpupageverticalLayout_2->addLayout(gpupageverticalLayout_2_1);


        horizontalLayout->addLayout(gpupageverticalLayout_2);

        hardwarestackedWidget->addWidget(gpupage);
        edidpage = new QWidget();
        edidpage->setObjectName(QStringLiteral("edidpage"));
        verticalLayout_13 = new QVBoxLayout(edidpage);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        edidsplitteedidr = new QSplitter(edidpage);
        edidsplitteedidr->setObjectName(QStringLiteral("edidsplitteedidr"));
        edidsplitteedidr->setOrientation(Qt::Horizontal);
        edidtreeWidget = new edidtreewidget(edidsplitteedidr);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        edidtreeWidget->setHeaderItem(__qtreewidgetitem);
        edidtreeWidget->setObjectName(QStringLiteral("edidtreeWidget"));
        sizePolicy.setHeightForWidth(edidtreeWidget->sizePolicy().hasHeightForWidth());
        edidtreeWidget->setSizePolicy(sizePolicy);
        edidtreeWidget->setMaximumSize(QSize(16777215, 16777215));
        edidtreeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        edidsplitteedidr->addWidget(edidtreeWidget);
        edidtextBrowser = new edidtextbrowser(edidsplitteedidr);
        edidtextBrowser->setObjectName(QStringLiteral("edidtextBrowser"));
        sizePolicy.setHeightForWidth(edidtextBrowser->sizePolicy().hasHeightForWidth());
        edidtextBrowser->setSizePolicy(sizePolicy);
        edidsplitteedidr->addWidget(edidtextBrowser);

        verticalLayout_13->addWidget(edidsplitteedidr);

        hardwarestackedWidget->addWidget(edidpage);
        usbpage = new QWidget();
        usbpage->setObjectName(QStringLiteral("usbpage"));
        horizontalLayout_2 = new QHBoxLayout(usbpage);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        usbsplitter = new QSplitter(usbpage);
        usbsplitter->setObjectName(QStringLiteral("usbsplitter"));
        usbsplitter->setOrientation(Qt::Horizontal);
        usbtreeWidget = new usbtreewidget(usbsplitter);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QStringLiteral("1"));
        usbtreeWidget->setHeaderItem(__qtreewidgetitem1);
        usbtreeWidget->setObjectName(QStringLiteral("usbtreeWidget"));
        usbtreeWidget->setMaximumSize(QSize(16777215, 16777215));
        usbsplitter->addWidget(usbtreeWidget);
        usbtextBrowser = new usbtestbrowser(usbsplitter);
        usbtextBrowser->setObjectName(QStringLiteral("usbtextBrowser"));
        usbsplitter->addWidget(usbtextBrowser);

        horizontalLayout_2->addWidget(usbsplitter);

        hardwarestackedWidget->addWidget(usbpage);
        diskpage = new QWidget();
        diskpage->setObjectName(QStringLiteral("diskpage"));
        verticalLayout_10 = new QVBoxLayout(diskpage);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        diskselecthorizontalLayout = new QHBoxLayout();
        diskselecthorizontalLayout->setSpacing(6);
        diskselecthorizontalLayout->setObjectName(QStringLiteral("diskselecthorizontalLayout"));

        verticalLayout_10->addLayout(diskselecthorizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        disktemperaturelabel = new QLabel(diskpage);
        disktemperaturelabel->setObjectName(QStringLiteral("disktemperaturelabel"));

        horizontalLayout_4->addWidget(disktemperaturelabel);

        disknamelabel = new QLabel(diskpage);
        disknamelabel->setObjectName(QStringLiteral("disknamelabel"));
        disknamelabel->setMaximumSize(QSize(16777215, 50));

        horizontalLayout_4->addWidget(disknamelabel);


        verticalLayout_10->addLayout(horizontalLayout_4);

        diskgroupBox = new QGroupBox(diskpage);
        diskgroupBox->setObjectName(QStringLiteral("diskgroupBox"));
        diskgroupBox->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout_31 = new QHBoxLayout(diskgroupBox);
        horizontalLayout_31->setSpacing(6);
        horizontalLayout_31->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_31->setObjectName(QStringLiteral("horizontalLayout_31"));
        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setSpacing(6);
        horizontalLayout_30->setObjectName(QStringLiteral("horizontalLayout_30"));
        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        diskfirenamehorizontalLayout = new QHBoxLayout();
        diskfirenamehorizontalLayout->setSpacing(6);
        diskfirenamehorizontalLayout->setObjectName(QStringLiteral("diskfirenamehorizontalLayout"));
        diskfirename = new QLabel(diskgroupBox);
        diskfirename->setObjectName(QStringLiteral("diskfirename"));

        diskfirenamehorizontalLayout->addWidget(diskfirename);

        diskfirenamelineEdit = new QLineEdit(diskgroupBox);
        diskfirenamelineEdit->setObjectName(QStringLiteral("diskfirenamelineEdit"));
        diskfirenamelineEdit->setMinimumSize(QSize(150, 0));
        diskfirenamelineEdit->setMaximumSize(QSize(300, 16777215));
        diskfirenamelineEdit->setFont(font);
        diskfirenamelineEdit->setFocusPolicy(Qt::NoFocus);
        diskfirenamelineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        diskfirenamelineEdit->setFrame(true);
        diskfirenamelineEdit->setAlignment(Qt::AlignCenter);
        diskfirenamelineEdit->setDragEnabled(false);
        diskfirenamelineEdit->setReadOnly(true);
        diskfirenamelineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        diskfirenamehorizontalLayout->addWidget(diskfirenamelineEdit);

        diskfirenamehorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        diskfirenamehorizontalLayout->addItem(diskfirenamehorizontalSpacer);


        verticalLayout_16->addLayout(diskfirenamehorizontalLayout);

        diskserialnumhorizontalLayout = new QHBoxLayout();
        diskserialnumhorizontalLayout->setSpacing(6);
        diskserialnumhorizontalLayout->setObjectName(QStringLiteral("diskserialnumhorizontalLayout"));
        diskserialnum = new QLabel(diskgroupBox);
        diskserialnum->setObjectName(QStringLiteral("diskserialnum"));

        diskserialnumhorizontalLayout->addWidget(diskserialnum);

        diskserialnumlineEdit = new QLineEdit(diskgroupBox);
        diskserialnumlineEdit->setObjectName(QStringLiteral("diskserialnumlineEdit"));
        diskserialnumlineEdit->setMinimumSize(QSize(150, 0));
        diskserialnumlineEdit->setMaximumSize(QSize(300, 16777215));
        diskserialnumlineEdit->setFont(font);
        diskserialnumlineEdit->setFocusPolicy(Qt::NoFocus);
        diskserialnumlineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        diskserialnumlineEdit->setFrame(true);
        diskserialnumlineEdit->setAlignment(Qt::AlignCenter);
        diskserialnumlineEdit->setDragEnabled(false);
        diskserialnumlineEdit->setReadOnly(true);
        diskserialnumlineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        diskserialnumhorizontalLayout->addWidget(diskserialnumlineEdit);

        diskserialnumhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        diskserialnumhorizontalLayout->addItem(diskserialnumhorizontalSpacer);


        verticalLayout_16->addLayout(diskserialnumhorizontalLayout);

        diskinterfacehorizontalLayout = new QHBoxLayout();
        diskinterfacehorizontalLayout->setSpacing(6);
        diskinterfacehorizontalLayout->setObjectName(QStringLiteral("diskinterfacehorizontalLayout"));
        diskinterface = new QLabel(diskgroupBox);
        diskinterface->setObjectName(QStringLiteral("diskinterface"));

        diskinterfacehorizontalLayout->addWidget(diskinterface);

        diskinterfacelineEdit = new QLineEdit(diskgroupBox);
        diskinterfacelineEdit->setObjectName(QStringLiteral("diskinterfacelineEdit"));
        diskinterfacelineEdit->setMinimumSize(QSize(150, 0));
        diskinterfacelineEdit->setMaximumSize(QSize(300, 16777215));
        diskinterfacelineEdit->setFont(font);
        diskinterfacelineEdit->setFocusPolicy(Qt::NoFocus);
        diskinterfacelineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        diskinterfacelineEdit->setFrame(true);
        diskinterfacelineEdit->setAlignment(Qt::AlignCenter);
        diskinterfacelineEdit->setDragEnabled(false);
        diskinterfacelineEdit->setReadOnly(true);
        diskinterfacelineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        diskinterfacehorizontalLayout->addWidget(diskinterfacelineEdit);

        diskinterfacehorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        diskinterfacehorizontalLayout->addItem(diskinterfacehorizontalSpacer);


        verticalLayout_16->addLayout(diskinterfacehorizontalLayout);

        disktransfermodehorizontalLayout = new QHBoxLayout();
        disktransfermodehorizontalLayout->setSpacing(6);
        disktransfermodehorizontalLayout->setObjectName(QStringLiteral("disktransfermodehorizontalLayout"));
        disktransfermode = new QLabel(diskgroupBox);
        disktransfermode->setObjectName(QStringLiteral("disktransfermode"));

        disktransfermodehorizontalLayout->addWidget(disktransfermode);

        disktransfermodelineEdit = new QLineEdit(diskgroupBox);
        disktransfermodelineEdit->setObjectName(QStringLiteral("disktransfermodelineEdit"));
        disktransfermodelineEdit->setMinimumSize(QSize(150, 0));
        disktransfermodelineEdit->setMaximumSize(QSize(300, 16777215));
        disktransfermodelineEdit->setFont(font);
        disktransfermodelineEdit->setFocusPolicy(Qt::NoFocus);
        disktransfermodelineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        disktransfermodelineEdit->setFrame(true);
        disktransfermodelineEdit->setAlignment(Qt::AlignCenter);
        disktransfermodelineEdit->setDragEnabled(false);
        disktransfermodelineEdit->setReadOnly(true);
        disktransfermodelineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        disktransfermodehorizontalLayout->addWidget(disktransfermodelineEdit);

        disktransfermodehorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        disktransfermodehorizontalLayout->addItem(disktransfermodehorizontalSpacer);


        verticalLayout_16->addLayout(disktransfermodehorizontalLayout);


        horizontalLayout_30->addLayout(verticalLayout_16);

        verticalLayout_17 = new QVBoxLayout();
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        diskpoweroncounthorizontalLayout = new QHBoxLayout();
        diskpoweroncounthorizontalLayout->setSpacing(6);
        diskpoweroncounthorizontalLayout->setObjectName(QStringLiteral("diskpoweroncounthorizontalLayout"));
        diskpoweroncount = new QLabel(diskgroupBox);
        diskpoweroncount->setObjectName(QStringLiteral("diskpoweroncount"));

        diskpoweroncounthorizontalLayout->addWidget(diskpoweroncount);

        diskpoweroncountlineEdit = new QLineEdit(diskgroupBox);
        diskpoweroncountlineEdit->setObjectName(QStringLiteral("diskpoweroncountlineEdit"));
        diskpoweroncountlineEdit->setMinimumSize(QSize(150, 0));
        diskpoweroncountlineEdit->setMaximumSize(QSize(300, 16777215));
        diskpoweroncountlineEdit->setFont(font);
        diskpoweroncountlineEdit->setFocusPolicy(Qt::NoFocus);
        diskpoweroncountlineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        diskpoweroncountlineEdit->setFrame(true);
        diskpoweroncountlineEdit->setAlignment(Qt::AlignCenter);
        diskpoweroncountlineEdit->setDragEnabled(false);
        diskpoweroncountlineEdit->setReadOnly(true);
        diskpoweroncountlineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        diskpoweroncounthorizontalLayout->addWidget(diskpoweroncountlineEdit);

        diskpoweroncounthorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        diskpoweroncounthorizontalLayout->addItem(diskpoweroncounthorizontalSpacer);


        verticalLayout_17->addLayout(diskpoweroncounthorizontalLayout);

        diskpoweronhourshorizontalLayout = new QHBoxLayout();
        diskpoweronhourshorizontalLayout->setSpacing(6);
        diskpoweronhourshorizontalLayout->setObjectName(QStringLiteral("diskpoweronhourshorizontalLayout"));
        diskpoweronhours = new QLabel(diskgroupBox);
        diskpoweronhours->setObjectName(QStringLiteral("diskpoweronhours"));

        diskpoweronhourshorizontalLayout->addWidget(diskpoweronhours);

        diskpoweronhourslineEdit = new QLineEdit(diskgroupBox);
        diskpoweronhourslineEdit->setObjectName(QStringLiteral("diskpoweronhourslineEdit"));
        diskpoweronhourslineEdit->setMinimumSize(QSize(150, 0));
        diskpoweronhourslineEdit->setMaximumSize(QSize(300, 16777215));
        diskpoweronhourslineEdit->setFont(font);
        diskpoweronhourslineEdit->setFocusPolicy(Qt::NoFocus);
        diskpoweronhourslineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        diskpoweronhourslineEdit->setFrame(true);
        diskpoweronhourslineEdit->setAlignment(Qt::AlignCenter);
        diskpoweronhourslineEdit->setDragEnabled(false);
        diskpoweronhourslineEdit->setReadOnly(true);
        diskpoweronhourslineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        diskpoweronhourshorizontalLayout->addWidget(diskpoweronhourslineEdit);

        diskpoweronhourshorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        diskpoweronhourshorizontalLayout->addItem(diskpoweronhourshorizontalSpacer);


        verticalLayout_17->addLayout(diskpoweronhourshorizontalLayout);

        diskStandardhorizontalLayout = new QHBoxLayout();
        diskStandardhorizontalLayout->setSpacing(6);
        diskStandardhorizontalLayout->setObjectName(QStringLiteral("diskStandardhorizontalLayout"));
        diskStandard = new QLabel(diskgroupBox);
        diskStandard->setObjectName(QStringLiteral("diskStandard"));

        diskStandardhorizontalLayout->addWidget(diskStandard);

        diskstandardlineEdit = new QLineEdit(diskgroupBox);
        diskstandardlineEdit->setObjectName(QStringLiteral("diskstandardlineEdit"));
        diskstandardlineEdit->setMinimumSize(QSize(150, 0));
        diskstandardlineEdit->setMaximumSize(QSize(300, 16777215));
        diskstandardlineEdit->setFont(font);
        diskstandardlineEdit->setFocusPolicy(Qt::NoFocus);
        diskstandardlineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        diskstandardlineEdit->setFrame(true);
        diskstandardlineEdit->setAlignment(Qt::AlignCenter);
        diskstandardlineEdit->setDragEnabled(false);
        diskstandardlineEdit->setReadOnly(true);
        diskstandardlineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        diskStandardhorizontalLayout->addWidget(diskstandardlineEdit);

        diskStandardhorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        diskStandardhorizontalLayout->addItem(diskStandardhorizontalSpacer);


        verticalLayout_17->addLayout(diskStandardhorizontalLayout);

        disksizehorizontalLayout = new QHBoxLayout();
        disksizehorizontalLayout->setSpacing(6);
        disksizehorizontalLayout->setObjectName(QStringLiteral("disksizehorizontalLayout"));
        disksize = new QLabel(diskgroupBox);
        disksize->setObjectName(QStringLiteral("disksize"));

        disksizehorizontalLayout->addWidget(disksize);

        disksizelineEdit = new QLineEdit(diskgroupBox);
        disksizelineEdit->setObjectName(QStringLiteral("disksizelineEdit"));
        disksizelineEdit->setMinimumSize(QSize(150, 0));
        disksizelineEdit->setMaximumSize(QSize(300, 16777215));
        disksizelineEdit->setFont(font);
        disksizelineEdit->setFocusPolicy(Qt::NoFocus);
        disksizelineEdit->setContextMenuPolicy(Qt::NoContextMenu);
        disksizelineEdit->setFrame(true);
        disksizelineEdit->setAlignment(Qt::AlignCenter);
        disksizelineEdit->setDragEnabled(false);
        disksizelineEdit->setReadOnly(true);
        disksizelineEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);

        disksizehorizontalLayout->addWidget(disksizelineEdit);

        disksizehorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        disksizehorizontalLayout->addItem(disksizehorizontalSpacer);


        verticalLayout_17->addLayout(disksizehorizontalLayout);


        horizontalLayout_30->addLayout(verticalLayout_17);


        horizontalLayout_31->addLayout(horizontalLayout_30);


        verticalLayout_10->addWidget(diskgroupBox);

        disktableWidget = new QTableWidget(diskpage);
        disktableWidget->setObjectName(QStringLiteral("disktableWidget"));
        disktableWidget->setMaximumSize(QSize(16777215, 350));

        verticalLayout_10->addWidget(disktableWidget);

        hardwarestackedWidget->addWidget(diskpage);
        smbiospage = new QWidget();
        smbiospage->setObjectName(QStringLiteral("smbiospage"));
        verticalLayout_14 = new QVBoxLayout(smbiospage);
        verticalLayout_14->setSpacing(6);
        verticalLayout_14->setContentsMargins(11, 11, 11, 11);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        smbiossplitter = new QSplitter(smbiospage);
        smbiossplitter->setObjectName(QStringLiteral("smbiossplitter"));
        smbiossplitter->setOrientation(Qt::Horizontal);
        smbiostreeWidget = new smbiostreewidget(smbiossplitter);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem();
        __qtreewidgetitem2->setText(0, QStringLiteral("1"));
        smbiostreeWidget->setHeaderItem(__qtreewidgetitem2);
        smbiostreeWidget->setObjectName(QStringLiteral("smbiostreeWidget"));
        smbiostreeWidget->setMaximumSize(QSize(16777215, 16777215));
        smbiossplitter->addWidget(smbiostreeWidget);
        smbiostextBrowser = new smbiostextbrowser(smbiossplitter);
        smbiostextBrowser->setObjectName(QStringLiteral("smbiostextBrowser"));
        smbiossplitter->addWidget(smbiostextBrowser);

        verticalLayout_14->addWidget(smbiossplitter);

        hardwarestackedWidget->addWidget(smbiospage);
        otherspage = new QWidget();
        otherspage->setObjectName(QStringLiteral("otherspage"));
        hardwarestackedWidget->addWidget(otherspage);

        horizontalLayout_10->addWidget(hardwarestackedWidget);

        MainstackedWidget->addWidget(hardwarepage);
        advencedhardwarepage = new QWidget();
        advencedhardwarepage->setObjectName(QStringLiteral("advencedhardwarepage"));
        advencedhardwarepage->setMouseTracking(true);
        advencedhardwarehorizontalLayout = new QHBoxLayout(advencedhardwarepage);
        advencedhardwarehorizontalLayout->setSpacing(6);
        advencedhardwarehorizontalLayout->setContentsMargins(11, 11, 11, 11);
        advencedhardwarehorizontalLayout->setObjectName(QStringLiteral("advencedhardwarehorizontalLayout"));
        advencedhardwarehorizontalLayout->setContentsMargins(0, 0, 0, 0);
        advencedhardwareselectwidget = new QWidget(advencedhardwarepage);
        advencedhardwareselectwidget->setObjectName(QStringLiteral("advencedhardwareselectwidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(advencedhardwareselectwidget->sizePolicy().hasHeightForWidth());
        advencedhardwareselectwidget->setSizePolicy(sizePolicy3);
        advencedhardwareselectwidget->setMinimumSize(QSize(150, 0));
        advencedhardwareselectwidget->setMaximumSize(QSize(100, 16777215));
        verticalLayout_5 = new QVBoxLayout(advencedhardwareselectwidget);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, -1, 0, -1);
        rwIobutton = new SV_HARDWARE_PUSHBUTTON(advencedhardwareselectwidget);
        rwIobutton->setObjectName(QStringLiteral("rwIobutton"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(rwIobutton->sizePolicy().hasHeightForWidth());
        rwIobutton->setSizePolicy(sizePolicy4);
        rwIobutton->setMinimumSize(QSize(0, 0));
        rwIobutton->setMaximumSize(QSize(16777215, 16777215));
        rwIobutton->setFocusPolicy(Qt::StrongFocus);
        rwIobutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        verticalLayout_5->addWidget(rwIobutton);

        rwmemorybutton = new SV_HARDWARE_PUSHBUTTON(advencedhardwareselectwidget);
        rwmemorybutton->setObjectName(QStringLiteral("rwmemorybutton"));
        sizePolicy4.setHeightForWidth(rwmemorybutton->sizePolicy().hasHeightForWidth());
        rwmemorybutton->setSizePolicy(sizePolicy4);
        rwmemorybutton->setMinimumSize(QSize(0, 0));
        rwmemorybutton->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_5->addWidget(rwmemorybutton);

        rwmsrbutton = new SV_HARDWARE_PUSHBUTTON(advencedhardwareselectwidget);
        rwmsrbutton->setObjectName(QStringLiteral("rwmsrbutton"));
        sizePolicy4.setHeightForWidth(rwmsrbutton->sizePolicy().hasHeightForWidth());
        rwmsrbutton->setSizePolicy(sizePolicy4);
        rwmsrbutton->setMinimumSize(QSize(0, 0));
        rwmsrbutton->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_5->addWidget(rwmsrbutton);

        PCIbutton = new SV_HARDWARE_PUSHBUTTON(advencedhardwareselectwidget);
        PCIbutton->setObjectName(QStringLiteral("PCIbutton"));
        sizePolicy2.setHeightForWidth(PCIbutton->sizePolicy().hasHeightForWidth());
        PCIbutton->setSizePolicy(sizePolicy2);

        verticalLayout_5->addWidget(PCIbutton);

        advencedhardwareselectverticalSpacer = new QSpacerItem(20, 517, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(advencedhardwareselectverticalSpacer);


        advencedhardwarehorizontalLayout->addWidget(advencedhardwareselectwidget);

        advencedhardwarestackedWidget = new QStackedWidget(advencedhardwarepage);
        advencedhardwarestackedWidget->setObjectName(QStringLiteral("advencedhardwarestackedWidget"));
        rwIopage = new QWidget();
        rwIopage->setObjectName(QStringLiteral("rwIopage"));
        verticalLayout_15 = new QVBoxLayout(rwIopage);
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setContentsMargins(11, 11, 11, 11);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        verticalLayout_15->setContentsMargins(0, 0, 0, 0);
        iobasepushButton = new RWBaseButton(rwIopage);
        iobasepushButton->setObjectName(QStringLiteral("iobasepushButton"));
        iobasepushButton->setMaximumSize(QSize(200, 16777215));

        verticalLayout_15->addWidget(iobasepushButton);

        rwiotableWidget = new RW_Qtablewidget(rwIopage);
        rwiotableWidget->setObjectName(QStringLiteral("rwiotableWidget"));

        verticalLayout_15->addWidget(rwiotableWidget);

        advencedhardwarestackedWidget->addWidget(rwIopage);
        rwmemorypage = new QWidget();
        rwmemorypage->setObjectName(QStringLiteral("rwmemorypage"));
        verticalLayout_19 = new QVBoxLayout(rwmemorypage);
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setContentsMargins(11, 11, 11, 11);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        verticalLayout_19->setContentsMargins(0, 0, 0, 0);
        memorybasepushButton = new RWBaseButton(rwmemorypage);
        memorybasepushButton->setObjectName(QStringLiteral("memorybasepushButton"));
        memorybasepushButton->setMaximumSize(QSize(200, 16777215));

        verticalLayout_19->addWidget(memorybasepushButton);

        rwmemorytableWidget = new RW_Qtablewidget(rwmemorypage);
        rwmemorytableWidget->setObjectName(QStringLiteral("rwmemorytableWidget"));

        verticalLayout_19->addWidget(rwmemorytableWidget);

        advencedhardwarestackedWidget->addWidget(rwmemorypage);
        rwmsrpage = new QWidget();
        rwmsrpage->setObjectName(QStringLiteral("rwmsrpage"));
        verticalLayout_21 = new QVBoxLayout(rwmsrpage);
        verticalLayout_21->setSpacing(6);
        verticalLayout_21->setContentsMargins(11, 11, 11, 11);
        verticalLayout_21->setObjectName(QStringLiteral("verticalLayout_21"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);

        msraddrlabel = new QLabel(rwmsrpage);
        msraddrlabel->setObjectName(QStringLiteral("msraddrlabel"));

        horizontalLayout_6->addWidget(msraddrlabel);

        MSRADDRlineEdit = new QLineEdit(rwmsrpage);
        MSRADDRlineEdit->setObjectName(QStringLiteral("MSRADDRlineEdit"));

        horizontalLayout_6->addWidget(MSRADDRlineEdit);

        msreadpushButton = new QPushButton(rwmsrpage);
        msreadpushButton->setObjectName(QStringLiteral("msreadpushButton"));

        horizontalLayout_6->addWidget(msreadpushButton);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout_21->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_2);

        msrdatalabel = new QLabel(rwmsrpage);
        msrdatalabel->setObjectName(QStringLiteral("msrdatalabel"));

        horizontalLayout_7->addWidget(msrdatalabel);

        MSRDATAlineEdit = new QLineEdit(rwmsrpage);
        MSRDATAlineEdit->setObjectName(QStringLiteral("MSRDATAlineEdit"));

        horizontalLayout_7->addWidget(MSRDATAlineEdit);

        mstwritepushButton = new QPushButton(rwmsrpage);
        mstwritepushButton->setObjectName(QStringLiteral("mstwritepushButton"));

        horizontalLayout_7->addWidget(mstwritepushButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);


        verticalLayout_21->addLayout(horizontalLayout_7);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_6);

        msrcorenumlabel = new QLabel(rwmsrpage);
        msrcorenumlabel->setObjectName(QStringLiteral("msrcorenumlabel"));

        horizontalLayout_11->addWidget(msrcorenumlabel);

        MSRcorenumlineEdit = new QLineEdit(rwmsrpage);
        MSRcorenumlineEdit->setObjectName(QStringLiteral("MSRcorenumlineEdit"));

        horizontalLayout_11->addWidget(MSRcorenumlineEdit);

        msrcorenumpushButton = new QPushButton(rwmsrpage);
        msrcorenumpushButton->setObjectName(QStringLiteral("msrcorenumpushButton"));

        horizontalLayout_11->addWidget(msrcorenumpushButton);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_7);


        verticalLayout_21->addLayout(horizontalLayout_11);

        rwmsrtableWidget = new QTableWidget(rwmsrpage);
        rwmsrtableWidget->setObjectName(QStringLiteral("rwmsrtableWidget"));

        verticalLayout_21->addWidget(rwmsrtableWidget);

        advencedhardwarestackedWidget->addWidget(rwmsrpage);
        PCIpage = new QWidget();
        PCIpage->setObjectName(QStringLiteral("PCIpage"));
        verticalLayout = new QVBoxLayout(PCIpage);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pcicomboBox = new pcicombobox(PCIpage);
        pcicomboBox->setObjectName(QStringLiteral("pcicomboBox"));

        verticalLayout->addWidget(pcicomboBox);

        PCIhorizontalLayout = new QHBoxLayout();
        PCIhorizontalLayout->setSpacing(6);
        PCIhorizontalLayout->setObjectName(QStringLiteral("PCIhorizontalLayout"));
        pcitableWidget = new pcitablewidget(PCIpage);
        pcitableWidget->setObjectName(QStringLiteral("pcitableWidget"));

        PCIhorizontalLayout->addWidget(pcitableWidget);

        pcitextBrowser = new QTextBrowser(PCIpage);
        pcitextBrowser->setObjectName(QStringLiteral("pcitextBrowser"));

        PCIhorizontalLayout->addWidget(pcitextBrowser);

        PCIhorizontalLayout->setStretch(0, 8);
        PCIhorizontalLayout->setStretch(1, 2);

        verticalLayout->addLayout(PCIhorizontalLayout);

        advencedhardwarestackedWidget->addWidget(PCIpage);

        advencedhardwarehorizontalLayout->addWidget(advencedhardwarestackedWidget);

        MainstackedWidget->addWidget(advencedhardwarepage);

        verticalLayout_18->addWidget(MainstackedWidget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        MainstackedWidget->setCurrentIndex(2);
        hardwarestackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SV Assistant- Hardware", Q_NULLPTR));
        comprehensivetoolButton->setText(QString());
        hardwaretoolButton->setText(QString());
        advencedhardwaretoolButton->setText(QString());
        settingtoolButton->setText(QString());
        minpushButton->setText(QString());
        closepushButton->setText(QString());
        maincomputertypelabel->setText(QApplication::translate("MainWindow", "Computer Type", Q_NULLPTR));
        maincomputertypedatalabel->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        mainsystemlabel->setText(QApplication::translate("MainWindow", "Operation System", Q_NULLPTR));
        mainsystemdatalabel->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        maincpupushButton->setText(QApplication::translate("MainWindow", "CPU", Q_NULLPTR));
        maincpulabel->setText(QApplication::translate("MainWindow", "Unknown", Q_NULLPTR));
        mainboardpushButton->setText(QApplication::translate("MainWindow", "Board", Q_NULLPTR));
        mainboardlabel->setText(QApplication::translate("MainWindow", "Unknown", Q_NULLPTR));
        mainmemorypushButton->setText(QApplication::translate("MainWindow", "Memory", Q_NULLPTR));
        mainmemorylabel->setText(QApplication::translate("MainWindow", "Unknown", Q_NULLPTR));
        maindiskpushButton->setText(QApplication::translate("MainWindow", "Disk", Q_NULLPTR));
        maindisklabel->setText(QApplication::translate("MainWindow", "Unknown", Q_NULLPTR));
        maingpupushButton->setText(QApplication::translate("MainWindow", "GPU", Q_NULLPTR));
        maingpulabel->setText(QApplication::translate("MainWindow", "Unknown", Q_NULLPTR));
        mainmonitorpushButton->setText(QApplication::translate("MainWindow", "Monitor", Q_NULLPTR));
        mainmonitorlabel->setText(QApplication::translate("MainWindow", "Unknown", Q_NULLPTR));
        mainaudiopushButton->setText(QApplication::translate("MainWindow", "Audio", Q_NULLPTR));
        mainaudiolabel->setText(QApplication::translate("MainWindow", "Unknown", Q_NULLPTR));
        mainnetpushButton->setText(QApplication::translate("MainWindow", "Network", Q_NULLPTR));
        mainnetlabel->setText(QApplication::translate("MainWindow", "Unknown", Q_NULLPTR));
        CPUbutton->setText(QApplication::translate("MainWindow", "CPU", Q_NULLPTR));
        memorybutton->setText(QApplication::translate("MainWindow", "Memory", Q_NULLPTR));
        GPUbutton->setText(QApplication::translate("MainWindow", "GPU", Q_NULLPTR));
        EDIDbutton->setText(QApplication::translate("MainWindow", "Mointor", Q_NULLPTR));
        USBbutton->setText(QApplication::translate("MainWindow", "USB", Q_NULLPTR));
        Diskbutton->setText(QApplication::translate("MainWindow", "Disk", Q_NULLPTR));
        SMBIOSbutton->setText(QApplication::translate("MainWindow", "SMBIOS", Q_NULLPTR));
        othersbutton->setText(QApplication::translate("MainWindow", "Others", Q_NULLPTR));
        processorgroupBox->setTitle(QApplication::translate("MainWindow", "Processor", Q_NULLPTR));
        Package->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Package:</p></body></html>", Q_NULLPTR));
        Manufacture->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Manufacture:</p></body></html>", Q_NULLPTR));
        processorid->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Processor ID:</p></body></html>", Q_NULLPTR));
        Instructions->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Instructions:</p></body></html>", Q_NULLPTR));
        Family->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Family:</p></body></html>", Q_NULLPTR));
        Model->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Model:</p></body></html>", Q_NULLPTR));
        stepping->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Stepping:</p></body></html>", Q_NULLPTR));
        Techonology->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Technology:</p></body></html>", Q_NULLPTR));
        Extfamily->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">EXT.Family:</p></body></html>", Q_NULLPTR));
        extmodel->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Ext.Model:</p></body></html>", Q_NULLPTR));
        revision->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Revision:</p></body></html>", Q_NULLPTR));
        CPUNAME->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">CPU Name:</p></body></html>", Q_NULLPTR));
        cpuiconlabel->setText(QApplication::translate("MainWindow", "Image", Q_NULLPTR));
        CPUNAME_7->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">CPU Name:</p></body></html>", Q_NULLPTR));
        CodeName->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Code Name:</p></body></html>", Q_NULLPTR));
        clockgroupBox->setTitle(QApplication::translate("MainWindow", "Clock", Q_NULLPTR));
        CurrentClock->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Current Clock:</p></body></html>", Q_NULLPTR));
        busspeed->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Bus Speed:</p></body></html>", Q_NULLPTR));
        CPUNAME_16->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">CPU Name:</p></body></html>", Q_NULLPTR));
        CPUNAME_17->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">CPU Name:</p></body></html>", Q_NULLPTR));
        cachegroupBox->setTitle(QApplication::translate("MainWindow", "Cache", Q_NULLPTR));
        L1inst->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">L1.Inst:</p></body></html>", Q_NULLPTR));
        l1data->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">L1.Data:</p></body></html>", Q_NULLPTR));
        l2data->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">L2:</p></body></html>", Q_NULLPTR));
        l3data->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">L3:</p></body></html>", Q_NULLPTR));
        memoryinfogroupBox->setTitle(QApplication::translate("MainWindow", "Memory Information", Q_NULLPTR));
        memtype->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Type:</p></body></html>", Q_NULLPTR));
        memModelSize->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Model Size:</p></body></html>", Q_NULLPTR));
        memmaxbrandwidth->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">MaxBrandwidth:</p></body></html>", Q_NULLPTR));
        memmanufacturer->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Manufacturer:</p></body></html>", Q_NULLPTR));
        mempartnumber->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Part Number:</p></body></html>", Q_NULLPTR));
        memserialnumber->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Serial Number:</p></body></html>", Q_NULLPTR));
        memCorrection->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Correction:</p></body></html>", Q_NULLPTR));
        memRegistered->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Registered:</p></body></html>", Q_NULLPTR));
        memRanks->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Ranks:</p></body></html>", Q_NULLPTR));
        memspdext->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">SPD.Ext:</p></body></html>", Q_NULLPTR));
        memweekyear->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Week/Year:</p></body></html>", Q_NULLPTR));
        timingtablegroupBox->setTitle(QApplication::translate("MainWindow", "Timing Table", Q_NULLPTR));
        ModelSize_2->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Model Size:</p></body></html>", Q_NULLPTR));
        maxbrandwidth_3->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">MaxBrandwidth:</p></body></html>", Q_NULLPTR));
        memmanufacturer_3->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Manufacturer:</p></body></html>", Q_NULLPTR));
        mempartnumber_3->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Part Number:</p></body></html>", Q_NULLPTR));
        memserialnumber_3->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Serial Number:</p></body></html>", Q_NULLPTR));
        Correction_2->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Correction:</p></body></html>", Q_NULLPTR));
        Registered_2->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Registered:</p></body></html>", Q_NULLPTR));
        Ranks_2->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Ranks:</p></body></html>", Q_NULLPTR));
        spdext_2->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">SPD.Ext:</p></body></html>", Q_NULLPTR));
        weekyear_2->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Week/Year:</p></body></html>", Q_NULLPTR));
        gpuname->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Name:</p></body></html>", Q_NULLPTR));
        gpubiosversion->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">BIOS Version:</p></body></html>", Q_NULLPTR));
        gpucurrentclock->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Current Clock:</p></body></html>", Q_NULLPTR));
        gpuavailablememory->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Available Memory:</p></body></html>", Q_NULLPTR));
        gpuvirtualmemory->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Used Memory:</p></body></html>", Q_NULLPTR));
        gputemperature->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Temperature:</p></body></html>", Q_NULLPTR));
        gpufans->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Fans:</p></body></html>", Q_NULLPTR));
        gpumemoryclock->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Memory Clock:</p></body></html>", Q_NULLPTR));
        gpuphysicalmemory->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Physical Memory:</p></body></html>", Q_NULLPTR));
        gpusharedsystemmemory->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Shared System Memory:</p></body></html>", Q_NULLPTR));
        disktemperaturelabel->setText(QString());
        disknamelabel->setText(QString());
        diskgroupBox->setTitle(QString());
        diskfirename->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Firmware:</p></body></html>", Q_NULLPTR));
        diskserialnum->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Serial Number:</p></body></html>", Q_NULLPTR));
        diskinterface->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Interface:</p></body></html>", Q_NULLPTR));
        disktransfermode->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Transfer Mode:</p></body></html>", Q_NULLPTR));
        diskpoweroncount->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Power On Count:</p></body></html>", Q_NULLPTR));
        diskpoweronhours->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Power On Hours:</p></body></html>", Q_NULLPTR));
        diskStandard->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Standarrd:</p></body></html>", Q_NULLPTR));
        disksize->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Size:</p></body></html>", Q_NULLPTR));
        rwIobutton->setText(QApplication::translate("MainWindow", "R/W IO", Q_NULLPTR));
        rwmemorybutton->setText(QApplication::translate("MainWindow", "R/W Memory", Q_NULLPTR));
        rwmsrbutton->setText(QApplication::translate("MainWindow", "R/W MSR", Q_NULLPTR));
        PCIbutton->setText(QApplication::translate("MainWindow", "PCI", Q_NULLPTR));
        iobasepushButton->setText(QApplication::translate("MainWindow", "IO Base", Q_NULLPTR));
        memorybasepushButton->setText(QApplication::translate("MainWindow", "Memory Base", Q_NULLPTR));
        msraddrlabel->setText(QApplication::translate("MainWindow", "MSR Addr:", Q_NULLPTR));
        msreadpushButton->setText(QApplication::translate("MainWindow", "RDMSR", Q_NULLPTR));
        msrdatalabel->setText(QApplication::translate("MainWindow", "MSR Data:", Q_NULLPTR));
        mstwritepushButton->setText(QApplication::translate("MainWindow", "WDMSR", Q_NULLPTR));
        msrcorenumlabel->setText(QApplication::translate("MainWindow", "Core Num:", Q_NULLPTR));
        MSRcorenumlineEdit->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        msrcorenumpushButton->setText(QApplication::translate("MainWindow", "WDMSR", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
