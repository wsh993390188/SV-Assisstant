#include "sv_master_mainwindows.h"
#include "ui_sv_master_mainwindows.h"

SV_Master_Mainwindows::SV_Master_Mainwindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SV_Master_Mainwindows)
{
    ui->setupUi(this);
}

SV_Master_Mainwindows::~SV_Master_Mainwindows()
{
    delete ui;
}
