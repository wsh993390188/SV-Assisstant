#-------------------------------------------------
#
# Project created by QtCreator 2018-08-02T14:02:53
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SV-Assistant
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
    appinit.cpp \
    AudioWidget.cpp \
    closebutton.cpp \
    CPUHardwareWidget.cpp \
    curvechart.cpp \
    DiskWidget.cpp \
    GPUWidget.cpp \
    HardwareMasterTab.cpp \
    HardwareOthersWidget.cpp \
    hardwaretab.cpp \
    homewidget.cpp \
    IOButton.cpp \
    IODataView.cpp \
    main.cpp \
    mainctrlbutton.cpp \
    MemoryButton.cpp \
    MemoryDataView.cpp \
    MemoryWidget.cpp \
    MonitorWidget.cpp \
    MotherBroadWidget.cpp \
    NetworkWidget.cpp	\
	svassistmainwindow.cpp

HEADERS += \
    appinit.h \
    AudioWidget.h \
    closebutton.h \
    CPUHardwareWidget.h \
    curvechart.h \
    DiskWidget.h \
    GPUWidget.h \
    HardwareMasterTab.h \
    HardwareOthersWidget.h \
    hardwaretab.h \
    homewidget.h \
    IOButton.h \
    IODataView.h \
    mainctrlbutton.h \
    MemoryButton.h \
    MemoryDataView.h \
    MemoryWidget.h \
    MonitorWidget.h \
    MotherBroadWidget.h \
    NetworkWidget.h	\
	svassistmainwindow.h

FORMS += \
        svassistmainwindow.ui

RESOURCES += \
    Resource/resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lsv-assistant
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lsv-assistantd

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/libsv-assistant.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/libsv-assistantd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/sv-assistant.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/sv-assistantd.lib
