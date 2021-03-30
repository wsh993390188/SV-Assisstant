#include "stdafx.h"
#include "HardwareRing0.h"
#ifdef COOLite
#include "..\..\Libcommon\CooLite\OlsApi.h"
#pragma comment(lib, "SystemThermalInvoker.lib")
#else
#ifdef RUN_TIME_DYNAMIC_LINKING
#include "WinRing0/OlsApiInit.h"
#else // for Load-Time Dynamic Linking
#include "WinRing0/OlsApi.h"
#ifdef _M_X64
#pragma comment(lib, "WinRing0/release/WinRing0x64.lib")
#else
#pragma comment(lib, "WinRing0/release/WinRing0.lib")
#endif
#endif
#endif

#define MK_PCIADDR(bus, dev, func, off) (ULONG)(0x80000000L|((bus & 0xFF) << 16) | (dev & 0x1F) << 11 | (func & 0x07) << 8 | off)
#define PciBusDevFunc(Bus, Dev, Func)	((Bus&0xFF)<<8) | ((Dev&0x1F)<<3) | (Func&7)

Hardware::Utils::Ring0::WinRing0& Hardware::Utils::Ring0::WinRing0::Instance()
{
	static WinRing0 inst;
	return inst;
}

Hardware::Utils::Ring0::WinRing0::WinRing0()
#ifdef RUN_TIME_DYNAMIC_LINKING
	: Hmodule(NULL)
#endif
{
#ifdef RUN_TIME_DYNAMIC_LINKING
	if (!InitOpenLibSys(&Hmodule))
	{
		auto status = GetDllStatus();
		auto DriverType = GetDriverType();
	}
#else
	InitializeOls();
#endif // RUN_TIME_DYNAMIC_LINKING
}

Hardware::Utils::Ring0::WinRing0::~WinRing0()
{
#ifdef RUN_TIME_DYNAMIC_LINKING
	DeinitOpenLibSys(&Hmodule);
#else
	DeinitializeOls();
#endif // RUN_TIME_DYNAMIC_LINKING
}

BOOL Hardware::Utils::Ring0::WinRing0::RdMsr(DWORD index, PDWORD eax, PDWORD edx)
{
#ifdef RUN_TIME_DYNAMIC_LINKING
	if (Hmodule == NULL)
	{
		return FALSE;
	}
#endif
	return ::Rdmsr(index, eax, edx);
}

BOOL Hardware::Utils::Ring0::WinRing0::WrMsr(DWORD index, DWORD eax, DWORD edx)
{
#ifdef RUN_TIME_DYNAMIC_LINKING
	if (Hmodule == NULL)
	{
		return FALSE;
	}
#endif
	return ::Wrmsr(index, eax, edx);
}

BOOL Hardware::Utils::Ring0::WinRing0::ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN USHORT offset, IN USHORT DataSize, OUT DWORD& pci_config)
{
#ifdef RUN_TIME_DYNAMIC_LINKING
	if (Hmodule == NULL)
	{
		return FALSE;
	}
#endif
	BOOL res = FALSE;
	switch (DataSize)
	{
	case 1:
		res = ::ReadPciConfigByteEx(PciBusDevFunc(bus, dev, func), offset, reinterpret_cast<PBYTE>(&pci_config));
		break;
	case 2:
		res = ::ReadPciConfigWordEx(PciBusDevFunc(bus, dev, func), offset, reinterpret_cast<PWORD>(&pci_config));
		break;
	case 4:
		res = ::ReadPciConfigDwordEx(PciBusDevFunc(bus, dev, func), offset, reinterpret_cast<PDWORD>(&pci_config));
		break;
	default:
		break;
	}
	return res;
}

BOOL Hardware::Utils::Ring0::WinRing0::WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN USHORT DataSize, IN ULONG Data)
{
#ifdef RUN_TIME_DYNAMIC_LINKING
	if (Hmodule == NULL)
	{
		return FALSE;
	}
#endif
	BOOL res = FALSE;
	switch (DataSize)
	{
	case 1:
		res = ::WritePciConfigByteEx(PciBusDevFunc(bus, dev, func), offset, static_cast<BYTE>(Data));
		break;
	case 2:
		res = ::WritePciConfigWordEx(PciBusDevFunc(bus, dev, func), offset, static_cast<WORD>(Data));
		break;
	case 4:
		res = ::WritePciConfigDwordEx(PciBusDevFunc(bus, dev, func), offset, static_cast<DWORD>(Data));
		break;
	default:
		break;
	}
	return res;
}

BOOL Hardware::Utils::Ring0::WinRing0::RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD& IO_Data)
{
#ifdef RUN_TIME_DYNAMIC_LINKING
	if (Hmodule == NULL)
	{
		return FALSE;
	}
#endif
	BOOL res = FALSE;
	switch (IO_DataSize)
	{
	case 1:
		res = ::ReadIoPortByteEx(IO_Port_Addr, reinterpret_cast<PBYTE>(&IO_Data));
		break;
	case 2:
		res = ::ReadIoPortWordEx(IO_Port_Addr, reinterpret_cast<PWORD>(&IO_Data));
		break;
	case 4:
		res = ::ReadIoPortDwordEx(IO_Port_Addr, reinterpret_cast<PDWORD>(&IO_Data));
		break;
	default:
		break;
	}
	return res;
}

BOOL Hardware::Utils::Ring0::WinRing0::WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data)
{
#ifdef RUN_TIME_DYNAMIC_LINKING
	if (Hmodule == NULL)
	{
		return FALSE;
	}
#endif
	BOOL res = FALSE;
	switch (IO_DataSize)
	{
	case 1:
		res = ::WriteIoPortByteEx(IO_Port_Addr, static_cast<BYTE>(IO_Data));
		break;
	case 2:
		res = ::WriteIoPortWordEx(IO_Port_Addr, static_cast<WORD>(IO_Data));
		break;
	case 4:
		res = ::WriteIoPortDwordEx(IO_Port_Addr, static_cast<DWORD>(IO_Data));
		break;
	default:
		break;
	}
	return res;
}

BOOL Hardware::Utils::Ring0::WinRing0::RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data)
{
#ifdef RUN_TIME_DYNAMIC_LINKING
	if (Hmodule == NULL)
	{
		return FALSE;
	}
#endif
	BOOL res = FALSE;
#ifdef COOLite
	switch (Mem_DataSize)
	{
	case 1:
		res = ::ReadMemoryByteEx((PBYTE)Memory_Addr, reinterpret_cast<PBYTE>(&Memory_Data));
		break;
	case 2:
		res = ::ReadMemoryWordEx((PBYTE)Memory_Addr, reinterpret_cast<PWORD>(&Memory_Data));
		break;
	case 4:
		res = ::ReadMemoryDwordEx((PBYTE)Memory_Addr, reinterpret_cast<PDWORD>(&Memory_Data));
		break;
	default:
		break;
	}
#endif // DEBUG

	return res;
}

BOOL Hardware::Utils::Ring0::WinRing0::WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
{
#ifdef RUN_TIME_DYNAMIC_LINKING
	if (Hmodule == NULL)
	{
		return FALSE;
	}
#endif
	BOOL res = FALSE;
#ifdef COOLite
	switch (Mem_DataSize)
	{
	case 1:
		res = ::WriteMemoryByteEx((PBYTE)Memory_Addr, static_cast<BYTE>(Memory_Data));
		break;
	case 2:
		res = ::WriteMemoryWordEx((PBYTE)Memory_Addr, static_cast<WORD>(Memory_Data));
		break;
	case 4:
		res = ::WriteMemoryDwordEx((PBYTE)Memory_Addr, static_cast<DWORD>(Memory_Data));
		break;
	default:
		break;
	}
#endif
	return res;
}

BOOL Hardware::Utils::Ring0::WinRing0::RdTsc(OUT DWORD64& Data)
{
	LARGE_INTEGER nFreq, nBeginTime, nEndTime;

	uint32_t dummy = 0;
	auto tsc1 = __rdtscp(&dummy);
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);
	Sleep(1);
	auto tsc2 = __rdtscp(&dummy);
	QueryPerformanceCounter(&nEndTime);
	auto time = (nEndTime.QuadPart - nBeginTime.QuadPart) / (nFreq.QuadPart * 1.0);
	Data = (tsc2 - tsc1) / time;
	return TRUE;
}