#pragma once
#include <windows.h>
#include "Ring0Defination.h"

namespace SV_ASSIST
{
    namespace Ring0
    {
        BOOL RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD64& IO_Data);
        BOOL WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data);
        BOOL RdMsr(IN DWORD Index, OUT DWORD64& Data, IN DWORD_PTR threadAffinityMask);
        BOOL WrMsr(IN DWORD Index, IN DWORD64 Data, IN DWORD_PTR threadAffinityMask);
        BOOL RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data);
        BOOL WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data);
        BOOL ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, OUT PCI_COMMON_CONFIG& pci_config);
        BOOL WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN ULONG Data);
        BOOL ReadSPD(USHORT Base_Address, USHORT Slave_Address, DDR3_INFO & data);
        BOOL GetECData(Embedded_Controller_Data& EC_Data);
        BOOL SetECData(BYTE EC_Addr, BYTE EC_Write_Data);
        BOOL GetSMbusBaseAddr(const USHORT VendorID, const PCI_COMMON_CONFIG pci, USHORT& SMbusBaseAddress);
    }
}
