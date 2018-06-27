#-------------------------------------------------
#
# Project created by QtCreator 2018-03-15T13:29:42
#
#-------------------------------------------------

QT       += core gui
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        rw_qtablewidget.cpp \
        sv_hardware_pushbutton.cpp \
        appinit.cpp \
        mainctrlbutton.cpp \
        closebutton.cpp \
        UsbController.cpp \
        UsbHub.cpp \
        UsbPort.cpp \
        usbtreewidget.cpp \
        UsbDefination.cpp \
    usbtestbrowser.cpp \
    smbiostreewidget.cpp \
    smbiostextbrowser.cpp \
    settingbutton.cpp \
    diskselectbutton.cpp \
    pcicombobox.cpp \
    pcitablewidget.cpp \
    rwbasebutton.cpp \
    edidterrwidget.cpp \
    edidtextbrowser.cpp \
    cpuusageqcharts.cpp
        appinit.cpp

HEADERS += \
        mainwindow.h \
    rw_qtablewidget.h \
    sv_hardware_pushbutton.h \
    appinit.h \
    mainctrlbutton.h \
    closebutton.h \
    UsbController.h \
    UsbHub.h \
    UsbPort.h \
    usbtreewidget.h \
    UsbDefination.h \
    usbtestbrowser.h \
    smbiostreewidget.h \
    smbiostextbrowser.h \
    settingbutton.h \
    diskselectbutton.h \
    pcicombobox.h \
    pcitablewidget.h \
    rwbasebutton.h \
    edidterrwidget.h \
    edidtextbrowser.h \
    cpuusageqcharts.h
    appinit.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    cpu.qrc \
    main.qrc \
    monitor.qrc \
    usb.qrc \
    disk.qrc \
    exeicon.qrc \
    qss.qrc

RC_FILE += exeicon.rc

win32: LIBS += -L$$PWD/Lib/ -lDriverdll

INCLUDEPATH += $$PWD/Lib
DEPENDPATH += $$PWD/Lib


win32: LIBS += -L$$PWD/Lib/ -lsv-assistant

INCLUDEPATH += $$PWD/Lib
DEPENDPATH += $$PWD/Lib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/Lib/sv-assistant.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/Lib/libsv-assistant.a
