/********************************************************************************
** Form generated from reading UI file 'sv_assistant_basic_window.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SV_ASSISTANT_BASIC_WINDOW_H
#define UI_SV_ASSISTANT_BASIC_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <cpuhardwarewidget.h>
#include <gpuwidget.h>
#include <hardwaretab.h>
#include <memorywidget.h>
#include <motherbroadwidget.h>
#include "NetworkWidget.h"
#include "NetworkWidget.h"
#include "audiowidget.h"
#include "closebutton.h"
#include "diskwidget.h"
#include "homewidget.h"
#include "mainctrlbutton.h"
#include "monitorwidget.h"
#include "usbtestbrowser.h"
#include "usbtreewidget.h"

QT_BEGIN_NAMESPACE

class Ui_SVAssistMainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QWidget *Titlewidget;
    QHBoxLayout *horizontalLayout;
    QToolButton *HometoolButton;
    QToolButton *HardwareViewtoolButton;
    QToolButton *SoftwareViewtoolButton;
    QToolButton *GameTesttoolButton;
    QSpacerItem *menuhorizontalSpacer;
    QGridLayout *menugridLayout;
    closebutton *closeButton;
    mainctrlbutton *minButton;
    QSpacerItem *menuverticalSpacer;
    QToolButton *setButton;
    mainctrlbutton *reportButton;
    QHBoxLayout *HomehorizontalLayout;
    QStackedWidget *HomestackedWidget;
    QWidget *Homepage;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    Homewidget *TitleStackedWidget;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QTextBrowser *textBrowser;
    QTextBrowser *textBrowser_2;
    QLabel *Versionlabel;
    QWidget *Hardwarepage;
    QHBoxLayout *HardwarehorizontalLayout;
    HardwareTab *HardwareTabwidget;
    QStackedWidget *HardwareInfomationstackedWidget;
    CPUHardwareWidget *CPUpage;
    MotherBroadWidget *MotherBroadpage;
    MemoryWidget *Memorypage;
    GPUWidget *GPUpage;
    DiskWidget *Diskpage;
    MonitorWidget *Monitorpage;
    AudioWidget *Audiopage;
    QWidget *Networkpage;
    QHBoxLayout *horizontalLayout_3;
    QSplitter *Networksplitter;
    NetworkListWidget *NetworklistWidget;
    NetworkTableWidget *NetworktextBrowser;
    QWidget *USBhardwarepage;
    QHBoxLayout *horizontalLayout_2;
    QSplitter *USBsplitter;
    usbtreewidget *usbtreeWidget;
    usbtestbrowser *usbtextBrowser;
    QWidget *Softwarepage;
    QLabel *label_2;
    QWidget *GameTestpage;
    QLabel *label_4;

    void setupUi(QMainWindow *SVAssistMainWindow)
    {
        if (SVAssistMainWindow->objectName().isEmpty())
            SVAssistMainWindow->setObjectName(QStringLiteral("SVAssistMainWindow"));
        SVAssistMainWindow->resize(1088, 702);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SVAssistMainWindow->sizePolicy().hasHeightForWidth());
        SVAssistMainWindow->setSizePolicy(sizePolicy);
        SVAssistMainWindow->setMinimumSize(QSize(900, 600));
        SVAssistMainWindow->setMaximumSize(QSize(1350, 900));
        QIcon icon;
        icon.addFile(QStringLiteral(":/menu/Title/ZX.png"), QSize(), QIcon::Normal, QIcon::Off);
        SVAssistMainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(SVAssistMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setMinimumSize(QSize(900, 600));
        centralWidget->setMaximumSize(QSize(1350, 900));
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        Titlewidget = new QWidget(centralWidget);
        Titlewidget->setObjectName(QStringLiteral("Titlewidget"));
        Titlewidget->setMinimumSize(QSize(900, 100));
        Titlewidget->setMaximumSize(QSize(1350, 150));
        Titlewidget->setStyleSheet(QStringLiteral("QWidget#Titlewidget{border-image: url(:/menu/Title/panel.png); }"));
        horizontalLayout = new QHBoxLayout(Titlewidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 6, 4, -1);
        HometoolButton = new QToolButton(Titlewidget);
        HometoolButton->setObjectName(QStringLiteral("HometoolButton"));
        HometoolButton->setMinimumSize(QSize(80, 80));
        HometoolButton->setMaximumSize(QSize(80, 80));
        HometoolButton->setStyleSheet(QString::fromUtf8("border:none;\n"
"font: 10pt \"\345\256\213\344\275\223\";\n"
"color: rgb(255, 255, 255);"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/menu/Title/computer.png"), QSize(), QIcon::Normal, QIcon::Off);
        HometoolButton->setIcon(icon1);
        HometoolButton->setIconSize(QSize(54, 54));
        HometoolButton->setPopupMode(QToolButton::DelayedPopup);
        HometoolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        HometoolButton->setArrowType(Qt::NoArrow);

        horizontalLayout->addWidget(HometoolButton);

        HardwareViewtoolButton = new QToolButton(Titlewidget);
        HardwareViewtoolButton->setObjectName(QStringLiteral("HardwareViewtoolButton"));
        HardwareViewtoolButton->setMinimumSize(QSize(80, 80));
        HardwareViewtoolButton->setMaximumSize(QSize(80, 80));
        HardwareViewtoolButton->setStyleSheet(QString::fromUtf8("border:none;\n"
"font: 10pt \"\345\256\213\344\275\223\";\n"
"color: rgb(255, 255, 255);"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/menu/Title/hardware.png"), QSize(), QIcon::Normal, QIcon::Off);
        HardwareViewtoolButton->setIcon(icon2);
        HardwareViewtoolButton->setIconSize(QSize(54, 54));
        HardwareViewtoolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout->addWidget(HardwareViewtoolButton);

        SoftwareViewtoolButton = new QToolButton(Titlewidget);
        SoftwareViewtoolButton->setObjectName(QStringLiteral("SoftwareViewtoolButton"));
        SoftwareViewtoolButton->setMinimumSize(QSize(80, 80));
        SoftwareViewtoolButton->setMaximumSize(QSize(80, 80));
        SoftwareViewtoolButton->setStyleSheet(QString::fromUtf8("border:none;\n"
"font: 10pt \"\345\256\213\344\275\223\";\n"
"color: rgb(255, 255, 255);"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/menu/Title/software.png"), QSize(), QIcon::Normal, QIcon::Off);
        SoftwareViewtoolButton->setIcon(icon3);
        SoftwareViewtoolButton->setIconSize(QSize(54, 54));
        SoftwareViewtoolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout->addWidget(SoftwareViewtoolButton);

        GameTesttoolButton = new QToolButton(Titlewidget);
        GameTesttoolButton->setObjectName(QStringLiteral("GameTesttoolButton"));
        GameTesttoolButton->setMinimumSize(QSize(80, 80));
        GameTesttoolButton->setMaximumSize(QSize(80, 80));
        GameTesttoolButton->setStyleSheet(QString::fromUtf8("border:none;\n"
"font: 10pt \"\345\256\213\344\275\223\";\n"
"color: rgb(255, 255, 255);"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/menu/Title/gamebox.png"), QSize(), QIcon::Normal, QIcon::Off);
        GameTesttoolButton->setIcon(icon4);
        GameTesttoolButton->setIconSize(QSize(54, 54));
        GameTesttoolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        horizontalLayout->addWidget(GameTesttoolButton);

        menuhorizontalSpacer = new QSpacerItem(500, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(menuhorizontalSpacer);

        menugridLayout = new QGridLayout();
        menugridLayout->setSpacing(0);
        menugridLayout->setObjectName(QStringLiteral("menugridLayout"));
        closeButton = new closebutton(Titlewidget);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        closeButton->setMinimumSize(QSize(48, 18));
        closeButton->setMaximumSize(QSize(48, 18));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/menu/menu/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon5);
        closeButton->setIconSize(QSize(16, 16));

        menugridLayout->addWidget(closeButton, 0, 3, 1, 1);

        minButton = new mainctrlbutton(Titlewidget);
        minButton->setObjectName(QStringLiteral("minButton"));
        minButton->setMinimumSize(QSize(33, 18));
        minButton->setMaximumSize(QSize(33, 18));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/menu/menu/minimize.png"), QSize(), QIcon::Normal, QIcon::Off);
        minButton->setIcon(icon6);
        minButton->setIconSize(QSize(16, 16));

        menugridLayout->addWidget(minButton, 0, 2, 1, 1);

        menuverticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        menugridLayout->addItem(menuverticalSpacer, 1, 2, 1, 1);

        setButton = new QToolButton(Titlewidget);
        setButton->setObjectName(QStringLiteral("setButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(33);
        sizePolicy2.setVerticalStretch(18);
        sizePolicy2.setHeightForWidth(setButton->sizePolicy().hasHeightForWidth());
        setButton->setSizePolicy(sizePolicy2);
        setButton->setMinimumSize(QSize(33, 18));
        setButton->setMaximumSize(QSize(33, 18));
        setButton->setStyleSheet(QStringLiteral(""));
        setButton->setArrowType(Qt::DownArrow);

        menugridLayout->addWidget(setButton, 0, 1, 1, 1);

        reportButton = new mainctrlbutton(Titlewidget);
        reportButton->setObjectName(QStringLiteral("reportButton"));
        reportButton->setMinimumSize(QSize(33, 18));
        reportButton->setMaximumSize(QSize(33, 18));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/menu/menu/report.png"), QSize(), QIcon::Normal, QIcon::Off);
        reportButton->setIcon(icon7);
        reportButton->setIconSize(QSize(16, 16));

        menugridLayout->addWidget(reportButton, 0, 0, 1, 1);


        horizontalLayout->addLayout(menugridLayout);


        verticalLayout_3->addWidget(Titlewidget);

        HomehorizontalLayout = new QHBoxLayout();
        HomehorizontalLayout->setSpacing(0);
        HomehorizontalLayout->setObjectName(QStringLiteral("HomehorizontalLayout"));
        HomehorizontalLayout->setContentsMargins(0, 0, -1, -1);
        HomestackedWidget = new QStackedWidget(centralWidget);
        HomestackedWidget->setObjectName(QStringLiteral("HomestackedWidget"));
        HomestackedWidget->setMinimumSize(QSize(900, 500));
        HomestackedWidget->setMaximumSize(QSize(1350, 750));
        HomestackedWidget->setStyleSheet(QStringLiteral(""));
        Homepage = new QWidget();
        Homepage->setObjectName(QStringLiteral("Homepage"));
        Homepage->setMinimumSize(QSize(900, 500));
        Homepage->setMaximumSize(QSize(1350, 750));
        verticalLayout_2 = new QVBoxLayout(Homepage);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        TitleStackedWidget = new Homewidget(Homepage);
        TitleStackedWidget->setObjectName(QStringLiteral("TitleStackedWidget"));
        TitleStackedWidget->setMinimumSize(QSize(650, 472));
        TitleStackedWidget->setMaximumSize(QSize(975, 708));

        gridLayout->addWidget(TitleStackedWidget, 1, 0, 1, 1);

        groupBox = new QGroupBox(Homepage);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textBrowser = new QTextBrowser(groupBox);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        textBrowser_2 = new QTextBrowser(groupBox);
        textBrowser_2->setObjectName(QStringLiteral("textBrowser_2"));

        verticalLayout->addWidget(textBrowser_2);


        gridLayout->addWidget(groupBox, 1, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        Versionlabel = new QLabel(Homepage);
        Versionlabel->setObjectName(QStringLiteral("Versionlabel"));
        Versionlabel->setMinimumSize(QSize(900, 28));
        Versionlabel->setMaximumSize(QSize(1350, 42));
        Versionlabel->setStyleSheet(QLatin1String("QLabel{color:#000000;font-family: \\\"consolas\\\"; font-size:10px; font-weight:bold;background-color: rgb(255, 255, 255);}\n"
""));
        Versionlabel->setMargin(8);

        verticalLayout_2->addWidget(Versionlabel);

        HomestackedWidget->addWidget(Homepage);
        Hardwarepage = new QWidget();
        Hardwarepage->setObjectName(QStringLiteral("Hardwarepage"));
        Hardwarepage->setMinimumSize(QSize(900, 500));
        Hardwarepage->setMaximumSize(QSize(1350, 750));
        HardwarehorizontalLayout = new QHBoxLayout(Hardwarepage);
        HardwarehorizontalLayout->setSpacing(0);
        HardwarehorizontalLayout->setContentsMargins(11, 11, 11, 11);
        HardwarehorizontalLayout->setObjectName(QStringLiteral("HardwarehorizontalLayout"));
        HardwarehorizontalLayout->setContentsMargins(0, 0, 0, 0);
        HardwareTabwidget = new HardwareTab(Hardwarepage);
        HardwareTabwidget->setObjectName(QStringLiteral("HardwareTabwidget"));
        HardwareTabwidget->setMinimumSize(QSize(100, 500));
        HardwareTabwidget->setMaximumSize(QSize(150, 750));

        HardwarehorizontalLayout->addWidget(HardwareTabwidget);

        HardwareInfomationstackedWidget = new QStackedWidget(Hardwarepage);
        HardwareInfomationstackedWidget->setObjectName(QStringLiteral("HardwareInfomationstackedWidget"));
        HardwareInfomationstackedWidget->setMinimumSize(QSize(800, 500));
        HardwareInfomationstackedWidget->setMaximumSize(QSize(1200, 750));
        HardwareInfomationstackedWidget->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        CPUpage = new CPUHardwareWidget();
        CPUpage->setObjectName(QStringLiteral("CPUpage"));
        HardwareInfomationstackedWidget->addWidget(CPUpage);
        MotherBroadpage = new MotherBroadWidget();
        MotherBroadpage->setObjectName(QStringLiteral("MotherBroadpage"));
        HardwareInfomationstackedWidget->addWidget(MotherBroadpage);
        Memorypage = new MemoryWidget();
        Memorypage->setObjectName(QStringLiteral("Memorypage"));
        HardwareInfomationstackedWidget->addWidget(Memorypage);
        GPUpage = new GPUWidget();
        GPUpage->setObjectName(QStringLiteral("GPUpage"));
        HardwareInfomationstackedWidget->addWidget(GPUpage);
        Diskpage = new DiskWidget();
        Diskpage->setObjectName(QStringLiteral("Diskpage"));
        HardwareInfomationstackedWidget->addWidget(Diskpage);
        Monitorpage = new MonitorWidget();
        Monitorpage->setObjectName(QStringLiteral("Monitorpage"));
        HardwareInfomationstackedWidget->addWidget(Monitorpage);
        Audiopage = new AudioWidget();
        Audiopage->setObjectName(QStringLiteral("Audiopage"));
        HardwareInfomationstackedWidget->addWidget(Audiopage);
        Networkpage = new QWidget();
        Networkpage->setObjectName(QStringLiteral("Networkpage"));
        horizontalLayout_3 = new QHBoxLayout(Networkpage);
        horizontalLayout_3->setSpacing(4);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(4, 4, 4, 4);
        Networksplitter = new QSplitter(Networkpage);
        Networksplitter->setObjectName(QStringLiteral("Networksplitter"));
        Networksplitter->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        Networksplitter->setOrientation(Qt::Vertical);
        Networksplitter->setHandleWidth(7);
        NetworklistWidget = new NetworkListWidget(Networksplitter);
        NetworklistWidget->setObjectName(QStringLiteral("NetworklistWidget"));
        NetworklistWidget->setMinimumSize(QSize(0, 150));
        NetworklistWidget->setMaximumSize(QSize(16777215, 225));
        Networksplitter->addWidget(NetworklistWidget);
        NetworktextBrowser = new NetworkTableWidget(Networksplitter);
        NetworktextBrowser->setObjectName(QStringLiteral("NetworktextBrowser"));
        NetworktextBrowser->setMinimumSize(QSize(0, 350));
        NetworktextBrowser->setMaximumSize(QSize(16777215, 525));
        Networksplitter->addWidget(NetworktextBrowser);

        horizontalLayout_3->addWidget(Networksplitter);

        HardwareInfomationstackedWidget->addWidget(Networkpage);
        USBhardwarepage = new QWidget();
        USBhardwarepage->setObjectName(QStringLiteral("USBhardwarepage"));
        horizontalLayout_2 = new QHBoxLayout(USBhardwarepage);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        USBsplitter = new QSplitter(USBhardwarepage);
        USBsplitter->setObjectName(QStringLiteral("USBsplitter"));
        USBsplitter->setOrientation(Qt::Horizontal);
        usbtreeWidget = new usbtreewidget(USBsplitter);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        usbtreeWidget->setHeaderItem(__qtreewidgetitem);
        usbtreeWidget->setObjectName(QStringLiteral("usbtreeWidget"));
        usbtreeWidget->setMinimumSize(QSize(0, 0));
        usbtreeWidget->setMaximumSize(QSize(16777215, 16777215));
        USBsplitter->addWidget(usbtreeWidget);
        usbtextBrowser = new usbtestbrowser(USBsplitter);
        usbtextBrowser->setObjectName(QStringLiteral("usbtextBrowser"));
        usbtextBrowser->setMinimumSize(QSize(0, 0));
        usbtextBrowser->setMaximumSize(QSize(16777215, 16777215));
        USBsplitter->addWidget(usbtextBrowser);

        horizontalLayout_2->addWidget(USBsplitter);

        HardwareInfomationstackedWidget->addWidget(USBhardwarepage);

        HardwarehorizontalLayout->addWidget(HardwareInfomationstackedWidget);

        HomestackedWidget->addWidget(Hardwarepage);
        Softwarepage = new QWidget();
        Softwarepage->setObjectName(QStringLiteral("Softwarepage"));
        Softwarepage->setMaximumSize(QSize(1350, 750));
        label_2 = new QLabel(Softwarepage);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(320, 190, 171, 71));
        HomestackedWidget->addWidget(Softwarepage);
        GameTestpage = new QWidget();
        GameTestpage->setObjectName(QStringLiteral("GameTestpage"));
        GameTestpage->setMaximumSize(QSize(1350, 750));
        label_4 = new QLabel(GameTestpage);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(300, 200, 171, 71));
        HomestackedWidget->addWidget(GameTestpage);

        HomehorizontalLayout->addWidget(HomestackedWidget);


        verticalLayout_3->addLayout(HomehorizontalLayout);

        SVAssistMainWindow->setCentralWidget(centralWidget);

        retranslateUi(SVAssistMainWindow);

        QMetaObject::connectSlotsByName(SVAssistMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SVAssistMainWindow)
    {
        SVAssistMainWindow->setWindowTitle(QApplication::translate("SVAssistMainWindow", "SVAssistMainWindow", nullptr));
        HometoolButton->setText(QApplication::translate("SVAssistMainWindow", "\345\237\272\346\234\254\344\277\241\346\201\257", nullptr));
        HardwareViewtoolButton->setText(QApplication::translate("SVAssistMainWindow", "\347\241\254\344\273\266\346\243\200\346\265\213", nullptr));
        SoftwareViewtoolButton->setText(QApplication::translate("SVAssistMainWindow", "\350\275\257\344\273\266\346\243\200\346\265\213", nullptr));
        GameTesttoolButton->setText(QApplication::translate("SVAssistMainWindow", "\346\200\247\350\203\275\346\243\200\346\265\213", nullptr));
        closeButton->setText(QString());
        minButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        setButton->setToolTip(QApplication::translate("SVAssistMainWindow", "\346\234\200\345\260\217\345\214\226", nullptr));
#endif // QT_NO_TOOLTIP
        setButton->setText(QString());
        reportButton->setText(QString());
        groupBox->setTitle(QString());
        textBrowser->setHtml(QApplication::translate("SVAssistMainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\346\234\254\350\275\257\344\273\266\345\237\272\344\272\216\351\251\261\345\212\250\350\216\267\345\217\226\350\216\267\345\217\226\347\263\273\347\273\237\347\241\254\344\273\266\344\277\241\346\201\257\357\274\214\346\265\213\350\257\225\347\211\210\346\234\254\357\274\214\344\275\277\347\224\250\345\211\215\350\257\267\345\201\232\345\245\275\345\244\207\344\273\275</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-bl"
                        "ock-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\344\275\234\350\200\205:f0rest 2018.8 \344\272\216 \345\214\227\344\272\254</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\351\241\271\347\233\256\346\272\220\347\240\201 https://github.com/wsh993390188/SV-Assisstant</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type"
                        ":empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p></body></html>", nullptr));
        textBrowser_2->setHtml(QApplication::translate("SVAssistMainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\346\265\213\350\257\225\347\211\210\346\234\254,\345\257\271APU\346\224\257\346\214\201\345\267\256\343\200\202</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\346\234\254\347\211\210\346\234\254\344\273\205\344\273\205\346\230\257\345\261\225\347\244\272\350\216\267\345\217\226\347\232\204\351\203\250\345\210\206\347\241\254\344\273\266\344\277\241\346\201\257,\345\217\257\344\273\245\344\270\216cpuz gpuz AI"
                        "DA64 HwInfo \351\262\201\345\244\247\345\270\210\347\255\211\350\277\233\350\241\214\344\277\241\346\201\257\345\257\271\346\257\224\343\200\202</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\344\270\252\344\272\272\345\274\200\345\217\221\357\274\214\344\277\241\346\201\257\350\216\267\345\217\226\344\270\212\351\232\276\345\205\215\346\234\211\350\257\257\345\267\256,\346\254\242\350\277\216\346\214\207\346\255\243</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">\346\234\200\344\275\263\345\210\206\350\276\250\347\216\207:1920*1080</span></p></body></html>", nullptr));
        Versionlabel->setText(QApplication::translate("SVAssistMainWindow", "Major Version 0.0.1", nullptr));
        label_2->setText(QApplication::translate("SVAssistMainWindow", "Under construction", nullptr));
        label_4->setText(QApplication::translate("SVAssistMainWindow", "Under construction", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SVAssistMainWindow: public Ui_SVAssistMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SV_ASSISTANT_BASIC_WINDOW_H
