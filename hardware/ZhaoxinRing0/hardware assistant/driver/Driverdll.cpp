// Driverdll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Driverdll.h"
#include "DriverOrigin.h"

namespace SV_ASSIST
{
	namespace Ring0
	{
		BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD & IO_Data)
		{
			return ZhaoxinDriver::Instance()->RdIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
		}

		BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data)
		{
			return ZhaoxinDriver::Instance()->WrIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
		}

		BOOL RdMsrTx(IN DWORD Index, OUT DWORD64 & Data, IN DWORD threadAffinityMask)
		{
			return ZhaoxinDriver::Instance()->RdMsrTx(Index, Data, threadAffinityMask);
		}

		BOOL RdTscTx(OUT DWORD64 & Data, IN DWORD threadAffinityMask)
		{
			return ZhaoxinDriver::Instance()->RdTscTx(Data, threadAffinityMask);
		}

		BOOL WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD threadAffinityMask)
		{
			return ZhaoxinDriver::Instance()->WrMsrTx(Index, Data, threadAffinityMask);
		}

		BOOL RdMsr(IN DWORD Index, OUT DWORD64 & Data)
		{
			return ZhaoxinDriver::Instance()->RdMsr(Index, Data);
		}

		BOOL RdTsc(OUT DWORD64 & Data)
		{
			return ZhaoxinDriver::Instance()->RdTsc(Data);
		}

		BOOL WrMsr(IN DWORD Index, IN DWORD64 Data)
		{
			return ZhaoxinDriver::Instance()->WrMsr(Index, Data);
		}

		BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG & Memory_Data)
		{
			return ZhaoxinDriver::Instance()->RdMemory(Memory_Addr, Mem_DataSize, Memory_Data);
		}

		BOOL WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
		{
			return ZhaoxinDriver::Instance()->WrMemory(Memory_Addr, Mem_DataSize, Memory_Data);
		}

		BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG & pci_config)
		{
			return ZhaoxinDriver::Instance()->ReadPci(bus, dev, func, pci_config);
		}

		BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data)
		{
			return ZhaoxinDriver::Instance()->WritePci(bus, dev, func, offset, Data);
		}

		BOOL GetECData(Embedded_Controller_Data & EC_Data)
		{
			return ZhaoxinDriver::Instance()->GetECData(EC_Data);
		}

		BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data)
		{
			return ZhaoxinDriver::Instance()->SetECData(EC_Addr, EC_Write_Data);
		}

		BOOL ReadSMbusByByte(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, BYTE & data)
		{
			return ZhaoxinDriver::Instance()->ReadSMbusByByte(SmbusBase, SlaveAddress, offset, data);
		}

		BOOL ReadSMbusByWord(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, WORD & data)
		{
			return ZhaoxinDriver::Instance()->ReadSMbusByWord(SmbusBase, SlaveAddress, offset, data);
		}
		const std::vector<USHORT> Scan_SMBUS_DEVICE(const USHORT SmbusBase)
		{
			std::vector<USHORT> re = {};
			for (USHORT i = 0; i <= 0xFE; i+=2)
			{
				BYTE data = 0;
				if (ZhaoxinDriver::Instance()->ReadSMbusByByte(SmbusBase, i, 0, data))
				{
					re.emplace_back(i);
				}
			}
			return re;
		}

		void WriteSMbusByByte(const USHORT SmbusBase, const USHORT SlaveAddress, const USHORT offset, const DWORD data)
		{
			return ZhaoxinDriver::Instance()->WriteSMbusByByte(SmbusBase, SlaveAddress, offset, data);
		}

		const USHORT GetPCIVendorID()
		{
			return ZhaoxinDriver::Instance()->GetPCIVendorID();
		}

		const Pci_All_Config_Space & GetAllPciInfo()
		{
			return ZhaoxinDriver::Instance()->GetAllPciInfo();
		}

		BOOL GetSMbusBaseAddr(const USHORT VendorID, USHORT& SMbusBaseAddress)
		{
			BOOL Success = FALSE;
			ULONG tmp = 0;
			PCI_COMMON_CONFIG pci = {};
			switch (VendorID)
			{
			case 0x8086:
				if (ZhaoxinDriver::Instance()->ReadPci(0, 31, 3, pci) == 0)
				{
					tmp = pci.u.type0.BaseAddresses[4];
					SMbusBaseAddress = (USHORT)((tmp & 0xFFFE));
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
