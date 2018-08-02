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

DRIVERDLL_API BOOL SV_ASSIST::CPU::ZHAOXINMSR::ZX_CoreVoltage(IN DWORD threadAffinityMask, OUT float & VCore)
{
	DWORD64 msrdata = 0;
	if (ZhaoxinDriver::Instance()->RdMsrTx(Zhaoxin_FIDVID, msrdata, threadAffinityMask) && msrdata)
	{
		BYTE VID = (msrdata & 0xFF);
		switch (VID)
		{
		case 0b0000000:
			VCore = 1.5000f;
			break;		  
		case 0b0000001:	  
				VCore = 1.4875f;
			break;		  
		case 0b0000010:	  
				VCore = 1.4750f;
			break;		  
		case 0b0000011:	  
				VCore = 1.4625f;
			break;		  
		case 0b0000100:	  
				VCore = 1.4500f;
			break;		  
		case 0b0000101:	  
				VCore = 1.4375f;
			break;		  
		case 0b0000110:	  
				VCore = 1.4250f;
			break;		  
		case 0b0000111:	  
				VCore = 1.4125f;
			break;		  
		case 0b0001000:	  
				VCore = 1.4000f;
			break;		 
		case 0b0001001:	 
				VCore = 1.3875f;
			break;		  
		case 0b0001010:	  
				VCore = 1.3750f;
			break;		  
		case 0b0001011:	  
				VCore = 1.3625f;
			break;		  
		case 0b0001100:	  
				VCore = 1.3500f;
			break;		  
		case 0b0001101:	  
				VCore = 1.3375f;
			break;		  
		case 0b0001110:	  
				VCore = 1.3250f;
			break;		  
		case 0b0001111:	  
				VCore = 1.3125f;
			break;		  
		case 0b0010000:	  
				VCore = 1.3000f;
			break;		  
		case 0b0010001:	  
				VCore = 1.2875f;
			break;		  
		case 0b0010010:	  
				VCore = 1.2750f;
			break;		  
		case 0b0010011:	  
				VCore = 1.2625f;
			break;		  
		case 0b0010100:	  
				VCore = 1.2500f;
			break;		  
		case 0b0010101:	  
				VCore = 1.2375f;
			break;		  
		case 0b0010110:	  
				VCore = 1.2250f;
			break;		  
		case 0b0010111:	  
				VCore = 1.2125f;
			break;		  
		case 0b0011000:	  
				VCore = 1.2000f;
			break;		  
		case 0b0011001:	  
				VCore = 1.1875f;
			break;		  
		case 0b0011010:	  
				VCore = 1.1750f;
			break;		  
		case 0b0011011:	  
				VCore = 1.1625f;
			break;		  
		case 0b0011100:	  
				VCore = 1.1500f;
			break;		  
		case 0b0011101:	  
				VCore = 1.1375f;
			break;		  
		case 0b0011110:	  
				VCore = 1.1250f;
			break;		  
		case 0b0011111:	  
				VCore = 1.1125f;
			break;		  
		case 0b0100000:	  
				VCore = 1.1000f;
			break;		  
		case 0b0100001:	  
				VCore = 1.0875f;
			break;		  
		case 0b0100010:	  
				VCore = 1.0750f;
			break;		  
		case 0b0100011:	  
				VCore = 1.0625f;
			break;		  
		case 0b0100100:	  
				VCore = 1.0500f;
			break;		  
		case 0b0100101:	  
				VCore = 1.0375f;
			break;		  
		case 0b0100110:	  
				VCore = 1.0250f;
			break;		  
		case 0b0100111:	  
				VCore = 1.0125f;
			break;		  
		case 0b0101000:	  
				VCore = 1.000f; 
				break;		  
		case 0b0101001:	  
				VCore = 0.9875f;
			break;		  
		case 0b0101010:	  
				VCore = 0.9750f;
			break;		  
		case 0b0101011:	  
				VCore = 0.9625f;
			break;		  
		case 0b0101100:	  
				VCore = 0.9500f;
			break;		  
		case 0b0101101:	  
				VCore = 0.9375f;
			break;		  
		case 0b0101110:	  
				VCore = 0.9250f;
			break;		  
		case 0b0101111:	  
				VCore = 0.9125f;
			break;		  
		case 0b0110000:	  
				VCore = 0.9000f;
			break;		  
		case 0b0110001:	  
				VCore = 0.8875f;
			break;		  
		case 0b0110010:	  
				VCore = 0.8750f;
			break;		  
		case 0b0110011:	  
				VCore = 0.8625f;
			break;		  
		case 0b0110100:	  
				VCore = 0.8500f;
			break;		  
		case 0b0110101:	  
				VCore = 0.8375f;
			break;		  
		case 0b0110110:	  
				VCore = 0.8250f;
			break;		  
		case 0b0110111:	  
				VCore = 0.8125f;
			break;		  
		case 0b0111000:	  
				VCore = 0.8000f;
			break;		  
		case 0b0111001:	  
				VCore = 0.7875f;
			break;		  
		case 0b0111010:	  
				VCore = 0.7750f;
			break;		  
		case 0b0111011:	  
				VCore = 0.7625f;
			break;		  
		case 0b0111100:	  
				VCore = 0.7500f;
			break;		  
		case 0b0111101:	  
				VCore = 0.7375f;
			break;		  
		case 0b0111110:	  
				VCore = 0.7250f;
			break;		  
		case 0b0111111:	  
				VCore = 0.7125f;
			break;		  
		case 0b1000000:	  
				VCore = 0.7000f;
			break;		  
		case 0b1000001:	  
				VCore = 0.6875f;
			break;		  
		case 0b1000010:	  
				VCore = 0.6750f;
			break;		  
		case 0b1000011:	  
				VCore = 0.6625f;
			break;		  
		case 0b1000100:	  
				VCore = 0.6500f;
			break;		  
		case 0b1000101:	  
				VCore = 0.6375f;
			break;		  
		case 0b1000110:	  
				VCore = 0.6250f;
			break;		  
		case 0b1000111:	  
				VCore = 0.6125f;
			break;		  
		case 0b1001000:	  
				VCore = 0.6000f;
			break;		  
		case 0b1001001:	  
				VCore = 0.5875f;
			break;		  
		case 0b1001010:	  
				VCore = 0.5750f;
			break;		  
		case 0b1001011:	  
				VCore = 0.5625f;
			break;		  
		case 0b1001100:	  
				VCore = 0.5500f;
			break;		  
		case 0b1001101:	  
				VCore = 0.5375f;
			break;		  
		case 0b1001110:	  
				VCore = 0.5250f;
			break;		  
		case 0b1001111:	  
				VCore = 0.5125f;
			break;		  
		case 0b1010000:	  
				VCore = 0.5000f;
			break;		  
		case 0b1010001:	  
				VCore = 0.4875f;
			break;		  
		case 0b1010010:	 
				VCore = 0.4750f;
			break;		  
		case 0b1010011:	  
				VCore = 0.4625f;
			break;		  
		case 0b1010100:	  
				VCore = 0.4500f;
			break;		  
		case 0b1010101:	  
				VCore = 0.4375f;
			break;		  
		case 0b1010110:	  
				VCore = 0.4250f;
			break;		  
		case 0b1010111:	  
				VCore = 0.4125f;
			break;		  
		case 0b1011000:	  
				VCore = 0.4000f;
			break;		  
		case 0b1011001:	  
				VCore = 0.3875f;
			break;		  
		case 0b1011010:	  
				VCore = 0.3750f;
			break;		  
		case 0b1011011:	  
				VCore = 0.3625f;
			break;		  
		case 0b1011100:	  
				VCore = 0.3500f;
			break;		  
		case 0b1011101:	  
				VCore = 0.3375f;
			break;		  
		case 0b1011110:	  
				VCore = 0.3250f;
			break;		  
		case 0b1011111:	  
				VCore = 0.3125f;
			break;		  
		case 0b1100000:	  
				VCore = 0.3000f;
			break;		  
		case 0b1100001:	  
				VCore = 0.2875f;
			break;		  
		case 0b1100010:	  
				VCore = 0.2750f;
			break;		  
		case 0b1100011:	  
				VCore = 0.2625f;
			break;		  
		case 0b1100100:	  
				VCore = 0.2500f;
			break;		  
		case 0b1100101:	  
				VCore = 0.2375f;
			break;		  
		case 0b1100110:	  
				VCore = 0.2250f;
			break;		  
		case 0b1100111:	  
				VCore = 0.2125f;
			break;		  
		case 0b1101000:	  
				VCore = 0.2000f;
			break;		  
		case 0b1101001:	  
				VCore = 0.1875f;
			break;		  
		case 0b1101010:	  
				VCore = 0.1750f;
			break;		  
		case 0b1101011:	  
				VCore = 0.1625f;
			break;		  
		case 0b1101100:	  
				VCore = 0.1500f;
			break;		  
		case 0b1101101:	  
				VCore = 0.1375f;
			break;		  
		case 0b1101110:	  
				VCore = 0.1250f;
			break;		  
		case 0b1101111:	  
				VCore = 0.1125f;
			break;		  
		case 0b1110000:	  
				VCore = 0.1000f;
			break;		  
		case 0b1110001:	  
				VCore = 0.0875f;
			break;		  
		case 0b1110010:	  
				VCore = 0.0750f;
			break;		  
		case 0b1110011:	  
				VCore = 0.0625f;
			break;		  
		case 0b1110100:	  
				VCore = 0.0500f;
			break;		  
		case 0b1110101:	  
				VCore = 0.0375f;
			break;
		case 0b1110110:
			VCore = 0.0250f;
			break;
		case 0b1110111:
			VCore = 0.0125f;
			break;
		default:
			VCore = 0.0f;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
