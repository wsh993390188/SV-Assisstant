#include "sv_assistant_basic_window.h"
#include "appinit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SV_ASSISTANT_BASIC_WINDOW w;
    w.show();
    AppInit::Instance()->start();
    return a.exec();
}
