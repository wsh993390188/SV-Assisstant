#include "stdafx.h"
#include "AMD.h"
#include "..\..\Zhaoxin.h"

DRIVERDLL_API BOOL SV_ASSIST::CPU::AMD_DRIVER::FIDVID_Family17(IN DWORD_PTR threadAffinityMask, IN ULONG Index, OUT double& COF, OUT double& CpuIdd, OUT ULONG& CpuVID)
{
	DWORD64 msrdata = 0;
	USHORT CpuFID = 0;
	double CpuDfsId = 0;
	DWORD IddValue = 0, IddDiv = 0;
	if(ZhaoxinDriver::RdMsrTx(Index, msrdata, threadAffinityMask) && msrdata)
	{
		CpuFID = (msrdata & 0xFF);
		CpuVID = (msrdata & 0x3FC000) >> 14;
		CpuDfsId = ((DWORD)msrdata & 0x3F00) >> 8;
		IddValue = (msrdata & 0x3FC00000) >> 22;
		IddDiv = (msrdata & 0xC0000000) >> 30;
		switch (IddDiv)
		{
		case 0:
			IddDiv = 1;
			break;
		case 1:
			IddDiv = 10;
			break;
		case 2:
			IddDiv = 100;
			break;
		default:
			IddDiv = 0;
			break;
		}
		if(IddDiv)
			CpuIdd = ((IddValue * 1.0) / (IddDiv * 1.0));
		switch ((ULONG)CpuDfsId)
		{
		case 0x08:
			break;
		case 0x09:
			CpuDfsId = CpuDfsId / 1.125;
			break;
		default:
			CpuDfsId = CpuDfsId / 8;
			break;
		}
		if (CpuDfsId == 0)
			COF = 0;
		else
			COF = CpuFID / CpuDfsId * 200;
	}
	return FALSE;
}
