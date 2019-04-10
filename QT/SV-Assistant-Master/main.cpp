#include "sv_master_mainwindows.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SV_Master_Mainwindows w;
    w.show();

    return a.exec();
}
