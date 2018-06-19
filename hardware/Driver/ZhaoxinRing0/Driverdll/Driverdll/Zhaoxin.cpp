//#include "stdafx.h"
#include "Zhaoxin.h"

shared_ptr<CRing0> ZhaoxinDriver::ring = make_shared<CRing0>();

BOOL ZhaoxinDriver::RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD64 & IO_Data)
{
//	ring0_mutex.lock();
	return ring->RdIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
//	ring0_mutex.unlock();
}

BOOL ZhaoxinDriver::WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data)
{
	return ring->WrIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
}

BOOL ZhaoxinDriver::RdMsr(IN DWORD Index, OUT DWORD64 & Data)
{
	return ring->RdMsr(Index, Data);
}

BOOL ZhaoxinDriver::WrMsr(IN DWORD Index, IN DWORD64 Data)
{
	return ring->WrMsr(Index, Data);
}

BOOL ZhaoxinDriver::RdMsrTx(IN DWORD Index, OUT DWORD64 & Data, IN DWORD_PTR threadAffinityMask)
{
	BOOL		result = FALSE;
	DWORD_PTR	mask = 0;
	HANDLE		hThread = NULL;

	hThread = GetCurrentThread();
	mask = SetThreadAffinityMask(hThread, threadAffinityMask);

	if (mask == 0)
	{
		return FALSE;
	}

	return ring->RdMsr(Index, Data);
}

BOOL ZhaoxinDriver::WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD_PTR threadAffinityMask)
{
	BOOL		result = FALSE;
	DWORD_PTR	mask = 0;
	HANDLE		hThread = NULL;

	hThread = GetCurrentThread();
	mask = SetThreadAffinityMask(hThread, threadAffinityMask);
	if (mask == 0)
	{
		return FALSE;
	}

	return ring->WrMsr(Index, Data);
}

BOOL ZhaoxinDriver::RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG & Memory_Data)
{
	return ring->RdMemory(Memory_Addr, Mem_DataSize, Memory_Data);
}

BOOL ZhaoxinDriver::WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
{
	return ring->WrMemory(Memory_Addr, Mem_DataSize, Memory_Data);
}

BOOL ZhaoxinDriver::ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG & pci_config)
{
	return ring->ReadPci(bus, dev, func, pci_config);
}

BOOL ZhaoxinDriver::WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data)
{
	return ring->WritePci(bus, dev, func, offset, Data);
}

BOOL ZhaoxinDriver::ReadSPD(USHORT Base_Address, USHORT Slave_Address, DDR3_INFO & data)
{
	return ring->ReadSPD(Base_Address, Slave_Address, data);
}

BOOL ZhaoxinDriver::GetECData(Embedded_Controller_Data & EC_Data)
{
	return ring->GetECData(EC_Data);
}

BOOL ZhaoxinDriver::SetECData(BYTE EC_Addr, BYTE EC_Write_Data)
{
	return ring->SetECData(EC_Addr, EC_Write_Data);
}

USHORT ZhaoxinDriver::GetPCIVendorID()
{
	return ring->VendorID;
}
