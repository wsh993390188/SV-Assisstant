#include "mainwindow.h"
#include "appinit.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppInit::Instance()->start();
    MainWindow w;
    w.show();


    return a.exec();
}
