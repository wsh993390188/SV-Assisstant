#ifndef USBTREEWIDGET_H
#define USBTREEWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QMap>
#include <algorithm>
#include <map>
#include <memory>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <string>
#include <QTreeWidget>
#include "UsbHub.h"

class usbtreewidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit usbtreewidget(QWidget *parent = nullptr);

signals:
    void sendUsbInfo(QString msg);
public slots:

private:
    void CreateUsbTree();
    void addItem(QStringList item, QString parentKey, QString theKey, QIcon icon);
    std::unique_ptr<_EnumUsbHub> controller;
    QMap<QString, QTreeWidgetItem*> itemMap_;
    void SetTextBrowser(QTreeWidgetItem* item1, int columns);
    void SaveUsbInfo(
            const QString thekey,
            const USBDEVICEINFOTYPE UsbType,
            const USBROOTHUBINFO* const rootinfo = nullptr,
            const USBHOSTCONTROLLERINFO * const rootctrlinfo = nullptr,
            const USBEXTERNALHUBINFO* const Hubinfo = nullptr,
            const USBDEVICEINFO* const deviceinfo = nullptr);
    void SaveRootInfo(
            const QString thekey,
            const USBROOTHUBINFO* const rootinfo = nullptr,
            const USBHOSTCONTROLLERINFO * const rootctrlinfo = nullptr);
    void SaveHubInfo(
            const QString thekey,
            const USBEXTERNALHUBINFO* const Hubinfo = nullptr);
    void SaveDeviceInfo(
            const QString thekey,
            const USBDEVICEINFO* const deviceinfo = nullptr);
};

#endif // USBTREEWIDGET_H
