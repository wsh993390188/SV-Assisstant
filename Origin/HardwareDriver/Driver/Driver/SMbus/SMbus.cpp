#include "SMbus.h"

BOOL GetSMbusBaseAddr(const USHORT VendorID, const PCI_COMMON_CONFIG pci, USHORT& SMbusBaseAddress)
{
	BOOL Success = FALSE;
	ULONG tmp = 0;
	switch (VendorID)
	{
	case 0x8086:
		tmp = pci.u.type0.BaseAddresses[4];
		SMbusBaseAddress = (USHORT)((tmp & 0x0000FFE) >> 5);
		Success = TRUE;
		break;
	case 0x1106:
		SMbusBaseAddress = 0x400;
		Success = TRUE;
		break;
	case 0x1002:
		Success = TRUE;
		break;
	default:
		break;
	}
	return Success;
}
