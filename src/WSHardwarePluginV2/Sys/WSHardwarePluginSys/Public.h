/*++

Module Name:

	public.h

Abstract:

	This module contains the common declarations shared by driver
	and user applications.

Environment:

	user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//
#pragma once
#include <initguid.h>

DEFINE_GUID(GUID_DEVINTERFACE_LenovoRing0,
	0x5fd82ae1, 0xef4d, 0x4975, 0xa6, 0x53, 0xb2, 0xf9, 0x8d, 0x9c, 0x2d, 0x9c);
// {5fd82ae1-ef4d-4975-a653-b2f98d9c2d9c}

// Bus Number, Device Number and Function Number to PCI Device Address
#define PciBusDevFunc(Bus, Dev, Func)	((Bus&0xFF)<<8) | ((Dev&0x1F)<<3) | (Func&7)

enum class Ring0OperatorType : ULONG
{
	None,
	RDMSR,
	WRMSR,
	RDIO,
	WRIO,
	RDMEMORY,
	WRMEMORY,
	RDPCI,
	WRPCI,
	RDTSC
};

#pragma pack(push,4)
struct MemoryData
{
	LARGE_INTEGER Address;
	ULONG UnitSize;
	/* value to write if write requet
	ignored if read request */
#ifdef _WIN64
	ULONG64 Value;
#else
	ULONG Value;
#endif // _WIN64
};

struct IoData
{
	USHORT Address;
	USHORT UnitSize;
	/* value to write if write requet
	ignored if read request */
#ifdef _WIN64
	ULONG64 Value;
#else
	ULONG Value;
#endif // _WIN64
};

struct MsrData
{
	ULONG Address;
	ULONG64 Value;     /* value to write if write requet
							 ignored if read request */
};

struct PCIData
{
	ULONG PciAddress;
	USHORT PciOffset;
	USHORT UnitSize;
	ULONG Value;
};

struct Ring0Operator
{
	Ring0OperatorType OperatorType;
	union
	{
		MemoryData memory;
		MsrData msr;
		IoData io;
		PCIData pci;
	};
};

struct Ring0Output
{
	union
	{
		ULONG64 Value;
	};
};
#pragma pack(pop)