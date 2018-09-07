#include "stdafx.h"
#include <fstream>
#include "..\driver\DriverOrigin.h"
#include "PCI-E.h"
#include "pcioptionrom.h"



namespace SV_ASSIST
{
	namespace PCIE
	{
		typedef std::pair<std::map<int,std::string>, std::map<int, std::map<int, std::string>>> PCIDB;
		class PCIELib
		{
		public:
			static PCIELib* Instance()
			{
				if (!m_pcie.get())
					m_pcie = std::make_unique<PCIELib>();
				return m_pcie.get();
			}

			const std::vector<std::pair<PCI_OPTION_ROM_HEADER, PCI_OPTION_ROM_Data_Structure>>& GetPCIOptionROM() const
			{
				return m_optionROM->GetPCIOptionRom();
			}
			PCIELib() : m_optionROM(std::make_shared<Option_Rom>())
			{
				this->InitPciDB();
			}

			BOOL GetPciSpeed(const PCI_COMMON_CONFIG &data, PCISPEED& MAXSpeed, PCISPEED& CurrentSpeed)
			{
				const UCHAR* capaaddr = reinterpret_cast<const UCHAR*>(&data);
				USHORT CapabilitiesPtr = data.u.type0.CapabilitiesPtr;
				bool IsPCIE10H = false;
				while (true)
				{
					if (capaaddr[CapabilitiesPtr] == 0x10)
					{
						IsPCIE10H = true;
						break;
					}
					if (capaaddr[CapabilitiesPtr + 1] == 0x00)
						break;
					CapabilitiesPtr = capaaddr[CapabilitiesPtr + 1];
				}
				if (IsPCIE10H)
				{
					MAXSpeed.LinkSpeed = capaaddr[CapabilitiesPtr + 0x0C] & 0xFF;
					USHORT temp = capaaddr[CapabilitiesPtr + 0x0C] | (capaaddr[CapabilitiesPtr + 0x0D] << 8);
					MAXSpeed.LinkWidth = (temp & 0x3F0) >> 4;

					CurrentSpeed.LinkSpeed = capaaddr[CapabilitiesPtr + 0x12] & 0xFF;
					temp = capaaddr[CapabilitiesPtr + 0x12] | (capaaddr[CapabilitiesPtr + 0x13] << 8);
					CurrentSpeed.LinkWidth = (temp & 0x3F0) >> 4;
					return TRUE;
				}
				return FALSE;
			}

			void UpdateData() { m_optionROM->UpdateData(); }

			BOOL GetPCIVendorName(const USHORT VenderID, std::string & VenderName);

			BOOL GetPCIDeviceName(const USHORT VenderID, const USHORT DeviceID, std::string & VenderName, std::string & DeviceName);

		protected:
		private:
			explicit PCIELib(const PCIELib& that);
			PCIELib& operator=(const PCIELib& that) {}
			BOOL InitPciDB();
		private:
			PCIDB pciDB;
			std::shared_ptr<Option_Rom> m_optionROM;
			static std::unique_ptr<PCIELib> m_pcie;

		};
		std::unique_ptr<PCIELib> PCIELib::m_pcie = nullptr;

		BOOL PCIELib::GetPCIVendorName(const USHORT VenderID, std::string & VenderName)
		{
			VenderName = (pciDB.first.count(VenderID) > 0) ? pciDB.first.at(VenderID).c_str() : "unknown vendor";
			return TRUE;
		}

		BOOL PCIELib::GetPCIDeviceName(const USHORT VenderID, const USHORT DeviceID, std::string & VenderName, std::string & DeviceName)
		{
			VenderName = (pciDB.first.count(VenderID) > 0) ? pciDB.first.at(VenderID).c_str() : "unknown vendor";
			DeviceName = (pciDB.second.count(VenderID) > 0 && pciDB.second.at(VenderID).count(DeviceID) > 0) ? pciDB.second.at(VenderID).at(DeviceID).c_str() : "unknown device";
			return TRUE;
		}

		BOOL PCIELib::InitPciDB()
		{
			std::ifstream in("pci.ids");
			std::string line, item;

			if (!in.is_open())
			{
				std::cerr << "pci.ids file is not available. Download it from https://raw.githubusercontent.com/pciutils/pciids/master/pci.ids" << std::endl;
				return FALSE;
			}

			int vendorID = -1;

			while (std::getline(in, line)) {
				// Ignore any line starting with #
				if (line.size() == 0 || line[0] == '#')
					continue;

				if (line[0] == '\t' && line[1] == '\t')
				{
					// subvendor subdevice  subsystem_name
					continue;
				}
				if (line[0] == '\t')
				{
					int deviceID = stoi(line.substr(1, 4), 0, 16);
					pciDB.second[vendorID][deviceID] = line.substr(7);
					continue;
				}
				// vendor
				vendorID = stoi(line.substr(0, 4), 0, 16);
				pciDB.first[vendorID] = line.substr(6);
			}
			return TRUE;
		}
	}
}

#define MK_PCIEADDR(bus, dev, func, off) (ULONG)(((bus & 0xFF) << 20) | (dev & 0x1F) << 15 | (func & 0x07) << 12 | off)

BOOL SV_ASSIST::PCIE::GetPCIEConfigSpace(const ULONGLONG PCIE_BaseAddress, ULONG bus, ULONG dev, ULONG func, PVOID& Data, size_t DataSize)
{
	if(DataSize < 4096)	return FALSE;
	ULONG tmp = 0;
	for (USHORT offset = 0; offset < 0x100; offset += 4)
	{
		ZhaoxinDriver::Instance()->RdMemory(PCIE_BaseAddress | MK_PCIEADDR(bus, dev, func, offset), 4, tmp);
		memcpy((PBYTE)Data + offset, &tmp, 4);
	}
	return TRUE;
}

BOOL SV_ASSIST::PCIE::GetPCIVendorName(const USHORT VenderID, std::string & VenderName)
{
	return SV_ASSIST::PCIE::PCIELib::Instance()->GetPCIVendorName(VenderID, VenderName);
}

BOOL SV_ASSIST::PCIE::GetPCIDeviceName(const USHORT VenderID, const USHORT DeviceID, std::string & VenderName, std::string & DeviceName)
{
	return SV_ASSIST::PCIE::PCIELib::Instance()->GetPCIDeviceName(VenderID, DeviceID, VenderName, DeviceName);
}

BOOL SV_ASSIST::PCIE::ExecPCIClassCode(const PCI_COMMON_CONFIG & data, std::string& DeviceType)
{
	DeviceType = "Unknown PCI Class";
	switch (data.BaseClass)
	{
	case 0:
	case 0xFF:
		DeviceType = "Unclassified device";
		break;
	case 1:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "SCSI controller";
			break;
		case 1:
			DeviceType = "IDE controller";
			break;
		case 2:
			DeviceType = "Floppy disk controller";
			break;
		case 3:
			DeviceType = "IPI bus controller";
			break;
		case 4:
			DeviceType = "RAID bus controller";
			break;
		case 5:
			DeviceType = "ATA controller";
			break;
		case 6:
			DeviceType = "SATA controller";
			break;
		case 7:
			DeviceType = "Serial Attached SCSI controller";
			break;
		case 8:
			DeviceType = "Non-Volatile memory controller";
			break;
		case 0x80:
			DeviceType = "Mass storage controller";
			break;
		default:
			break;
		}
		break;
	case 2:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "Ethernet controller";
			break;
		case 1:
			DeviceType = "Token ring network controller";
			break;
		case 2:
			DeviceType = "FDDI network controller";
			break;
		case 3:
			DeviceType = "ATM network controller";
			break;
		case 4:
			DeviceType = "ISDN controller";
			break;
		case 5:
			DeviceType = "WorldFip controller";
			break;
		case 6:
			DeviceType = "PICMG controller";
			break;
		case 7:
			DeviceType = "Infiniband controller";
			break;
		case 8:
			DeviceType = "Fabric controller";
			break;
		case 0x80:
			DeviceType = "Network controller";
			break;
		default:
			break;
		}
		break;
	case 3:
		switch (data.SubClass)
		{
		case 0:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "VGA controller";
				break;
			case 1:
				DeviceType = "8514 controller";
				break;
			default:
				break;
			}
			break;
		case 1:
			DeviceType = "XGA compatible controller";
			break;
		case 2:
			DeviceType = "3D controller";
			break;
		case 0x80:
			DeviceType = "Display controller";
			break;
		default:
			break;
		}
		break;
	case 4:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "Multimedia video controller";
			break;
		case 1:
			DeviceType = "Multimedia audio controller";
			break;
		case 2:
			DeviceType = "Computer telephony device";
			break;
		case 3:
			DeviceType = "Audio device";
			break;
		case 0x80:
			DeviceType = "Multimedia controller";
			break;
		default:
			break;
		}
		break;
	case 5:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "RAM memory";
			break;
		case 1:
			DeviceType = "FLASH memory";
			break;
		case 0x80:
			DeviceType = "Memory controller";
			break;
		default:
			break;
		}
		break;
	case 6:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "Host bridge";
			break;
		case 1:
			DeviceType = "ISA bridge";
			break;
		case 2:
			DeviceType = "EISA bridge";
			break;
		case 3:
			DeviceType = "MicroChannel bridge";
			break;
		case 4:
			DeviceType = "PCI bridge";
			break;
		case 5:
			DeviceType = "PCMCIA bridge";
			break;
		case 6:
			DeviceType = "NuBus bridge";
			break;
		case 7:
			DeviceType = "CardBus bridge";
			break;
		case 8:
			DeviceType = "RACEway bridge";
			break;
		case 9:
			DeviceType = "Semi-transparent PCI-to-PCI bridge";
			break;
		case 0x0a:
			DeviceType = "	InfiniBand to PCI host bridge";
			break;
		case 0x80:
			DeviceType = "Bridge";
			break;
		default:
			break;
		}
		break;
	case 7:
		switch (data.SubClass)
		{
		case 0:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "8250 compatable serial controller";
				break;
			case 1:
				DeviceType = "16450 compatable serial controller";
				break;
			case 2:
				DeviceType = "16550 compatable serial controller";
				break;
			case 3:
				DeviceType = "16650 compatable serial controller";
				break;
			case 4:
				DeviceType = "16750 compatable serial controller";
				break;
			case 5:
				DeviceType = "16850 compatable serial controller";
				break;
			case 6:
				DeviceType = "16950 compatable serial controller";
				break;
			default:
				DeviceType = "Serial controller";
				break;
			}
			break;
		case 1:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "SPP Parallel controller";
				break;
			case 1:
				DeviceType = "BiDir Parallel controller";
				break;
			case 2:
				DeviceType = "ECP Parallel controller";
				break;
			case 3:
				DeviceType = "IEEE1284 Parallel controller";
				break;
			case 0xFE:
				DeviceType = "IEEE1284 Target Parallel controller";
				break;
			default:
				DeviceType = "Parallel controller";
				break;
			}
			break;
		case 2:
			DeviceType = "Multiport serial controller";
			break;
		case 3:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "Generic Modem";
				break;
			case 1:
				DeviceType = "Hayes/16450 Modem";
				break;
			case 2:
				DeviceType = "Hayes/16550 Modem";
				break;
			case 3:
				DeviceType = "Hayes/16650 Modem";
				break;
			case 4:
				DeviceType = "Hayes/16750 Modem";
				break;
			default:
				DeviceType = "Modem";
				break;
			}
			break;
		case 4:
			DeviceType = "GPIB controller";
			break;
		case 5:
			DeviceType = "Smard Card controller";
			break;
		case 0x80:
			DeviceType = "Communication controller";
			break;
		default:
			break;
		}
		break;
	case 8:
		switch (data.SubClass)
		{
		case 0:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "8259 PIC";
				break;
			case 1:
				DeviceType = "ISA PIC";
				break;
			case 2:
				DeviceType = "EISA PIC";
				break;
			case 0x10:
				DeviceType = "IO-APIC";
				break;
			case 0x20:
				DeviceType = "IO(X)-APIC";
				break;
			default:
				DeviceType = "PIC";
				break;
			}
			break;
		case 1:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "8237 DMA controller";
				break;
			case 1:
				DeviceType = "ISA DMA controller";
				break;
			case 2:
				DeviceType = "EISA DMA controller";
				break;
			default:
				DeviceType = "DMA controller";
				break;
			}
			break;
		case 2:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "8254 Timer";
				break;
			case 1:
				DeviceType = "ISA Timer";
				break;
			case 2:
				DeviceType = "EISA Timer";
				break;
			case 3:
				DeviceType = "HPET";
				break;
			default:
				DeviceType = "Timer";
				break;
			}
			break;
		case 3:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "Generic RTC controller";
				break;
			case 1:
				DeviceType = "ISA RTC controller";
				break;
			default:
				DeviceType = "RTC controller";
				break;
			}
			break;
		case 4:
			DeviceType = "PCI Hot-plug controller";
			break;
		case 5:
			DeviceType = "SD Host controller";
			break;
		case 6:
			DeviceType = "IOMMU";
			break;
		case 0x80:
			DeviceType = "System peripheral";
			break;
		default:
			break;
		}
		break;
	case 9:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "Keyboard controller";
			break;
		case 1:
			DeviceType = "Digitizer Pen";
			break;
		case 2:
			DeviceType = "Mouse controller";
			break;
		case 3:
			DeviceType = "Scanner controller";
			break;
		case 4:
			DeviceType = "Gameport controller";
			break;
		case 0x80:
			DeviceType = "Input device controller";
			break;
		default:
			break;
		}
		break;
	case 0x0A:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "Generic Docking Station";
			break;
		case 0x80:
			DeviceType = "Docking Station";
			break;
		default:
			break;
		}
		break;
	case 0x0B:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "386";
			break;
		case 1:
			DeviceType = "486";
			break;
		case 2:
			DeviceType = "Pentium";
			break;
		case 0x10:
			DeviceType = "Alpha";
			break;
		case 0x20:
			DeviceType = "Power PC";
			break;
		case 0x30:
			DeviceType = "MIPS";
			break;
		case 0x40:
			DeviceType = "Co-processor";
			break;
		default:
			DeviceType = "Processor";
			break;
		}
		break;
	case 0x0C:
		switch (data.SubClass)
		{
		case 0:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "Generic FireWire (IEEE 1394)";
				break;
			case 0x10:
				DeviceType = "OHCI FireWire (IEEE 1394)";
				break;
			default:
				DeviceType = "FireWire (IEEE 1394)";
				break;
			}
			break;
		case 1:
			DeviceType = "ACCESS Bus";
			break;
		case 2:
			DeviceType = "SSA (Serial Storage Architecture)";
			break;
		case 3:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "UHCI USB controller";
				break;
			case 0x10:
				DeviceType = "OHCI USB controller";
				break;
			case 0x20:
				DeviceType = "EHCI USB controller";
				break;
			case 0x30:
				DeviceType = "XHCI USB controller";
				break;
			case 0x80:
				DeviceType = "Unspecified USB controller";
				break;
			case 0xFE:
				DeviceType = "USB Device";
				break;
			default:
				DeviceType = "USB controller";
				break;
			}
			break;
		case 4:
			DeviceType = "Fibre Channel";
			break;
		case 5:
			DeviceType = "SMBUS controller";
			break;
		case 6:
			DeviceType = "InfiniBand controller";
			break;
		case 7:
			switch (data.ProgIf)
			{
			case 0:
				DeviceType = "SMIC Interface";
				break;
			case 1:
				DeviceType = "KCS Interface";
				break;
			case 2:
				DeviceType = "BT (Block Transfer) Interface";
				break;
			default:
				DeviceType = "IPMI Interface";
				break;
			}
			break;
		case 8:
			DeviceType = "SERCOS interface";
			break;
		case 9:
			DeviceType = "CAN BUS";
			break;
		default:
			DeviceType = "Serial bus controller";
			break;
		}
		break;
	case 0x0D:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "IRDA controller";
			break;
		case 1:
			DeviceType = "Consumer IR controller";
			break;
		case 0x10:
			DeviceType = "RF controller";
			break;
		case 0x11:
			DeviceType = "Bluetooth controller";
			break;
		case 0x12:
			DeviceType = "Broadband controller";
			break;
		case 0x20:
			DeviceType = "802.1a controller";
			break;
		case 0x21:
			DeviceType = "802.1b controller";
			break;
		case 0x80:
			DeviceType = "Wireless controller";
			break;
		default:
			break;
		}
		break;
	case 0x0E:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "I2O controller";
			break;
		default:
			DeviceType = "Intelligent controller";
			break;
		}
		break;
	case 0x0F:
		switch (data.SubClass)
		{
		case 1:
			DeviceType = "Satellite TV controller";
			break;
		case 2:
			DeviceType = "Satellite audio communication controller";
			break;
		case 3:
			DeviceType = "Satellite voice communication controller";
			break;
		case 4:
			DeviceType = "Satellite data communication controller";
			break;
		default:
			DeviceType = "Satellite communications controller";
			break;
		}
		break;
	case 0x10:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "Network and computing encryption device";
			break;
		case 0x10:
			DeviceType = "Entertainment encryption device";
			break;
		case 0x80:
			DeviceType = "Encryption controller";
			break;
		default:
			DeviceType = "Encryption controller";
			break;
		}
		break;
	case 0x11:
		switch (data.SubClass)
		{
		case 0:
			DeviceType = "DPIO module";
			break;
		case 1:
			DeviceType = "Performance counters";
			break;
		case 0x10:
			DeviceType = "Communication synchronizer";
			break;
		case 0x20:
			DeviceType = "Signal processing management";
			break;
		case 0x80:
			DeviceType = "Signal processing controller";
			break;
		default:
			DeviceType = "Signal processing controller";
			break;
		}
		break;
	case 0x12:
		DeviceType = "Processing accelerators";
		break;
	case 0x13:
		DeviceType = "Non-Essential Instrumentation";
		break;
	case 0x40:
		DeviceType = "Coprocessor";
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL SV_ASSIST::PCIE::GetPciSpeed(const PCI_COMMON_CONFIG & data, PCISPEED & MAXSpeed, PCISPEED & CurrentSpeed)
{
	return SV_ASSIST::PCIE::PCIELib::Instance()->GetPciSpeed(data, MAXSpeed, CurrentSpeed);
}

const std::vector<std::pair<PCI_OPTION_ROM_HEADER, PCI_OPTION_ROM_Data_Structure>>& SV_ASSIST::PCIE::GetPCIOptionROM()
{
	return SV_ASSIST::PCIE::PCIELib::Instance()->GetPCIOptionROM();
}

void SV_ASSIST::PCIE::UpdateData()
{
	SV_ASSIST::PCIE::PCIELib::Instance()->UpdateData();
}

const BOOL SV_ASSIST::PCIE::PCIstringToBDF(const std::wstring& pcistring, ULONG & bus, ULONG & dev, ULONG & func)
{
	BOOL status = FALSE;
	if (swscanf_s(pcistring.c_str(),
		_T("bus:%d dev:%d func:%d"),
		&bus, &dev, &func))
		status = TRUE;
	return status;
}