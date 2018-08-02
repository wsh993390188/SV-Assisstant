#include "svassistmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SVAssistMainWindow w;
    w.show();

    return a.exec();
}
