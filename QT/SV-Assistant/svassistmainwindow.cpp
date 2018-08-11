#include "svassistmainwindow.h"
#include "ui_svassistmainwindow.h"

SVAssistMainWindow::SVAssistMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SVAssistMainWindow)
{
    ui->setupUi(this);
    this->InitWindow();
}

void SVAssistMainWindow::InitWindow()
{
    this->setProperty("canMove", true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    connect(ui->minButton, &QToolButton::clicked, this, [&](bool state){ if(!state) showMinimized();});
    connect(ui->HometoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(0); });
    connect(ui->HardwareViewtoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(1); });
    connect(ui->HardwareManagetoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(2); });
    connect(ui->SoftwareViewtoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(3); });
    connect(ui->SoftwareManagetoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(4); });
    connect(ui->GameTesttoolButton, &QToolButton::clicked, this, [&]{ ui->HomestackedWidget->setCurrentIndex(5); });
	connect(ui->HardwareTabwidget, &HardwareTab::SendHardwareTabIndex, this, [&](int Index) {ui->HardwareInfomationstackedWidget->setCurrentIndex(Index); });
}

SVAssistMainWindow::~SVAssistMainWindow()
{
    delete ui;
}
