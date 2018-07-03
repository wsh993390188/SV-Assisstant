#include "stdafx.h"
#include "ZhaoxinMSR.h"
#include "..\..\Zhaoxin.h"

const DWORD Zhaoxin_Temperature = 0x1423;
const DWORD Zhaoxin_FIDVID = 0x198;
const DWORD Zhaoxin_Target = 0x199;

DRIVERDLL_API BOOL SV_ASSIST::CPU::ZHAOXINMSR::ZX_Temperature(IN DWORD threadAffinityMask, OUT int& temperature)
{
	DWORD64 msrdata = 0;
	if (ZhaoxinDriver::Instance()->RdMsrTx(Zhaoxin_Temperature, msrdata, threadAffinityMask) && msrdata)
	{
		temperature = (int)msrdata;
		return TRUE;
	}
	return FALSE;
}

DRIVERDLL_API BOOL SV_ASSIST::CPU::ZHAOXINMSR::ZX_FIDVID(IN DWORD threadAffinityMask, OUT DWORD& FID, OUT DWORD& VID)
{
	DWORD64 msrdata = 0;
	if (ZhaoxinDriver::Instance()->RdMsrTx(Zhaoxin_FIDVID, msrdata, threadAffinityMask) && msrdata)
	{
		VID = (msrdata & 0xFF);
		FID = (msrdata & 0xFF00) >> 8;
		return TRUE;
	}
	return FALSE;
}

DRIVERDLL_API BOOL SV_ASSIST::CPU::ZHAOXINMSR::ZX_TargetFIDVID(IN DWORD threadAffinityMask, OUT DWORD& FID, OUT DWORD& VID)
{
	DWORD64 msrdata = 0;
	if (ZhaoxinDriver::Instance()->RdMsrTx(Zhaoxin_Target, msrdata, threadAffinityMask) && msrdata)
	{
		VID = (msrdata & 0xFF);
		FID = (msrdata & 0xFF00) >> 8;
		return TRUE;
	}
	return FALSE;
}
