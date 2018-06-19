#include "Ring0lib.h"
#include "Zhaoxin.h"

namespace SV_ASSIST
{
	namespace Ring0lib
	{
		BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD64 & IO_Data)
		{
			return ZhaoxinDriver::RdIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
		}

		BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data)
		{
			return ZhaoxinDriver::WrIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
		}

		BOOL RdMsr(IN DWORD Index, OUT DWORD64 & Data, IN DWORD_PTR threadAffinityMask)
		{
			return ZhaoxinDriver::RdMsr(Index, Data, threadAffinityMask);
		}

		BOOL WrMsr(IN DWORD Index, IN DWORD64 Data, IN DWORD_PTR threadAffinityMask)
		{
			return ZhaoxinDriver::WrMsr(Index, Data, threadAffinityMask);;
		}

		BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG & Memory_Data)
		{
			return ZhaoxinDriver::RdMemory(Memory_Addr, Mem_DataSize, Memory_Data);
		}

		BOOL WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
		{
			return ZhaoxinDriver::WrMemory(Memory_Addr, Mem_DataSize, Memory_Data);
		}

		BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG & pci_config)
		{
			return ZhaoxinDriver::ReadPci(bus, dev, func, pci_config);
		}

		BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data)
		{
			return ZhaoxinDriver::WritePci(bus, dev, func, offset, Data);
		}

		BOOL ReadSPD(USHORT Base_Address, USHORT Slave_Address, DDR3_INFO & data)
		{
			return ZhaoxinDriver::ReadSPD(Base_Address, Slave_Address, data);
		}

		BOOL GetECData(Embedded_Controller_Data & EC_Data)
		{
			return ZhaoxinDriver::GetECData(EC_Data);
		}

		BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data)
		{
			return ZhaoxinDriver::SetECData(EC_Addr, EC_Write_Data);
		}

		BOOL GetSMbusBaseAddr(const USHORT VendorID, const PCI_COMMON_CONFIG pci, USHORT& SMbusBaseAddress)
		{
			BOOL Success = FALSE;
			ULONG tmp = 0;
			switch (VendorID)
			{
			case 0x8086:
				tmp = pci.u.type0.BaseAddresses[4];
				SMbusBaseAddress = (USHORT)((tmp & 0x0000FFE) >> 5);
				Success = TRUE;
				break;
			case 0x1106:
				SMbusBaseAddress = 0x400;
				Success = TRUE;
				break;
			case 0x1022:
			case 0x1002:
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
