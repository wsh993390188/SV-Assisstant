#include "stdafx.h"
#include "DriverOrigin.h"

std::shared_ptr<ZhaoxinDriver> ZhaoxinDriver::ring0_temp = nullptr;

ZhaoxinDriver::ZhaoxinDriver() : ring(make_shared<CRing0>()), ring0_mutex{}
{}

ZhaoxinDriver * ZhaoxinDriver::Instance()
{
	if (!ring0_temp.get())
	{
		ring0_temp = make_shared<ZhaoxinDriver>();
	}
	return ring0_temp.get();
}

BOOL ZhaoxinDriver::RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD & IO_Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->RdIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
}

BOOL ZhaoxinDriver::WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->WrIOPort(IO_Port_Addr, IO_DataSize, IO_Data);
}

BOOL ZhaoxinDriver::RdMsr(IN DWORD Index, OUT DWORD64 & Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->RdMsr(Index, Data);
}

BOOL ZhaoxinDriver::WrMsr(IN DWORD Index, IN DWORD64 Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->WrMsr(Index, Data);
}

BOOL ZhaoxinDriver::RdMsrTx(IN DWORD Index, OUT DWORD64 & Data, IN DWORD threadAffinityMask)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->RdMsrTx(Index, threadAffinityMask, Data);
}

BOOL ZhaoxinDriver::WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD threadAffinityMask)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->WrMsrTx(Index, threadAffinityMask, Data);
}

BOOL ZhaoxinDriver::RdMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG & Memory_Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->RdMemory(Memory_Addr, Mem_DataSize, Memory_Data);
}

BOOL ZhaoxinDriver::WrMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	auto state = ring->WrMemory(Memory_Addr, Mem_DataSize, Memory_Data);
	Sleep(1);
	return state;
}

BOOL ZhaoxinDriver::ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG & pci_config)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->ReadPci(bus, dev, func, pci_config);
}

BOOL ZhaoxinDriver::WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data)
{
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->WritePci(bus, dev, func, offset, Data);
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
	std::lock_guard<std::mutex> lock(ring0_mutex);
	return ring->VendorID;
}

const Pci_All_Config_Space& ZhaoxinDriver::GetAllPciInfo()
{
	return ring->ReturnPCIConfigSpace();
}

