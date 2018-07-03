#pragma once
#include <memory>
// #include <mutex>
// #include <thread>
#include "MyDriverClass.h"

typedef std::pair<std::map<int,std::string>, std::map<int, std::map<int, std::string>>> PCIDB;

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
	BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data);
	BOOL WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data);
	BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG& pci_config);
	BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data);
	BOOL ReadSPD(USHORT Base_Address, USHORT Slave_Address, DDR3_INFO & data);
	BOOL GetECData(Embedded_Controller_Data& EC_Data);
	BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data);
	USHORT GetPCIVendorID();
	const Pci_All_Config_Space& GetAllPciInfo();
	BOOL GetPCIDeviceName(USHORT VenderID, USHORT DeviceID, std::string& VenderName, std::string& DeviceName);
	ZhaoxinDriver();
private:
	BOOL InitPciDB();
	static std::shared_ptr<ZhaoxinDriver> ring0_temp;
	//static std::mutex ring0_mutex;
	PCIDB pciDB;
	std::shared_ptr<CRing0> ring;
};
//mutex ZhaoxinDriver::ring0_mutex;

