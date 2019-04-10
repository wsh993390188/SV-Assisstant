#pragma region Include Header
#include "stdafx.h"
#include "zhaoxin.h"
#include  <math.h>
#include "..\driver\Driverdll.h"
#pragma endregion

#pragma region CPUID Offset
//L1 Cache and TLB Information CPUID = 0x80000005
//EAX
#define INSTRUCTION_TLB_SIZE				0x000000FF
#define INSTRUCTION_TLB_ASSOC				0x0000FF00
#define DATA_TLB_SIZE						0x00FF0000
#define DATA_TLB_ASSOC						0xFF000000
//EBX
#define KB4_INSTRUCTION_TLB_SIZE			0x000000FF
#define KB4_INSTRUCTION_TLB_ASSOC			0x0000FF00
#define KB4_DATA_TLB_SIZE					0x00FF0000
#define KB4_DATA_TLB_ASSOC					0xFF000000
//ECX
#define L1_DATA_CACHE_SIZE					0xFF000000
#define L1_DATA_CACHE_ASSOC					0x00FF0000
#define L1_DATA_CACHE_LINE_PER_TAG			0x0000FF00
#define L1_DATA_CACHE_LINE_SIZE				0x000000FF
//EDX
#define L1_INSTRUCTION_CACHE_SIZE			0xFF000000
#define L1_INSTRUCTION_CACHE_ASSOC			0x00FF0000
#define L1_INSTRUCTION_CACHE_LINE_PER_TAG	0x0000FF00
#define L1_INSTRUCTION_CACHE_LINE_SIZE		0x000000FF

//L2 Cache and TLB and L3 Cache Information CPUID = 0x80000006
//EAX
#define L2_INSTRUCTION_TLB_ASSOC			0x0000F000
#define L2_INSTRUCTION_TLB_SIZE				0x00000FFF
#define L2_DATA_TLB_SIZE					0x0FFF0000
#define L2_DATA_TLB_ASSOC					0xF0000000
//EBX
#define KB4_L2_INSTRUCTION_TLB_SIZE			0x00000FFF
#define KB4_L2_INSTRUCTION_TLB_ASSOC		0x0000F000
#define KB4_L2_DATA_TLB_SIZE				0x0FFF0000
#define KB4_L2_DATA_TLB_ASSOC				0xF0000000
//ECX
#define L2_DATA_CACHE_SIZE					0xFFFF0000
#define L2_DATA_CACHE_ASSOC					0x0000F000
#define L2_DATA_CACHE_LINE_PER_TAG			0x00000F00
#define L2_DATA_CACHE_LINE_SIZE				0x000000FF
//EDX
#define L3_INSTRUCTION_CACHE_SIZE			0xFFFC0000
#define L3_INSTRUCTION_CACHE_ASSOC			0x0000F000
#define L3_INSTRUCTION_CACHE_LINE_PER_TAG	0x00000F00
#define L3_INSTRUCTION_CACHE_LINE_SIZE		0x000000FF
#pragma endregion

#pragma region Initialize
Zhaoxin::Zhaoxin() : f1_ecx{ 0 }, f1_edx{ 0 }, f7_ecx{ 0 }, f7_ebx{ 0 }, f81_ecx{ 0 }, f81_edx{ 0 }
, Zhaoxin_Temperature{ 0x1423 }, Zhaoxin_FIDVID{ 0x198 }, Zhaoxin_Target{ 0x199 }
{
}

Zhaoxin::~Zhaoxin()
{
}

void Zhaoxin::Init(void)
{
	std::array<int, 4> cpuid__;
	__cpuidex(cpuid__.data(), 0, 0);
	nIds_ = cpuid__[0];
	__cpuid(cpuid__.data(), 0x80000000);
	nExIds_ = cpuid__[0];
	for (int i = 0; i <= nIds_; i++)
	{
		int sub = 0;
		__cpuidex(cpuid__.data(), i, sub);
		data__.emplace_back(cpuid__);
		nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | sub);
		switch (i)
		{
		case 4:
			while (true)
			{
				__cpuidex(cpuid__.data(), i, ++sub);
				if ((cpuid__[0] & 0xf) == 0)
					break;
				else
				{
					data__.emplace_back(cpuid__);
					nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | sub);
				}
			}
			break;
		case 0x7:
			sub = cpuid__[0];
			for (int j = 1; j < sub; j++)
			{
				__cpuidex(cpuid__.data(), i, j);
				data__.emplace_back(cpuid__);
				nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | j);
			}
			break;
		case 0x0B:
			while (true)
			{
				__cpuidex(cpuid__.data(), i, ++sub);
				if ((cpuid__[2] & 0xff00) == 0)
					break;
				else
				{
					data__.emplace_back(cpuid__);
					nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | sub);
				}
			}
			break;
		case 0x0D:
			while (true)
			{
				__cpuidex(cpuid__.data(), i, ++sub);
				if (!cpuid__[0] && !cpuid__[1] && !cpuid__[2] && !cpuid__[3])
					break;
				else
				{
					data__.emplace_back(cpuid__);
					nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | sub);
				}
			}
			break;
		case 0x0F:
			__cpuidex(cpuid__.data(), i, 1);
			data__.emplace_back(cpuid__);
			nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | 1);
		case 0x10:
			for (int j = 1; j < 4; j++)
			{
				__cpuidex(cpuid__.data(), i, j);
				data__.emplace_back(cpuid__);
				nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | j);
			}
			break;
		case 0x12:
			__cpuidex(cpuid__.data(), i, ++sub);
			data__.emplace_back(cpuid__);
			nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | sub);
			while (true)
			{
				__cpuidex(cpuid__.data(), i, ++sub);
				if ((cpuid__[0] & 0xF) == 0x001B)
				{
					data__.emplace_back(cpuid__);
					nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | sub);
				}
				else
					break;

			}
			break;
		case 0x14:
			__cpuidex(cpuid__.data(), i, ++sub);
			data__.emplace_back(cpuid__);
			nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | sub);
			break;
		case 0x17:
			sub = cpuid__[0];
			for (int j = 1; j < sub; j++)
			{
				;
				__cpuidex(cpuid__.data(), i, j);
				data__.emplace_back(cpuid__);
				nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | j);
			}
			break;
		case 0x18:
			while (true)
			{
				__cpuidex(cpuid__.data(), i, ++sub);
				if (cpuid__[0] || (cpuid__[3] & 0xF))
					break;
				else
				{
					data__.emplace_back(cpuid__);
					nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000) | sub);
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0x80000000; i <= nExIds_; i++)
	{
		__cpuid(cpuid__.data(), i);
		extdata__.emplace_back(cpuid__);
	}
	char vendor[0x20];
	memset(vendor, 0, 0x20);
	*reinterpret_cast<int*>(vendor) = data__[0][1];
	*reinterpret_cast<int*>(vendor + 4) = data__[0][3];
	*reinterpret_cast<int*>(vendor + 8) = data__[0][2];
	Manufacturer = vendor;
	if (nIds_ > 1)
	{
		Family = (data__[1][0] & CPUID_Family) >> 8;
		Model = (data__[1][0] & CPUID_Model) >> 4;
		Stepping = (data__[1][0] & CPUID_Stepping) >> 0;
		f1_ecx = data__[1][2];
		f1_edx = data__[1][3];
	}
	if (nIds_ > 7)
	{
		size_t t = CPUBASE::findcpuid(0x00070000);
		if (t != -1)
		{
			f7_ecx = data__[t][2];
			f7_ebx = data__[t][1];
		}
	}
	if (nExIds_ > 0x80000001)
	{
		f81_ecx = extdata__[1][2];
		f81_edx = extdata__[1][3];
	}
	// Interpret CPU brand string if reported  
	if (nExIds_ >= 0x80000004)
	{
		char brand[0x40] = {};
		memcpy(brand, extdata__[2].data(), sizeof(std::array<int, 4>));
		memcpy(brand + 16, extdata__[3].data(), sizeof(std::array<int, 4>));
		memcpy(brand + 32, extdata__[4].data(), sizeof(std::array<int, 4>));
		Brand = brand;
		boost::trim(Brand);
	}
	ExecFeature();

	ExecCache();
}
#pragma endregion

#pragma region Update
void Zhaoxin::UpDateData(void)
{
	GetBusSpeed();
	GetCurrentSpeed();
	ExecVoltage();
	ExecTemperature();
}
#pragma endregion

#pragma region Feature
void Zhaoxin::ExecFeature()
{
	Feature.ZhaoxinFeature.SSE3 = f1_ecx[0];
	Feature.ZhaoxinFeature.MMWAIT = f1_ecx[3];
	Feature.ZhaoxinFeature.VMX = f1_ecx[5];
	Feature.ZhaoxinFeature.EPS = f1_ecx[7];
	Feature.ZhaoxinFeature.TM2 = f1_ecx[8];
	Feature.ZhaoxinFeature.SSSE3 = f1_ecx[9];
	Feature.ZhaoxinFeature.CMPXCHG16B = f1_ecx[13];
	Feature.ZhaoxinFeature.EchoTPR = f1_ecx[14];
	Feature.ZhaoxinFeature.SSE4_1 = f1_ecx[19];
	Feature.ZhaoxinFeature.SSE4_2 = f1_ecx[20];

	Feature.ZhaoxinFeature.FPU = f1_edx[0];
	Feature.ZhaoxinFeature.VME = f1_edx[1];
	Feature.ZhaoxinFeature.DE = f1_edx[2];
	Feature.ZhaoxinFeature.PSE = f1_edx[3];
	Feature.ZhaoxinFeature.TSC = f1_edx[4];
	Feature.ZhaoxinFeature.MSR = f1_edx[5];
	Feature.ZhaoxinFeature.PAE = f1_edx[6];
	Feature.ZhaoxinFeature.MCE = f1_edx[7];
	Feature.ZhaoxinFeature.CX8 = f1_edx[8];
	Feature.ZhaoxinFeature.APIC = f1_edx[9];
	Feature.ZhaoxinFeature.SEP = f1_edx[11];
	Feature.ZhaoxinFeature.MTRR = f1_edx[12];
	Feature.ZhaoxinFeature.PGE = f1_edx[13];
	Feature.ZhaoxinFeature.CMOV = f1_edx[15];
	Feature.ZhaoxinFeature.PAT = f1_edx[16];
	Feature.ZhaoxinFeature.CLFSH = f1_edx[19];
	Feature.ZhaoxinFeature.ACPI = f1_edx[22];
	Feature.ZhaoxinFeature.MMX = f1_edx[23];
	Feature.ZhaoxinFeature.FXSR = f1_edx[24];
	Feature.ZhaoxinFeature.SSE = f1_edx[25];
	Feature.ZhaoxinFeature.SSE2 = f1_edx[26];
	Feature.ZhaoxinFeature.SS = f1_edx[27];
	Feature.ZhaoxinFeature.TM = f1_edx[29];
	Feature.ZhaoxinFeature.PBE = f1_edx[31];

}
#pragma endregion

#pragma region Cache
void Zhaoxin::ExecCache()
{
	Cache[0].Cache_level = 1;
	Cache[0].Cache_type = 'D';
	Cache[0].Cache_line_size = ((extdata__[5][2] & L1_DATA_CACHE_LINE_SIZE) >> 0);
	Cache[0].Cache_Ways = ((extdata__[5][2] & L1_DATA_CACHE_ASSOC) >> 16);
	Cache[0].Cache_Size = ((extdata__[5][2] & L1_DATA_CACHE_SIZE) >> 24);

	Cache[1].Cache_level = 1;
	Cache[1].Cache_type = 'T';
	Cache[1].Cache_line_size = ((extdata__[5][3] & L1_DATA_CACHE_LINE_SIZE) >> 0);
	Cache[1].Cache_Ways = ((extdata__[5][3] & L1_DATA_CACHE_ASSOC) >> 16);
	Cache[1].Cache_Size = ((extdata__[5][3] & L1_DATA_CACHE_SIZE) >> 24);

	Cache[2].Cache_level = 2;
	Cache[2].Cache_type = 'D';
	Cache[2].Cache_line_size = ((extdata__[6][2] & L2_DATA_CACHE_LINE_SIZE) >> 0);
	Cache[2].Cache_Ways = (unsigned int)pow(2, ((extdata__[6][2] & L2_DATA_CACHE_ASSOC) >> 12) / 2);
	//Cache[2].Cache_Ways = ((extdata__[6][2] & L2_DATA_CACHE_ASSOC) >> 12);
	Cache[2].Cache_Size = ((extdata__[6][2] & L2_DATA_CACHE_SIZE) >> 16);

	Cache[3].Cache_level = 3;
	Cache[3].Cache_type = 'D';
	Cache[3].Cache_line_size = ((extdata__[6][3] & L3_INSTRUCTION_CACHE_LINE_SIZE) >> 0);
	Cache[3].Cache_Ways = (unsigned int)pow(2, ((extdata__[6][3] & L3_INSTRUCTION_CACHE_ASSOC) >> 12) / 2);
	//Cache[3].Cache_Ways = ((extdata__[6][3] & L3_INSTRUCTION_CACHE_ASSOC) >> 12);
	Cache[3].Cache_Size = ((extdata__[6][3] & L3_INSTRUCTION_CACHE_SIZE) >> 18);
}
#pragma endregion

#pragma region Voltage
void Zhaoxin::ExecVoltage()
{
	double VCore = INFINITY;
	DWORD64 msrdata = 0;
	for (DWORD threadAffinityMask = 0; threadAffinityMask < Core; threadAffinityMask++)
	{
		if (SV_ASSIST::Ring0::RdMsrTx(Zhaoxin_FIDVID, msrdata, threadAffinityMask) == 0)
		{
			BYTE VID = (msrdata & 0xFF);
			VCore = 1.5 - VID * 0.0125;
			if (VCore < 0.0)
				VCore = INFINITY;
			CoreVID[threadAffinityMask] = VCore;
		}
	}
}

#pragma endregion

#pragma region Temperature
void Zhaoxin::ExecTemperature()
{
	DWORD64 msrdata = 0;
	for (DWORD threadAffinityMask = 0; threadAffinityMask < Core; threadAffinityMask++)
	{
		if (SV_ASSIST::Ring0::RdMsrTx(Zhaoxin_Temperature, msrdata, threadAffinityMask) == 0)
		{
			auto temperature = (int)msrdata * 1.0;
			Temperature[threadAffinityMask] = temperature;
		}
	}

}
#pragma endregion

#pragma region Frequency
void Zhaoxin::GetBusSpeed()
{
	DWORD64 data = 0;
	if (SV_ASSIST::Ring0::RdMsr(0xCD, data) == 0)
	{
		switch (data & 0x07)
		{
		case 0b101:
			BusSpeed = 100.0;
			break;
		case 0b001:
			BusSpeed = 133.33;
			break;
		case 0b011:
			BusSpeed = 166.67;
			break;
		case 0b010:
			BusSpeed = 200.0;
			break;
		case 0b000:
			BusSpeed = 266.67;
			break;
		case 0b100:
			BusSpeed = 333.33;
			break;
		case 0b110:
			BusSpeed = 400.0;
			break;
		default:
			break;
		}
	}
}

void Zhaoxin::GetCurrentSpeed()
{
	DWORD64 msrdata = 0;
	if (BusSpeed != INFINITY)
	{
		for (DWORD threadAffinityMask = 0; threadAffinityMask < this->Core; threadAffinityMask++)
		{
			if (SV_ASSIST::Ring0::RdMsrTx(Zhaoxin_FIDVID, msrdata, threadAffinityMask) == 0)
			{
				CurrentClockSpeed[threadAffinityMask] = (((DWORD)msrdata >> 8) & 0xFF) / 2 * BusSpeed;
			}
		}
	}
}
#pragma endregion