#pragma once
#include <memory>
#include <mutex>
// #include <thread>
#include "MyDriverClass.h"

class ZhaoxinDriver
{
public:
	static ZhaoxinDriver* Instance();
	BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD& IO_Data);
	BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data);
	BOOL RdMsr(IN DWORD Index, OUT DWORD64& Data);
	BOOL WrMsr(IN DWORD Index, IN DWORD64 Data);
	BOOL RdMsrTx(IN DWORD Index, OUT DWORD64& Data, IN DWORD threadAffinityMask);
	BOOL WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD threadAffinityMask);
	BOOL RdMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT DWORD& Memory_Data);
	BOOL WrMemory(IN ULONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data);
	BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG& pci_config);
	BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data);
	BOOL GetECData(Embedded_Controller_Data& EC_Data);
	BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data);
	USHORT GetPCIVendorID();
	const Pci_All_Config_Space& GetAllPciInfo();
	ZhaoxinDriver();
private:
	static std::shared_ptr<ZhaoxinDriver> ring0_temp;
	std::mutex ring0_mutex;
	std::shared_ptr<CRing0> ring;
};
//mutex ZhaoxinDriver::ring0_mutex;

