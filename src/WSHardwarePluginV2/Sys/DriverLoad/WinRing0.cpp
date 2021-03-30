#include "stdafx.h"
#include "WinRing0.h"
#include "..\WSHardwarePluginSys\Public.h"
#include <winioctl.h>
#include "..\WSHardwarePluginSys\ioctl.h"
#include "DriverLoad.h"

Hardware::WinRing0& Hardware::WinRing0::Instance()
{
	static WinRing0 inst;
	return inst;
}

Hardware::WinRing0::WinRing0() : hdevice(GetGlobalHandle())
{
	StartRing0();
}

Hardware::WinRing0::~WinRing0()
{
	ExitRing0();
}

BOOL Hardware::WinRing0::RdIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, OUT DWORD& IO_Data)
{
	if (hdevice == INVALID_HANDLE_VALUE)
		return FALSE;
	Ring0Output nOutput;
	DWORD Results;
	Ring0Operator Operator;
	Operator.OperatorType = Ring0OperatorType::RDIO;
	Operator.io.Address = IO_Port_Addr;
	Operator.io.UnitSize = IO_DataSize;
	Operator.io.Value = 0;
	if ((Operator.io.UnitSize == 1) || (Operator.io.UnitSize == 2) || (Operator.io.UnitSize == 4))
	{
		if (!DeviceIoControl(hdevice,
			IOCTL_RING0_OPERATOR,
			&Operator,
			sizeof(Operator),
			&nOutput,
			sizeof(nOutput),
			&Results,
			NULL)
			)
		{
			return FALSE;
		}
		IO_Data = nOutput.Value;
		return TRUE;
	}
	return FALSE;
}

BOOL Hardware::WinRing0::WrIOPort(IN USHORT IO_Port_Addr, IN USHORT IO_DataSize, IN ULONG IO_Data)
{
	if (hdevice == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD Results;
	Ring0Operator Operator;
	Operator.OperatorType = Ring0OperatorType::WRIO;
	Operator.io.Address = IO_Port_Addr;
	Operator.io.UnitSize = IO_DataSize;
	Operator.io.Value = IO_Data;
	if ((Operator.io.UnitSize == 1) || (Operator.io.UnitSize == 2) || (Operator.io.UnitSize == 4))
	{
		if (!DeviceIoControl(hdevice,
			IOCTL_RING0_OPERATOR,
			&Operator,
			sizeof(Operator),
			nullptr,
			0,
			&Results,
			NULL)
			)
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL Hardware::WinRing0::RdMsr(IN DWORD Index, OUT DWORD64& Data)
{
	if (hdevice == INVALID_HANDLE_VALUE)
		return FALSE;
	Ring0Output nOutput;
	DWORD Results;
	Ring0Operator Operator;
	Operator.OperatorType = Ring0OperatorType::RDMSR;
	Operator.msr.Address = Index;
	Operator.msr.Value = Data;

	if (!DeviceIoControl(hdevice,
		IOCTL_RING0_OPERATOR,
		&Operator,
		sizeof(Operator),
		&nOutput,
		sizeof(nOutput),
		&Results,
		NULL)
		)
	{
		return FALSE;
	}
	Data = nOutput.Value;
	return TRUE;
}

BOOL Hardware::WinRing0::WrMsr(IN DWORD Index, IN DWORD64 Data)
{
	if (hdevice == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD Results;
	Ring0Operator Operator;
	Operator.OperatorType = Ring0OperatorType::WRMSR;
	Operator.msr.Address = Index;
	Operator.msr.Value = Data;

	if (!DeviceIoControl(hdevice,
		IOCTL_RING0_OPERATOR,
		&Operator,
		sizeof(Operator),
		nullptr,
		0,
		&Results,
		NULL)
		)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL Hardware::WinRing0::RdTsc(OUT DWORD64& Data)
{
	if (hdevice == INVALID_HANDLE_VALUE)
		return FALSE;
	Ring0Output nOutput;
	DWORD Results;
	Ring0Operator Operator;
	Operator.OperatorType = Ring0OperatorType::RDTSC;

	if (!DeviceIoControl(hdevice,
		IOCTL_RING0_OPERATOR,
		&Operator,
		sizeof(Operator),
		&nOutput,
		sizeof(nOutput),
		&Results,
		NULL)
		)
	{
		return FALSE;
	}
	Data = nOutput.Value;
	return TRUE;
}

BOOL Hardware::WinRing0::RdMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, OUT ULONG& Memory_Data)
{
	if (hdevice == INVALID_HANDLE_VALUE)
		return FALSE;
	Ring0Output nOutput;
	DWORD Results;
	Ring0Operator Operator;
	Operator.OperatorType = Ring0OperatorType::RDMEMORY;
	Operator.memory.Address.QuadPart = Memory_Addr;
	Operator.memory.UnitSize = Mem_DataSize;
	Operator.memory.Value = 0;
	if ((Operator.memory.UnitSize == 1) || (Operator.memory.UnitSize == 2) || (Operator.memory.UnitSize == 4))
	{
		if (!DeviceIoControl(hdevice,
			IOCTL_RING0_OPERATOR,
			&Operator,
			sizeof(Operator),
			&nOutput,
			sizeof(nOutput),
			&Results,
			NULL)
			)
		{
			return FALSE;
		}
		Memory_Data = nOutput.Value;
		return TRUE;
	}
	return FALSE;
}

BOOL Hardware::WinRing0::WrMemory(IN LONGLONG Memory_Addr, IN USHORT Mem_DataSize, IN ULONG Memory_Data)
{
	if (hdevice == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD Results;
	Ring0Operator Operator;
	Operator.OperatorType = Ring0OperatorType::WRMEMORY;
	Operator.memory.Address.QuadPart = Memory_Addr;
	Operator.memory.UnitSize = Mem_DataSize;
	Operator.memory.Value = Memory_Data;
	if ((Operator.memory.UnitSize == 1) || (Operator.memory.UnitSize == 2) || (Operator.memory.UnitSize == 4))
	{
		if (!DeviceIoControl(hdevice,
			IOCTL_RING0_OPERATOR,
			&Operator,
			sizeof(Operator),
			nullptr,
			0,
			&Results,
			NULL)
			)
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL Hardware::WinRing0::ReadPci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN USHORT offset, IN USHORT DataSize, OUT DWORD& pci_config)
{
	if (hdevice == INVALID_HANDLE_VALUE)
		return FALSE;
	Ring0Output nOutput;
	DWORD Results;
	Ring0Operator Operator;
	Operator.OperatorType = Ring0OperatorType::RDPCI;
	Operator.pci.PciAddress = PciBusDevFunc(bus, dev, func);
	Operator.pci.PciOffset = offset;
	Operator.pci.UnitSize = DataSize;
	Operator.pci.Value = 0;
	if (!DeviceIoControl(hdevice,
		IOCTL_RING0_OPERATOR,
		&Operator,
		sizeof(Operator),
		&nOutput,
		sizeof(nOutput),
		&Results,
		NULL)
		)
	{
		return FALSE;
	}
	pci_config = nOutput.Value;
	return TRUE;
}

BOOL Hardware::WinRing0::WritePci(IN USHORT bus, IN USHORT dev, IN USHORT func, IN UCHAR offset, IN USHORT DataSize, IN ULONG Data)
{
	if (hdevice == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD Results;
	Ring0Operator Operator;
	Operator.OperatorType = Ring0OperatorType::WRPCI;
	Operator.pci.PciAddress = PciBusDevFunc(bus, dev, func);
	Operator.pci.PciOffset = offset;
	Operator.pci.UnitSize = DataSize;
	Operator.pci.Value = Data;
	if (!DeviceIoControl(hdevice,
		IOCTL_RING0_OPERATOR,
		&Operator,
		sizeof(Operator),
		nullptr,
		0,
		&Results,
		NULL)
		)
	{
		return FALSE;
	}
	return TRUE;
}