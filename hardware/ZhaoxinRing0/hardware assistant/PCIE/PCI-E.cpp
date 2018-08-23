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
					m_pcie = std::make_shared<PCIELib>();
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

			void UpdateData() { m_optionROM->UpdateData(); }

			BOOL GetPCIDeviceName(const USHORT VenderID, const USHORT DeviceID, std::string & VenderName, std::string & DeviceName);

		protected:
		private:
			explicit PCIELib(const PCIELib& that);
			PCIELib& operator=(const PCIELib& that) {}
			BOOL InitPciDB();
		private:
			PCIDB pciDB;
			std::shared_ptr<Option_Rom> m_optionROM;
			static std::shared_ptr<PCIELib> m_pcie;

		};
		std::shared_ptr<PCIELib> PCIELib::m_pcie = nullptr;


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
				std::cerr << "pci.ids file is not available. Download it from https://raw.githubusercontent.com/pciutils/pciids/master/pci.ids" << endl;
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

BOOL SV_ASSIST::PCIE::GetPCIDeviceName(const USHORT VenderID, const USHORT DeviceID, std::string & VenderName, std::string & DeviceName)
{
	return SV_ASSIST::PCIE::PCIELib::Instance()->GetPCIDeviceName(VenderID, DeviceID, VenderName, DeviceName);
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