#include "pcicombobox.h"
#include "Driverdll.h"

pcicombobox::pcicombobox(QWidget *parent) : QComboBox(parent)
{
    PCI_COMMON_CONFIG Pci_Temp;
    for (USHORT bus = 0; bus <= 0xFF; ++bus)
    {
        for (USHORT dev = 0; dev <= 0x1F; ++dev)
        {
            for (USHORT func = 0; func <= 0x07; ++func)
            {
                ReadPci(bus, dev, func, Pci_Temp);
                if (Pci_Temp.VendorID != 0xFFFF && Pci_Temp.VendorID != 0x0000)
                {
                    this->addItem(tr("bus %1 dev %2 func %3").arg(bus).arg(dev).arg(func));
                }
            }
        }
    }
    connect(this, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), [=](const QString &text){emit SendPCIName(text);});
}
