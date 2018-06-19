#include "usbtreewidget.h"

using namespace std;

usbtreewidget::usbtreewidget(QWidget *parent) : QTreeWidget(parent), controller(new _EnumUsbHub)
{
    controller->Excute();
    this->setHeaderHidden(true);
    this->CreateUsbTree();
    connect(this, &usbtreewidget::itemClicked, this, &usbtreewidget::SetTextBrowser);
}

void usbtreewidget::CreateUsbTree()
{
    QString rootkey = tr("My Computer");
    this->addItem(QStringList(rootkey), tr(""), rootkey, QIcon(tr(":/resources/Usb/monitor.ico")));
    for(auto& i : this->controller->Usb_Tree)
    {
        int a = 0;
        QString t = QString::fromStdWString(i.UsbRootHubInfo.UsbDeviceProperties.DeviceDesc);
        QString parentKey = tr("My Computer");
        QString thekey = QString::fromStdWString(i.UsbRootHubInfo.UsbDeviceProperties.DeviceId);
        thekey = thekey.replace("\\", "_");
        qDebug() << thekey;
        this->addItem(QStringList(t), parentKey, thekey, QIcon(tr(":/resources/Usb/usb.ico")));
        this->SaveUsbInfo(thekey, i.UsbRootHubInfo.DeviceInfoType, &i.UsbRootHubInfo,&i.UsbRootControllerInfo);
        for(auto& j : i.UsbDeviceInfo.UsbHubInfo)
        {
            int b = 0;
            for(auto& p : j.UsbExternalHubInfo)
            {
                QString parenthubKey = thekey;
                //qDebug() << QString::fromStdWString(p.HubName);
                QString thehubkey = QString::fromStdWString(p.HubName).replace("\\", "_");
                this->addItem(QStringList(tr("Port ") + QString::number(a++)+ tr(" ") + QString::fromStdWString(p.UsbDeviceProperties.DeviceDesc)), parenthubKey, thehubkey, QIcon(tr(":/resources/Usb/hub.ico")));
                this->SaveUsbInfo(thehubkey, p.DeviceInfoType, nullptr, nullptr, &p);
                for(auto& l : j.UsbDeviceInfo)
                {
                    QString parentportKey = thehubkey;
                    QString theportkey = parentportKey + tr("#") + QString::number(b++);
                    if(l.ConnectionInfo.ConnectionStatus == DeviceConnected)
                    {
                        this->addItem(QStringList(tr("Port ") + QString::number(b)+ tr(" ") + QString::fromStdWString(l.UsbDeviceProperties.DeviceDesc)), parentportKey, theportkey, QIcon(tr(":/resources/Usb/usb.ico")));
                    }
                    else
                    {
                        this->addItem(QStringList(tr("Port ") + QString::number(b)), parentportKey, theportkey, QIcon(tr(":/resources/Usb/port.ico")));
                    }
                    this->SaveUsbInfo(theportkey, l.DeviceInfoType, nullptr, nullptr, nullptr, &l);

                }
            }
        }
        for(auto& k : i.UsbDeviceInfo.UsbDeviceInfo)
        {
            QString parentportKey = thekey;
            QString theportkey = parentportKey + tr("#") + QString::number(a++);
            if(k.ConnectionInfo.ConnectionStatus == DeviceConnected)
            {

                this->addItem(QStringList(tr("Port ") + QString::number(a) + tr(" ") + QString::fromStdWString(k.UsbDeviceProperties.DeviceDesc)), parentportKey, theportkey, QIcon(tr(":/resources/Usb/usb.ico")));
            }
            else
            {
                this->addItem(QStringList(tr("Port ") + QString::number(a)), parentportKey, theportkey, QIcon(tr(":/resources/Usb/port.ico")));
            }

            this->SaveUsbInfo(theportkey, k.DeviceInfoType, nullptr, nullptr, nullptr, &k);
        }

    }
}

void usbtreewidget::addItem(QStringList item, QString parentKey, QString theKey, QIcon icon)
{
    QTreeWidgetItem* temp_item;
    /// 生成到父级节点下
    QMap<QString,QTreeWidgetItem*>::iterator iter_item = itemMap_.find(parentKey);
    if (iter_item == itemMap_.end()) {
        temp_item = new QTreeWidgetItem(this, item);
    }
    else {
        temp_item = new QTreeWidgetItem(iter_item.value(), item);
    }
    /// 判断
    temp_item->setIcon(0, icon);
    if (!theKey.isEmpty()) {
        itemMap_.insert(theKey,temp_item);
    }
}

void usbtreewidget::SetTextBrowser(QTreeWidgetItem *item1, int columns)
{
    Q_UNUSED(columns);
    QString value = tr("");
    for(QMap<QString, QTreeWidgetItem*>::iterator it = this->itemMap_.begin(); it != this->itemMap_.end(); it++ )
    {
      if(it.value() == item1)
      {
          value = it.key();
          break;
      }
    }
    emit sendUsbInfo(value);
}

void usbtreewidget::SaveUsbInfo(
        const QString thekey,
        const USBDEVICEINFOTYPE UsbType,
        const USBROOTHUBINFO* const rootinfo,
        const USBHOSTCONTROLLERINFO * const rootctrlinfo,
        const USBEXTERNALHUBINFO* const Hubinfo,
        const USBDEVICEINFO* const deviceinfo
        )
{
    QString currentdir = qApp->applicationDirPath();
    currentdir.append(tr("\\HardWare\\Usb"));

    QDir dir(currentdir);
    if(!dir.exists())
    {
        dir.mkpath(currentdir);//创建多级目录
    }

    if(UsbType == RootHubInfo && rootinfo && rootctrlinfo)
    {
        this->SaveRootInfo(currentdir + tr("\\") + thekey, rootinfo, rootctrlinfo);
    }
    else if(UsbType == ExternalHubInfo && Hubinfo)
    {
        this->SaveHubInfo(currentdir + tr("\\") + thekey, Hubinfo);
    }
    else if(DEVICEInfo == UsbType && deviceinfo)
    {
        this->SaveDeviceInfo(currentdir + tr("\\") + thekey, deviceinfo);
    }

}

void usbtreewidget::SaveRootInfo(const QString thekey, const USBROOTHUBINFO * const rootinfo, const USBHOSTCONTROLLERINFO * const rootctrlinfo)
{
   QFile file(thekey);
//   if(file.exists()){
//       qDebug()<<"文件已存在";
//   }else{
//       qDebug()<<"文件不存在";
//       file.open( QIODevice::ReadWrite | QIODevice::Text );
//       file.close();
//   }

   if(!file.open(QFile::WriteOnly))
   {
       qDebug() << "Can not open " << thekey;
   }
   else
   {
       QTextStream in(&file);
       in << QString::fromStdWString(rootinfo->UsbDeviceProperties.DeviceDesc) << endl;
       in << endl;
       in << tr("DriverKey: ") << QString::fromStdWString(rootctrlinfo->DriverKey)<< endl;
       in << tr("VendorID: ") << QString::number(rootctrlinfo->VendorID, 16).toUpper()<< endl;
       in << tr("DeviceID: ") << QString::number(rootctrlinfo->DeviceID, 16).toUpper()<< endl;
       in << tr("SubSysID: ") << QString::number(rootctrlinfo->SubSysID, 16).toUpper()<< endl;
       in << tr("Revision: ") << QString::number(rootctrlinfo->Revision, 16).toUpper()<< endl;
       in << tr("Bus.Device.Function (in hex): ") << QString::number(rootctrlinfo->UsbControllerBDF.BusNumber, 16).toUpper() \
          << tr(".") <<  QString::number(rootctrlinfo->UsbControllerBDF.BusDevice, 16).toUpper() \
          << tr(".") <<  QString::number(rootctrlinfo->UsbControllerBDF.BusFunction, 16).toUpper()<< endl;
       in << tr("Host Controller Power State Mappings")<< endl;
       in << tr("System State       Host Controller     Root Hub        USB wakeup      Powered")<< endl;
       in << tr("")<< endl;
       file.flush();
       file.close();
   }
}

void usbtreewidget::SaveHubInfo(const QString thekey, const USBEXTERNALHUBINFO * const Hubinfo)
{
    QFile file(thekey);
    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << "Can not open " << thekey;
    }
    {
        QTextStream in(&file);
        in << tr("Is Port User Connectable:         ") << (Hubinfo->PortConnectorProps.UsbPortProperties.PortIsUserConnectable ? tr("yes") : tr("no")) << endl;
        in << tr("Is Port Debug Capable:            ") << (Hubinfo->PortConnectorProps.UsbPortProperties.PortIsDebugCapable ? tr("yes") : tr("no")) << endl;
        in << tr("Is Port TypeC:                    ") << (Hubinfo->PortConnectorProps.UsbPortProperties.PortConnectorIsTypeC ? tr("yes") : tr("no")) << endl;
        in << tr("Companion Port Number:            ") << QString::number(Hubinfo->PortConnectorProps.CompanionPortNumber) << endl;
        in << tr("Companion Hub Symbolic Link Name: ") << QString::fromStdWString(Hubinfo->PortConnectorProps.CompanionHubSymbolicLinkName) << endl;
        in << tr("Protocols Supported:              ") << endl;
        in << tr(" USB 1.1:                         ") << (Hubinfo->ConnectionInfoV2.SupportedUsbProtocols.Usb110 ? tr("yes") : tr("no")) << endl;
        in << tr(" USB 2.0:                         ") << (Hubinfo->ConnectionInfoV2.SupportedUsbProtocols.Usb200 ? tr("yes") : tr("no")) << endl;
        in << tr(" USB 3.0:                         ") << (Hubinfo->ConnectionInfoV2.SupportedUsbProtocols.Usb300 ? tr("yes") : tr("no")) << endl;
        in << endl;
        in << tr("Hub Power:                        ") << endl;

        if (Hubinfo->ConfigDesc.bmAttributes & USB_CONFIG_BUS_POWERED)
        {
            in << tr("Bus Powered") << endl;
        }
        else if (Hubinfo->ConfigDesc.bmAttributes & USB_CONFIG_SELF_POWERED)
        {
            in << tr("Self Powered") << endl;
        }
        else if (Hubinfo->ConfigDesc.bmAttributes & USB_CONFIG_REMOTE_WAKEUP)
        {
            in << tr("Remote Wakeup") << endl;
        }
        else
        {
            in << tr("WARNING: bmAttributes is using reserved space") << endl;
        }
        in << tr("Hub Type:                        ") << endl;
        switch (Hubinfo->HubInfoEx.HubType) {
            case UsbRootHub:
                in << tr("USB Root Hub") << endl;
                break;
            case Usb20Hub:
                in << tr("USB 2.0 Hub") << endl;
                switch (Hubinfo->HubInfoEx.u.UsbHubDescriptor.wHubCharacteristics & 0x0003)
                {
                case 0x0000:
                    in << tr("Power switching:              Ganged") << endl;
                    break;

                case 0x0001:
                    in << tr("Power switching:              Individual") << endl;
                    break;

                case 0x0002:
                case 0x0003:
                    in << tr("Power switching:              None") << endl;
                    break;
                }

                switch (Hubinfo->HubInfoEx.u.UsbHubDescriptor.wHubCharacteristics & 0x0004)
                {
                case 0x0000:
                    in << tr("Compound device:              No") << endl;
                    break;

                case 0x0004:
                    in << tr("Compound device:              Yes") << endl;
                    break;
                }

                switch (Hubinfo->HubInfoEx.u.UsbHubDescriptor.wHubCharacteristics & 0x0018)
                {
                case 0x0000:
                    in << tr("Over-current Protection:      Global") << endl;
                    break;

                case 0x0008:
                    in << tr("Over-current Protection:      Individual") << endl;
                    break;

                case 0x0010:
                case 0x0018:
                    in << tr("No Over-current Protection (Bus Power Only)") << endl;
                    break;
                }
                break;
            case Usb30Hub:
                in << tr("USB 3.0 Hub") << endl;
                switch (Hubinfo->HubInfoEx.u.UsbHubDescriptor.wHubCharacteristics & 0x0003)
                {
                case 0x0000:
                    in << tr("Power switching:              Ganged") << endl;
                    break;

                case 0x0001:
                    in << tr("Power switching:              Individual") << endl;
                    break;

                case 0x0002:
                case 0x0003:
                    in << tr("Power switching:              None") << endl;
                    break;
                }

                switch (Hubinfo->HubInfoEx.u.UsbHubDescriptor.wHubCharacteristics & 0x0004)
                {
                case 0x0000:
                    in << tr("Compound device:              No") << endl;
                    break;

                case 0x0004:
                    in << tr("Compound device:              Yes") << endl;
                    break;
                }

                switch (Hubinfo->HubInfoEx.u.UsbHubDescriptor.wHubCharacteristics & 0x0018)
                {
                case 0x0000:
                    in << tr("Over-current Protection:      Global") << endl;
                    break;

                case 0x0008:
                    in << tr("Over-current Protection:      Individual") << endl;
                    break;

                case 0x0010:
                case 0x0018:
                    in << tr("No Over-current Protection (Bus Power Only)") << endl;
                    break;
                }
                in << tr("Packet Header Decode Latency:            ") << QString::number(Hubinfo->HubInfoEx.u.Usb30HubDescriptor.bHubHdrDecLat, 16).toUpper() << endl;
                in << tr("Delay:                                   ") << QString::number(Hubinfo->HubInfoEx.u.Usb30HubDescriptor.wHubDelay, 16).toUpper() << tr("ns") << endl;
                break;
            default:
                in << tr("ERROR: Unknown hub type") << endl;
                break;
        }
        in << tr("Number of Ports:                  ") << QString::number(Hubinfo->HubInfoEx.HighestPortNumber)<< endl;
        in << tr("High speed capable:               ") << (Hubinfo->HubCapabilityEx.CapabilityFlags.HubIsHighSpeedCapable ? tr("yes") : tr("no")) << endl;
        in << tr("High speed:                       ") << (Hubinfo->HubCapabilityEx.CapabilityFlags.HubIsHighSpeed ? tr("yes") : tr("no")) << endl;
        in << tr("Multiple transaction translations capable:    ") << (Hubinfo->HubCapabilityEx.CapabilityFlags.HubIsMultiTtCapable ? tr("yes") : tr("no")) << endl;
        in << tr("Performs multiple transaction translations simultaneously:    ") << (Hubinfo->HubCapabilityEx.CapabilityFlags.HubIsMultiTt ? tr("yes") : tr("no")) << endl;
        in << tr("Hub wakes when device is connected:   ") << (Hubinfo->HubCapabilityEx.CapabilityFlags.HubIsArmedWakeOnConnect ? tr("yes") : tr("no")) << endl;
        in << tr("Hub is bus powered:               ") << (Hubinfo->HubCapabilityEx.CapabilityFlags.HubIsBusPowered ? tr("yes") : tr("no"))<< endl;
        in << tr("Hub is root:                      ") << (Hubinfo->HubCapabilityEx.CapabilityFlags.HubIsRoot ? tr("yes") : tr("no")) << endl;
        in << endl;
        in << tr("        ---===>Device Information<===---") << endl;
        in << tr("          ===>Device Descriptor<===") << endl;
        in << tr("bLength:                           ") << Hubinfo->ConnectionInfo.DeviceDescriptor.bLength << endl;
        in << tr("bDescriptorType:                   ") << Hubinfo->ConnectionInfo.DeviceDescriptor.bDescriptorType << endl;
        in << tr("bcdUSB:                            ") << Hubinfo->ConnectionInfo.DeviceDescriptor.bcdUSB << endl;
        in << tr("bDeviceClass:                      ") << Hubinfo->ConnectionInfo.DeviceDescriptor.bDeviceClass << endl;
        in << tr("bDeviceSubClass:                   ") << Hubinfo->ConnectionInfo.DeviceDescriptor.bDeviceSubClass << endl;
        in << tr("bDeviceProtocol:                   ") << Hubinfo->ConnectionInfo.DeviceDescriptor.bDeviceProtocol << endl;
        in << tr("bMaxPacketSize0:                   ") << Hubinfo->ConnectionInfo.DeviceDescriptor.bMaxPacketSize0 << endl;
        in << tr("idVendor:                          ") << Hubinfo->ConnectionInfo.DeviceDescriptor.idVendor << endl;
        in << tr("idProduct:                         ") << Hubinfo->ConnectionInfo.DeviceDescriptor.idProduct << endl;
        in << tr("bcdDevice:                         ") << Hubinfo->ConnectionInfo.DeviceDescriptor.bcdDevice << endl;
        in << tr("iManufacturer:                     ") << Hubinfo->ConnectionInfo.DeviceDescriptor.iManufacturer << endl;
        int num = 0;
        if(Hubinfo->ConnectionInfo.DeviceDescriptor.iManufacturer)
        in << tr("           ") << QString::fromStdWString(Hubinfo->StringDescs.at(num++)) << endl;
        in << tr("iProduct:                          ") << Hubinfo->ConnectionInfo.DeviceDescriptor.iProduct<< endl;
              if(Hubinfo->ConnectionInfo.DeviceDescriptor.iProduct)
              in << tr("           ") << QString::fromStdWString(Hubinfo->StringDescs.at(num++)) << endl;
        in << tr("iSerialNumber:                     ") << Hubinfo->ConnectionInfo.DeviceDescriptor.iSerialNumber << endl;
              if(Hubinfo->ConnectionInfo.DeviceDescriptor.iSerialNumber)
              in << tr("           ") << QString::fromStdWString(Hubinfo->StringDescs.at(num++)) << endl;
        in << tr("bNumConfigurations:                ") << Hubinfo->ConnectionInfo.DeviceDescriptor.bNumConfigurations << endl;
        in << tr("           ---===>Full Configuration Descriptor<===---") << endl;
        for(auto& i : Hubinfo->AllDescs)
        {
            if(i.Usb_Desc_Types == USB_CONFIGURATION_DESC)
            {
                in << tr("          ===>Configuration Descriptor<===") << endl;
                in << tr("bLength:                           ") << i.Descriptors.ConfigDesc.bLength << endl;
                in << tr("bDescriptorType:                   ") << i.Descriptors.ConfigDesc.bDescriptorType << endl;
                in << tr("wTotalLength:                      ") << i.Descriptors.ConfigDesc.wTotalLength << endl;
                in << tr("bNumInterfaces:                    ") << i.Descriptors.ConfigDesc.bNumInterfaces << endl;
                in << tr("bConfigurationValue:               ") << i.Descriptors.ConfigDesc.bConfigurationValue<< endl;
                in << tr("iConfiguration:                    ") << i.Descriptors.ConfigDesc.iConfiguration << endl;
                if(i.Descriptors.ConfigDesc.iConfiguration)
                in << tr("           ") << QString::fromStdWString(i.StringDescriptors) << endl;
                in << tr("bmAttributes:                      ") << i.Descriptors.ConfigDesc.bmAttributes << endl;
                in << tr("MaxPower:                          ") << i.Descriptors.ConfigDesc.MaxPower << endl;
             }
            else if(i.Usb_Desc_Types == USB_INTERFACE_DESC)
            {
                in << tr("          ===>Interface Descriptor<===") << endl;
                in << tr("bLength:                           ") << i.Descriptors.InterFaceDesc.bLength << endl;
                in << tr("bDescriptorType:                   ") << i.Descriptors.InterFaceDesc.bDescriptorType << endl;
                in << tr("bInterfaceNumber:                  ") << i.Descriptors.InterFaceDesc.bInterfaceNumber << endl;
                in << tr("bAlternateSetting:                 ") << i.Descriptors.InterFaceDesc.bAlternateSetting << endl;
                in << tr("bNumEndpoints:                     ") << i.Descriptors.InterFaceDesc.bNumEndpoints<< endl;
                in << tr("bInterfaceClass:                   ") << i.Descriptors.InterFaceDesc.bInterfaceClass << endl;
                in << tr("bInterfaceSubClass:                ") << i.Descriptors.InterFaceDesc.bInterfaceSubClass << endl;
                in << tr("bInterfaceProtocol:                ") << i.Descriptors.InterFaceDesc.bInterfaceProtocol << endl;
                in << tr("iInterface:                        ") << i.Descriptors.InterFaceDesc.iInterface << endl;
                if(i.Descriptors.InterFaceDesc.iInterface)
                in << tr("           ") << QString::fromStdWString(i.StringDescriptors) << endl;
            }
            else if(i.Usb_Desc_Types == USB_ENDPOINT_DESC)
            {
                in << tr("          ===>Endpoint Descriptor<===") << endl;
                in << tr("bLength:                           ") << i.Descriptors.EndPoint.bLength << endl;
                in << tr("bDescriptorType:                   ") << i.Descriptors.EndPoint.bDescriptorType << endl;
                in << tr("bEndpointAddress:                  ") << i.Descriptors.EndPoint.bEndpointAddress << endl;
                in << tr("bmAttributes:                      ") << i.Descriptors.EndPoint.bmAttributes << endl;
                in << tr("wMaxPacketSize:                    ") << i.Descriptors.EndPoint.wMaxPacketSize<< endl;
                in << tr("bInterval:                         ") << i.Descriptors.EndPoint.bInterval << endl;
            }
            else if(i.Usb_Desc_Types == USB_IAD_DESC)
            {
                in << tr("          ===>IAD Descriptor<===") << endl;
                in << tr("bLength:                           ") << i.Descriptors.IADDesc.bLength << endl;
                in << tr("bDescriptorType:                   ") << i.Descriptors.IADDesc.bDescriptorType << endl;
                in << tr("bFirstInterface:                   ") << i.Descriptors.IADDesc.bFirstInterface << endl;
                in << tr("bInterfaceCount:                   ") << i.Descriptors.IADDesc.bInterfaceCount << endl;
                in << tr("bFunctionClass:                    ") << i.Descriptors.IADDesc.bFunctionClass<< endl;
                in << tr("bFunctionSubClass:                 ") << i.Descriptors.IADDesc.bFunctionSubClass << endl;
                in << tr("bFunctionProtocol:                 ") << i.Descriptors.IADDesc.bFunctionProtocol << endl;
                in << tr("iFunction:                         ") << i.Descriptors.IADDesc.iFunction << endl;
                if(i.Descriptors.IADDesc.iFunction)
                in << tr("           ") << QString::fromStdWString(i.StringDescriptors) << endl;
            }
            else if(i.Usb_Desc_Types == USB_HID_DESC)
            {
                in << tr("          ===>HID Descriptor<===") << endl;
                in << tr("bLength:                           ") << i.Descriptors.HIDDesc.bLength << endl;
                in << tr("bDescriptorType:                   ") << i.Descriptors.HIDDesc.bDescriptorType << endl;
                in << tr("bcdHID:                            ") << i.Descriptors.HIDDesc.bcdHID << endl;
                in << tr("bCountryCode:                      ") << i.Descriptors.HIDDesc.bCountryCode << endl;
                in << tr("bNumDescriptors:                   ") << i.Descriptors.HIDDesc.bNumDescriptors<< endl;
                in << tr("bDescriptorType:                   ") << i.Descriptors.HIDDesc.OptionalDescriptors[0].bDescriptorType << endl;
                in << tr("wDescriptorLength:                 ") << i.Descriptors.HIDDesc.OptionalDescriptors[0].wDescriptorLength << endl;
             }
            else
            {
                in << tr("          ===>Unknown Descriptor<===") << endl;
            }
        }
        for(auto&j : Hubinfo->Usb_Pipe_Info)
        {
            if(j.EndpointDescriptor.bLength == 0)
            {
                break;
            }
            else
            {
                in << tr("          ===>Endpoint Descriptor<===") << endl;
                in << tr("bLength:                           ") << j.EndpointDescriptor.bLength << endl;
                in << tr("bDescriptorType:                   ") << j.EndpointDescriptor.bDescriptorType << endl;
                in << tr("bEndpointAddress:                  ") << j.EndpointDescriptor.bEndpointAddress << endl;
                in << tr("bmAttributes:                      ") << j.EndpointDescriptor.bmAttributes << endl;
                in << tr("wMaxPacketSize:                    ") << j.EndpointDescriptor.wMaxPacketSize<< endl;
                in << tr("bInterval:                         ") << j.EndpointDescriptor.bInterval << endl;
            }
        }
        file.flush();
        file.close();
    }
}

void usbtreewidget::SaveDeviceInfo(const QString thekey, const USBDEVICEINFO * const deviceinfo)
{
    QFile file(thekey);
    if(!file.open(QFile::WriteOnly))
    {
       qDebug() << "Can not open " << thekey;
    }
    else
    {
        QTextStream in(&file);
        in << tr("Is Port User Connectable:         ") << (deviceinfo->PortConnectorProps.UsbPortProperties.PortIsUserConnectable ? tr("yes") : tr("no")) << endl;
        in << tr("Is Port Debug Capable:            ") << (deviceinfo->PortConnectorProps.UsbPortProperties.PortIsDebugCapable ? tr("yes") : tr("no")) << endl;
        in << tr("Is Port TypeC:                    ") << (deviceinfo->PortConnectorProps.UsbPortProperties.PortConnectorIsTypeC ? tr("yes") : tr("no")) << endl;
        in << tr("Companion Port Number:            ") << QString::number(deviceinfo->PortConnectorProps.CompanionPortNumber) << endl;
        in << tr("Companion Hub Symbolic Link Name: ") << QString::fromStdWString(deviceinfo->PortConnectorProps.CompanionHubSymbolicLinkName) << endl;
        in << tr("Protocols Supported:              ") << endl;
        in << tr(" USB 1.1:                         ") << (deviceinfo->ConnectionInfoV2.SupportedUsbProtocols.Usb110 ? tr("yes") : tr("no")) << endl;
        in << tr(" USB 2.0:                         ") << (deviceinfo->ConnectionInfoV2.SupportedUsbProtocols.Usb200 ? tr("yes") : tr("no")) << endl;
        in << tr(" USB 3.0:                         ") << (deviceinfo->ConnectionInfoV2.SupportedUsbProtocols.Usb300 ? tr("yes") : tr("no")) << endl;
        in << endl;
        if(deviceinfo->ConnectionInfo.ConnectionStatus == NoDeviceConnected)
        {
            in << tr("ConnectionStatus:                 ") << tr("NoDeviceConnected") << endl;
        }
        else
        {
           in << tr("        ---===>Device Information<===---") << endl;
           in << tr("          ===>Device Descriptor<===") << endl;
           in << tr("bLength:                           ") << deviceinfo->ConnectionInfo.DeviceDescriptor.bLength << endl;
           in << tr("bDescriptorType:                   ") << deviceinfo->ConnectionInfo.DeviceDescriptor.bDescriptorType << endl;
           in << tr("bcdUSB:                            ") << deviceinfo->ConnectionInfo.DeviceDescriptor.bcdUSB << endl;
           in << tr("bDeviceClass:                      ") << deviceinfo->ConnectionInfo.DeviceDescriptor.bDeviceClass << endl;
           in << tr("bDeviceSubClass:                   ") << deviceinfo->ConnectionInfo.DeviceDescriptor.bDeviceSubClass << endl;
           in << tr("bDeviceProtocol:                   ") << deviceinfo->ConnectionInfo.DeviceDescriptor.bDeviceProtocol << endl;
           in << tr("bMaxPacketSize0:                   ") << deviceinfo->ConnectionInfo.DeviceDescriptor.bMaxPacketSize0 << endl;
           in << tr("idVendor:                          ") << deviceinfo->ConnectionInfo.DeviceDescriptor.idVendor << endl;
           in << tr("idProduct:                         ") << deviceinfo->ConnectionInfo.DeviceDescriptor.idProduct << endl;
           in << tr("bcdDevice:                         ") << deviceinfo->ConnectionInfo.DeviceDescriptor.bcdDevice << endl;
           in << tr("iManufacturer:                     ") << deviceinfo->ConnectionInfo.DeviceDescriptor.iManufacturer << endl;
           int num = 0;
           if(deviceinfo->ConnectionInfo.DeviceDescriptor.iManufacturer)
           in << tr("           ") << QString::fromStdWString(deviceinfo->StringDescs.at(num++)) << endl;
           in << tr("iProduct:                          ") << deviceinfo->ConnectionInfo.DeviceDescriptor.iProduct<< endl;
                 if(deviceinfo->ConnectionInfo.DeviceDescriptor.iProduct)
                 in << tr("           ") << QString::fromStdWString(deviceinfo->StringDescs.at(num++)) << endl;
           in << tr("iSerialNumber:                     ") << deviceinfo->ConnectionInfo.DeviceDescriptor.iSerialNumber << endl;
                 if(deviceinfo->ConnectionInfo.DeviceDescriptor.iSerialNumber)
                 in << tr("           ") << QString::fromStdWString(deviceinfo->StringDescs.at(num++)) << endl;
           in << tr("bNumConfigurations:                ") << deviceinfo->ConnectionInfo.DeviceDescriptor.bNumConfigurations << endl;
           in << tr("           ---===>Full Configuration Descriptor<===---") << endl;
           for(auto& i : deviceinfo->AllDescs)
           {
               if(i.Usb_Desc_Types == USB_CONFIGURATION_DESC)
               {
                   in << tr("          ===>Configuration Descriptor<===") << endl;
                   in << tr("bLength:                           ") << i.Descriptors.ConfigDesc.bLength << endl;
                   in << tr("bDescriptorType:                   ") << i.Descriptors.ConfigDesc.bDescriptorType << endl;
                   in << tr("wTotalLength:                      ") << i.Descriptors.ConfigDesc.wTotalLength << endl;
                   in << tr("bNumInterfaces:                    ") << i.Descriptors.ConfigDesc.bNumInterfaces << endl;
                   in << tr("bConfigurationValue:               ") << i.Descriptors.ConfigDesc.bConfigurationValue<< endl;
                   in << tr("iConfiguration:                    ") << i.Descriptors.ConfigDesc.iConfiguration << endl;
                   if(i.Descriptors.ConfigDesc.iConfiguration)
                   in << tr("           ") << QString::fromStdWString(i.StringDescriptors) << endl;
                   in << tr("bmAttributes:                      ") << i.Descriptors.ConfigDesc.bmAttributes << endl;
                   in << tr("MaxPower:                          ") << i.Descriptors.ConfigDesc.MaxPower << endl;
                }
               else if(i.Usb_Desc_Types == USB_INTERFACE_DESC)
               {
                   in << tr("          ===>Interface Descriptor<===") << endl;
                   in << tr("bLength:                           ") << i.Descriptors.InterFaceDesc.bLength << endl;
                   in << tr("bDescriptorType:                   ") << i.Descriptors.InterFaceDesc.bDescriptorType << endl;
                   in << tr("bInterfaceNumber:                  ") << i.Descriptors.InterFaceDesc.bInterfaceNumber << endl;
                   in << tr("bAlternateSetting:                 ") << i.Descriptors.InterFaceDesc.bAlternateSetting << endl;
                   in << tr("bNumEndpoints:                     ") << i.Descriptors.InterFaceDesc.bNumEndpoints<< endl;
                   in << tr("bInterfaceClass:                   ") << i.Descriptors.InterFaceDesc.bInterfaceClass << endl;
                   in << tr("bInterfaceSubClass:                ") << i.Descriptors.InterFaceDesc.bInterfaceSubClass << endl;
                   in << tr("bInterfaceProtocol:                ") << i.Descriptors.InterFaceDesc.bInterfaceProtocol << endl;
                   in << tr("iInterface:                        ") << i.Descriptors.InterFaceDesc.iInterface << endl;
                   if(i.Descriptors.InterFaceDesc.iInterface)
                   in << tr("           ") << QString::fromStdWString(i.StringDescriptors) << endl;
               }
               else if(i.Usb_Desc_Types == USB_ENDPOINT_DESC)
               {
                   in << tr("          ===>Endpoint Descriptor<===") << endl;
                   in << tr("bLength:                           ") << i.Descriptors.EndPoint.bLength << endl;
                   in << tr("bDescriptorType:                   ") << i.Descriptors.EndPoint.bDescriptorType << endl;
                   in << tr("bEndpointAddress:                  ") << i.Descriptors.EndPoint.bEndpointAddress << endl;
                   in << tr("bmAttributes:                      ") << i.Descriptors.EndPoint.bmAttributes << endl;
                   in << tr("wMaxPacketSize:                    ") << i.Descriptors.EndPoint.wMaxPacketSize<< endl;
                   in << tr("bInterval:                         ") << i.Descriptors.EndPoint.bInterval << endl;
               }
               else if(i.Usb_Desc_Types == USB_IAD_DESC)
               {
                   in << tr("          ===>IAD Descriptor<===") << endl;
                   in << tr("bLength:                           ") << i.Descriptors.IADDesc.bLength << endl;
                   in << tr("bDescriptorType:                   ") << i.Descriptors.IADDesc.bDescriptorType << endl;
                   in << tr("bFirstInterface:                   ") << i.Descriptors.IADDesc.bFirstInterface << endl;
                   in << tr("bInterfaceCount:                   ") << i.Descriptors.IADDesc.bInterfaceCount << endl;
                   in << tr("bFunctionClass:                    ") << i.Descriptors.IADDesc.bFunctionClass<< endl;
                   in << tr("bFunctionSubClass:                 ") << i.Descriptors.IADDesc.bFunctionSubClass << endl;
                   in << tr("bFunctionProtocol:                 ") << i.Descriptors.IADDesc.bFunctionProtocol << endl;
                   in << tr("iFunction:                         ") << i.Descriptors.IADDesc.iFunction << endl;
                   if(i.Descriptors.IADDesc.iFunction)
                   in << tr("           ") << QString::fromStdWString(i.StringDescriptors) << endl;
               }
               else if(i.Usb_Desc_Types == USB_HID_DESC)
               {
                   in << tr("          ===>HID Descriptor<===") << endl;
                   in << tr("bLength:                           ") << i.Descriptors.HIDDesc.bLength << endl;
                   in << tr("bDescriptorType:                   ") << i.Descriptors.HIDDesc.bDescriptorType << endl;
                   in << tr("bcdHID:                            ") << i.Descriptors.HIDDesc.bcdHID << endl;
                   in << tr("bCountryCode:                      ") << i.Descriptors.HIDDesc.bCountryCode << endl;
                   in << tr("bNumDescriptors:                   ") << i.Descriptors.HIDDesc.bNumDescriptors<< endl;
                   in << tr("bDescriptorType:                   ") << i.Descriptors.HIDDesc.OptionalDescriptors[0].bDescriptorType << endl;
                   in << tr("wDescriptorLength:                 ") << i.Descriptors.HIDDesc.OptionalDescriptors[0].wDescriptorLength << endl;
                }
               else
               {
                   in << tr("          ===>Unknown Descriptor<===") << endl;
               }
           }
           for(auto&j : deviceinfo->Usb_Pipe_Info)
           {
               if(j.EndpointDescriptor.bLength == 0)
               {
                   break;
               }
               else
               {
                   in << tr("          ===>Endpoint Descriptor<===") << endl;
                   in << tr("bLength:                           ") << j.EndpointDescriptor.bLength << endl;
                   in << tr("bDescriptorType:                   ") << j.EndpointDescriptor.bDescriptorType << endl;
                   in << tr("bEndpointAddress:                  ") << j.EndpointDescriptor.bEndpointAddress << endl;
                   in << tr("bmAttributes:                      ") << j.EndpointDescriptor.bmAttributes << endl;
                   in << tr("wMaxPacketSize:                    ") << j.EndpointDescriptor.wMaxPacketSize<< endl;
                   in << tr("bInterval:                         ") << j.EndpointDescriptor.bInterval << endl;
               }
           }
        }
           file.flush();
        file.close();
    }
}

