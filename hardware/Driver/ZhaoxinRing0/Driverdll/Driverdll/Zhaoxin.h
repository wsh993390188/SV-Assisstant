#pragma once
#include <memory>
// #include <mutex>
// #include <thread>
#include "MyDriverClass.h"

class ZhaoxinDriver
{
public:
	static BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD64& IO_Data);
	static BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data);
	static BOOL RdMsr(IN DWORD Index, OUT DWORD64& Data);
	static BOOL WrMsr(IN DWORD Index, IN DWORD64 Data);
	static BOOL RdMsrTx(IN DWORD Index, OUT DWORD64& Data, IN DWORD_PTR threadAffinityMask);
	static BOOL WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD_PTR threadAffinityMask);
	static BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data);
	static BOOL WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data);
	static BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG& pci_config);
	static BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data);
	static BOOL ReadSPD(USHORT Base_Address, USHORT Slave_Address, DDR3_INFO & data);
	static BOOL GetECData(Embedded_Controller_Data& EC_Data);
	static BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data);
	static USHORT GetPCIVendorID();
private:
	//static std::mutex ring0_mutex;
	static std::tr1::shared_ptr<CRing0> ring;
};
//mutex ZhaoxinDriver::ring0_mutex;

