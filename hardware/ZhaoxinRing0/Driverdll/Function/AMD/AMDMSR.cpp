#include "stdafx.h"
#include "AMDMSR.h"
#include "..\..\Zhaoxin.h"

const DWORD AMD_PSTATE[]
{
	0xC0010064,
	0xC0010065,
	0xC0010066,
	0xC0010067,
	0xC0010068,
	0xC0010069,
	0xC001006A,
	0xC001006B
};

const DWORD AMD_PState_Status = 0xC0010063;
const DWORD AMD_CurrentState = 0xC0010293;

DRIVERDLL_API BOOL SV_ASSIST::CPU::AMD_DRIVER::FIDVID_Family17(IN DWORD threadAffinityMask, IN ULONG Index, OUT double& COF, OUT double& CpuIdd, OUT ULONG& CpuVID)
{
	DWORD64 msrdata = 0;
	USHORT CpuFID = 0;
	double CpuDfsId = 0;
	DWORD IddValue = 0, IddDiv = 0;
	if(ZhaoxinDriver::Instance()->RdMsrTx(AMD_PSTATE[Index], msrdata, threadAffinityMask) && msrdata)
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
