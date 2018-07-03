// Driverdll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Driverdll.h"
#include "Zhaoxin.h"
#include "PCI-E.h"

namespace SV_ASSIST
{
	namespace Ring0
	{
		DRIVERDLL_API BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD & IO_Data)
		{
			return ZhaoxinDriver::Instance()->RdIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
		}

		DRIVERDLL_API BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data)
		{
			return ZhaoxinDriver::Instance()->WrIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
		}

		DRIVERDLL_API BOOL RdMsrTx(IN DWORD Index, OUT DWORD64 & Data, IN DWORD threadAffinityMask)
		{
			return ZhaoxinDriver::Instance()->RdMsrTx(Index, Data, threadAffinityMask);
		}

		DRIVERDLL_API BOOL WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD threadAffinityMask)
		{
			return ZhaoxinDriver::Instance()->WrMsrTx(Index, Data, threadAffinityMask);
		}

		DRIVERDLL_API BOOL RdMsr(IN DWORD Index, OUT DWORD64 & Data)
		{
			return ZhaoxinDriver::Instance()->RdMsr(Index, Data);
		}

		DRIVERDLL_API BOOL WrMsr(IN DWORD Index, IN DWORD64 Data)
		{
			return ZhaoxinDriver::Instance()->WrMsr(Index, Data);
		}

		DRIVERDLL_API BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG & Memory_Data)
		{
			return ZhaoxinDriver::Instance()->RdMemory(Memory_Addr, Mem_DataSize, Memory_Data);
		}

		DRIVERDLL_API BOOL WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
		{
			return ZhaoxinDriver::Instance()->WrMemory(Memory_Addr, Mem_DataSize, Memory_Data);
		}

		DRIVERDLL_API BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG & pci_config)
		{
			return ZhaoxinDriver::Instance()->ReadPci(bus, dev, func, pci_config);
		}

		DRIVERDLL_API BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data)
		{
			return ZhaoxinDriver::Instance()->WritePci(bus, dev, func, offset, Data);
		}

		DRIVERDLL_API BOOL ReadSPD(USHORT Base_Address, USHORT Slave_Address, DDR3_INFO & data)
		{
			return ZhaoxinDriver::Instance()->ReadSPD(Base_Address, Slave_Address, data);
		}

		DRIVERDLL_API BOOL GetECData(Embedded_Controller_Data & EC_Data)
		{
			return ZhaoxinDriver::Instance()->GetECData(EC_Data);
		}

		DRIVERDLL_API BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data)
		{
			return ZhaoxinDriver::Instance()->SetECData(EC_Addr, EC_Write_Data);
		}

		DRIVERDLL_API USHORT GetPCIVendorID()
		{
			return ZhaoxinDriver::Instance()->GetPCIVendorID();
		}

		DRIVERDLL_API BOOL GetPCIEInfo(const ULONGLONG PCIE_BaseAddress, ULONG bus, ULONG dev, ULONG func, PVOID*& Data, size_t DataSize)
		{
			return GetPCIEConfigSpace(PCIE_BaseAddress, bus, dev, func, Data, DataSize);
		}

		DRIVERDLL_API BOOL GetPCIDeviceName(USHORT VenderID, USHORT DeviceID, std::string & VenderName, std::string & DeviceName)
		{
			return ZhaoxinDriver::Instance()->GetPCIDeviceName(VenderID, DeviceID, VenderName, DeviceName);
		}

		DRIVERDLL_API const Pci_All_Config_Space & GetAllPciInfo()
		{
			return ZhaoxinDriver::Instance()->GetAllPciInfo();
		}

		DRIVERDLL_API const BOOL PCIstringToBDF(const std::wstring& pcistring,ULONG & bus, ULONG & dev, ULONG & func)
		{
			BOOL status = FALSE;
			if (swscanf_s(pcistring.c_str(),
				_T("bus:%d dev:%d func:%d"),
				&bus, &dev, &func))
				status = TRUE;
			return status;
		}

		DRIVERDLL_API BOOL GetSMbusBaseAddr(const USHORT VendorID, ULONG& SMbusBaseAddress)
		{
			BOOL Success = FALSE;
			ULONG tmp = 0;
			PCI_COMMON_CONFIG pci = {};
			switch (VendorID)
			{
			case 0x8086:
				if (ZhaoxinDriver::Instance()->ReadPci(0, 31, 3, pci))
				{
					tmp = pci.u.type0.BaseAddresses[4];
					SMbusBaseAddress = ((tmp & 0x000FFFE));
					Success = TRUE;
				}
				break;
			case 0x1106:
				SMbusBaseAddress = 0x400;
				Success = TRUE;
				break;
			case 0x1002:
			case 0x1022:
				SMbusBaseAddress = 0xB00;
				Success = TRUE;
				break;
			default:
				break;
			}
			return Success;
		}
	}
}
