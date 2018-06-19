// Driverdll.cpp : 定义 DLL 应用程序的导出函数。
//

//#include "stdafx.h"
#include "Driverdll.h"
#include "Zhaoxin.h"

namespace SV_ASSIST
{
	namespace Ring0
	{
		DRIVERDLL_API BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD64 & IO_Data)
		{
			return ZhaoxinDriver::RdIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
		}

		DRIVERDLL_API BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data)
		{
			return ZhaoxinDriver::WrIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
		}

		DRIVERDLL_API BOOL RdMsrTx(IN DWORD Index, OUT DWORD64 & Data, IN DWORD_PTR threadAffinityMask)
		{
			return ZhaoxinDriver::RdMsr(Index, Data);
		}

		DRIVERDLL_API BOOL WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD_PTR threadAffinityMask)
		{
			return ZhaoxinDriver::WrMsr(Index, Data);
		}

		DRIVERDLL_API BOOL RdMsr(IN DWORD Index, OUT DWORD64 & Data)
		{
			return ZhaoxinDriver::RdMsr(Index, Data);
		}

		DRIVERDLL_API BOOL WrMsr(IN DWORD Index, IN DWORD64 Data)
		{
			return ZhaoxinDriver::WrMsr(Index, Data);
		}

		DRIVERDLL_API BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG & Memory_Data)
		{
			return ZhaoxinDriver::RdMemory(Memory_Addr, Mem_DataSize, Memory_Data);
		}

		DRIVERDLL_API BOOL WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
		{
			return ZhaoxinDriver::WrMemory(Memory_Addr, Mem_DataSize, Memory_Data);
		}

		DRIVERDLL_API BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG & pci_config)
		{
			return ZhaoxinDriver::ReadPci(bus, dev, func, pci_config);
		}

		DRIVERDLL_API BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data)
		{
			return ZhaoxinDriver::WritePci(bus, dev, func, offset, Data);
		}

		DRIVERDLL_API BOOL ReadSPD(USHORT Base_Address, USHORT Slave_Address, DDR3_INFO & data)
		{
			return ZhaoxinDriver::ReadSPD(Base_Address, Slave_Address, data);
		}

		DRIVERDLL_API BOOL GetECData(Embedded_Controller_Data & EC_Data)
		{
			return ZhaoxinDriver::GetECData(EC_Data);
		}

		DRIVERDLL_API BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data)
		{
			return ZhaoxinDriver::SetECData(EC_Addr, EC_Write_Data);
		}

		DRIVERDLL_API USHORT GetPCIVendorID()
		{
			return ZhaoxinDriver::GetPCIVendorID();
		}

		DRIVERDLL_API BOOL GetSMbusBaseAddr(const USHORT VendorID, USHORT& SMbusBaseAddress)
		{
			BOOL Success = FALSE;
			ULONG tmp = 0;
			PCI_COMMON_CONFIG pci = {};
			switch (VendorID)
			{
			case 0x8086:
				if (ZhaoxinDriver::ReadPci(0, 31, 3, pci))
				{
					tmp = pci.u.type0.BaseAddresses[4];
					SMbusBaseAddress = (USHORT)((tmp & 0x0000FFE) >> 5);
					Success = TRUE;
				}
				break;
			case 0x1106:
				SMbusBaseAddress = 0x400;
				Success = TRUE;
				break;
			case 0x1002:
				break;
			default:
				break;
			}
			return Success;
		}
	}
}
