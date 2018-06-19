//#include "stdafx.h"
#include "AMD.h"

BOOL Family17(USHORT Family, USHORT Model, ULONG Index, double& COF, ULONG& CpuVID, IN DWORD_PTR threadAffinityMask)
{
	CRing0 ring0;
	DWORD64 msrdata = 0;
	USHORT CpuFID = 0;
	double CpuDfsId = 0;
	ring0.RdMsr(Index, msrdata);
	/*if (msrdata & 0x8000000000000000)
	{
		CpuFID = (msrdata & 0xFF);
		CpuVID = (msrdata & 0x3FC000) >> 14;
		CpuDfsId = (msrdata & 0x3F00) >> 8;
		switch (CpuDfsId)
		{
		case 0x08:
			CpuDfsId = 1 / 1;
			break;
		case 0x09:
			CpuDfsId = 1 / 1.125;
			break;
		default:
			CpuDfsId = CpuDfsId / 8;
			break;
		}
		COF = CpuFID * 25 / CpuDfsId;
		return TRUE;
	}
	else
		COF = 0;
		return FALSE;*/
	CpuFID = (msrdata & 0xFF);
	CpuVID = (msrdata & 0x3FC000) >> 14;
	CpuDfsId = ((DWORD)msrdata & 0x3F00) >> 8;
	switch ((ULONG)CpuDfsId)
	{
	case 0x08:
		CpuDfsId = 1 / 1;
		break;
	case 0x09:
		CpuDfsId = 1 / 1.125;
		break;
	default:
		CpuDfsId = CpuDfsId / 8;
		break;
	}
	if (CpuDfsId == 0)
	{
		COF = 0;
		return FALSE;
	}
	COF = CpuFID * 25 / CpuDfsId;
	return TRUE;
}
