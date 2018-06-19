// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DRIVERDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// DRIVERDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef DRIVERDLL_EXPORTS
#define DRIVERDLL_API __declspec(dllexport)
#else
#define DRIVERDLL_API __declspec(dllimport)
#endif

#include <windows.h>
#include "Ring0Defination.h"

DRIVERDLL_API BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD64& IO_Data);
DRIVERDLL_API BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data);
DRIVERDLL_API BOOL RdMsrTx(IN DWORD Index, OUT DWORD64& Data, IN DWORD_PTR threadAffinityMask);
DRIVERDLL_API BOOL WrMsrTx(IN DWORD Index, IN DWORD64 Data, IN DWORD_PTR threadAffinityMask);
DRIVERDLL_API BOOL RdMsr(IN DWORD Index, OUT DWORD64& Data);
DRIVERDLL_API BOOL WrMsr(IN DWORD Index, IN DWORD64 Data);
DRIVERDLL_API BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data);
DRIVERDLL_API BOOL WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data);
DRIVERDLL_API BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG& pci_config);
DRIVERDLL_API BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data);
DRIVERDLL_API BOOL ReadSPD(USHORT Base_Address, USHORT Slave_Address, DDR3_INFO & data);
DRIVERDLL_API BOOL GetECData(Embedded_Controller_Data& EC_Data);
DRIVERDLL_API BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data);
DRIVERDLL_API BOOL GetSMbusBaseAddr(const USHORT VendorID,USHORT& SMbusBaseAddress);
DRIVERDLL_API USHORT GetPCIVendorID();