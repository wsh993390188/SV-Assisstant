#include "svassistmainwindow.h"
#include "ui_svassistmainwindow.h"

SVAssistMainWindow::SVAssistMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SVAssistMainWindow)
{
    ui->setupUi(this);
}

SVAssistMainWindow::~SVAssistMainWindow()
{
    delete ui;
}
