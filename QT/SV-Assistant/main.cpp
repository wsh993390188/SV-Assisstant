#include "svassistmainwindow.h"
#include "appinit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppInit::Instance()->start();
    SVAssistMainWindow w;
    w.show();
    return a.exec();
}
