#pragma once
#include <memory>
// #include <mutex>
// #include <thread>
#include "MyDriverClass.h"

typedef std::pair<std::map<int,std::string>, std::map<int, std::map<int, std::string>>> PCIDB;

class ZhaoxinDriver
{
public:
	ZhaoxinDriver();
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
	static const Pci_All_Config_Space& GetAllPciInfo();
	static BOOL GetPCIDeviceName(USHORT VenderID, USHORT DeviceID, std::string& VenderName, std::string& DeviceName);
private:
	static BOOL InitPciDB();
	//static std::mutex ring0_mutex;
	static PCIDB pciDB;
	static std::shared_ptr<CRing0> ring;
};
//mutex ZhaoxinDriver::ring0_mutex;

