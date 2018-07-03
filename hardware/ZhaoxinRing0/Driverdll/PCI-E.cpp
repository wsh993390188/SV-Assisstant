#include "stdafx.h"
#include "PCI-E.h"
#include "Zhaoxin.h"

#define MK_PCIEADDR(bus, dev, func, off) (ULONG)(((bus & 0xFF) << 20) | (dev & 0x1F) << 15 | (func & 0x07) << 12 | off)

BOOL GetPCIEConfigSpace(const ULONGLONG PCIE_BaseAddress, ULONG bus, ULONG dev, ULONG func, PVOID*& Data, size_t DataSize)
{
	if(DataSize < 4096)	return FALSE;
	ULONG tmp = 0;
	for (USHORT offset = 0; offset < 0x100; offset += 4)
	{
		ZhaoxinDriver::Instance()->RdMemory(PCIE_BaseAddress + MK_PCIEADDR(bus, dev, func, offset), 4, tmp);
		memcpy(*Data, &tmp, 4);
	}
	return TRUE;
}