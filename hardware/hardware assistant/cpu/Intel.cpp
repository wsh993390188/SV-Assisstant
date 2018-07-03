#include "stdafx.h"
#include "Intel.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
using namespace std;
Intel::Intel() :f1_ecx{ 0 }, f1_edx{ 0 }, f7_ebx{ 0 }, f7_ecx{ 0 }
{
}

Intel::~Intel()
{
}

void Intel::Init(void)
{
	array<int, 4> cpuid__;
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
			for(int j = 1; j < sub; j++)
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
//		nIdsLeaf.emplace_back((i << 16) | (i & 0xFFFF0000));
	}
}

void Intel::UpDateData(void)
{
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
		ExtFamily = ((data__[1][0] & CPUID_Extended_Family) >> 20) + Family;
		ExtModel = (((data__[1][0] & CPUID_Extended_Model) >> 16) << 4) + Model;
		f1_ecx = data__[1][2];
		f1_edx = data__[1][3];

		int DisplayFamily, DisplayModel;
		if (this->Family != 0x0F)		DisplayFamily = this->Family;
		else	DisplayFamily = this->ExtFamily;
		if (this->Family == 0x06 || this->Family == 0x0F)	DisplayModel = this->ExtModel;
		else	DisplayModel = this->Model;
		this->ExecProcessorOtherInfo(DisplayFamily, DisplayModel);
	}
	if (nIds_ > 7)
	{
		size_t t = findcpuid(0x00070000);
		if (t != -1)
		{
			f7_ecx = data__[t][2];
			f7_ebx = data__[t][1];
		}
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

void Intel::ExecFeature()
{
	Feature.IntelFeature.SSE3 = f1_ecx[0];
	Feature.IntelFeature.PCLMULQDQ = f1_ecx[1];
	Feature.IntelFeature.DTES64 = f1_ecx[2];
	Feature.IntelFeature.MONITOR = f1_ecx[3];
	Feature.IntelFeature.DS_CPL = f1_ecx[4];
	Feature.IntelFeature.VMX = f1_ecx[5];
	Feature.IntelFeature.SMX = f1_ecx[6];
	Feature.IntelFeature.EIST = f1_ecx[7];
	Feature.IntelFeature.TM2 = f1_ecx[8];
	Feature.IntelFeature.SSSE3 = f1_ecx[9];
	Feature.IntelFeature.CNXT_ID = f1_ecx[10];
	Feature.IntelFeature.SDBG = f1_ecx[11];
	Feature.IntelFeature.FMA = f1_ecx[12];
	Feature.IntelFeature.CMPXCHG16B = f1_ecx[13];
	Feature.IntelFeature.xTPR_Update_Control = f1_ecx[14];
	Feature.IntelFeature.PDCM = f1_ecx[15];
	Feature.IntelFeature.PCID = f1_ecx[17];
	Feature.IntelFeature.DCA = f1_ecx[18];
	Feature.IntelFeature.SSE4_1 = f1_ecx[19];
	Feature.IntelFeature.SSE4_2 = f1_ecx[20];
	Feature.IntelFeature.x2APIC = f1_ecx[21];
	Feature.IntelFeature.MOVBE = f1_ecx[22];
	Feature.IntelFeature.POPCNT = f1_ecx[23];
	Feature.IntelFeature.TSC_Deadline = f1_ecx[24];
	Feature.IntelFeature.AESNI = f1_ecx[25];
	Feature.IntelFeature.XSAVE = f1_ecx[26];
	Feature.IntelFeature.OSXSAVE = f1_ecx[27];
	Feature.IntelFeature.AVX = f1_ecx[28];
	Feature.IntelFeature.F16C = f1_ecx[29];
	Feature.IntelFeature.RDRAND = f1_ecx[30];

	Feature.IntelFeature.FPU = f1_edx[0];
	Feature.IntelFeature.VME = f1_edx[1];
	Feature.IntelFeature.DE = f1_edx[2];
	Feature.IntelFeature.PSE = f1_edx[3];
	Feature.IntelFeature.TSC = f1_edx[4];
	Feature.IntelFeature.MSR = f1_edx[5];
	Feature.IntelFeature.PAE = f1_edx[6];
	Feature.IntelFeature.MCE = f1_edx[7];
	Feature.IntelFeature.CX8 = f1_edx[8];
	Feature.IntelFeature.APIC = f1_edx[9];
	Feature.IntelFeature.SEP = f1_edx[11];
	Feature.IntelFeature.MTRR = f1_edx[12];
	Feature.IntelFeature.PGE = f1_edx[13];
	Feature.IntelFeature.MCA = f1_edx[14];
	Feature.IntelFeature.CMOV = f1_edx[15];
	Feature.IntelFeature.PAT = f1_edx[16];
	Feature.IntelFeature.PSE_36 = f1_edx[17];
	Feature.IntelFeature.PSN = f1_edx[18];
	Feature.IntelFeature.CLFSH = f1_edx[19];
	Feature.IntelFeature.DS = f1_edx[21];
	Feature.IntelFeature.ACPI = f1_edx[22];
	Feature.IntelFeature.MMX = f1_edx[23];
	Feature.IntelFeature.FXSR = f1_edx[24];
	Feature.IntelFeature.SSE = f1_edx[25];
	Feature.IntelFeature.SSE2 = f1_edx[26];
	Feature.IntelFeature.SS = f1_edx[27];
	Feature.IntelFeature.HTT = f1_edx[28];
	Feature.IntelFeature.TM = f1_edx[29];
	Feature.IntelFeature.PBE = f1_edx[31];

	Feature.IntelFeature.FSGSBASE = f7_ebx[0];
	Feature.IntelFeature.IA32_TSC_ADJUST_MSR = f7_ebx[1];
	Feature.IntelFeature.SGX = f7_ebx[2];
	Feature.IntelFeature.BMI1 = f7_ebx[3];
	Feature.IntelFeature.HLE = f7_ebx[4];
	Feature.IntelFeature.AVX2 = f7_ebx[5];
	Feature.IntelFeature.FDP_EXCPTN_ONLY = f7_ebx[6];
	Feature.IntelFeature.SMEP = f7_ebx[7];
	Feature.IntelFeature.BMI2 = f7_ebx[8];
	Feature.IntelFeature.Enhanced_REP_MOVSB_OR_STOSB = f7_ebx[9];
	Feature.IntelFeature.INVPCID = f7_ebx[10];
	Feature.IntelFeature.RTM = f7_ebx[11];
	Feature.IntelFeature.RDT_M = f7_ebx[12];
	Feature.IntelFeature.FPU_CS_DS = f7_ebx[13];
	Feature.IntelFeature.MPX = f7_ebx[14];
	Feature.IntelFeature.RDT_A = f7_ebx[15];
	Feature.IntelFeature.RDSEED = f7_ebx[18];
	Feature.IntelFeature.ADX = f7_ebx[19];
	Feature.IntelFeature.SMAP = f7_ebx[20];
	Feature.IntelFeature.CLFLUSHOPT = f7_ebx[23];
	Feature.IntelFeature.CLWB = f7_ebx[24];
	Feature.IntelFeature.Intel_Processor_Trace = f7_ebx[25];
	Feature.IntelFeature.SHA = f7_ebx[29];

	Feature.IntelFeature.PREFETCHWT1 = f7_ecx[0];
	Feature.IntelFeature.UMIP = f7_ecx[2];
	Feature.IntelFeature.PKU= f7_ecx[3];
	Feature.IntelFeature.OSPKE = f7_ecx[4];
	Feature.IntelFeature.RDPID = f7_ecx[22];
	Feature.IntelFeature.SGX_LC = f7_ecx[30];
}

void Intel::ExecCache()
{
	for (int i = 0; i < 4; i++)
	{
		size_t t = findcpuid(0x00040000 | i);
		if (t != -1)
		{
			switch (data__[t][0] & 0x1F)
			{
			case 1:
				Cache[i].Cache_type = 'D';
				break;
			case 2:
				Cache[i].Cache_type = 'T';
				break;
			case 3:
				Cache[i].Cache_type = 'U';
				break;
			default:
				break;
			}
			Cache[i].Cache_level = (data__[t][0] & CACHE_LEVEL) >> 5;
			Cache[i].Cache_Ways =((data__[t][1] & CACHE_WAYS) >> 22) + 1;
			Cache[i].Cache_partitions = ((data__[t][1] & CACHE_PARTITIONS) >> 12) + 1;
			Cache[i].Cache_line_size = (data__[t][1] & CACHE_LINE_SIZE) + 1;
			Cache[i].Cache_Set = data__[t][2] + 1;
			Cache[i].Cache_Size = Cache[i].Cache_Ways * Cache[i].Cache_partitions * Cache[i].Cache_line_size * Cache[i].Cache_Set;
		}
	}
}

bool Intel::ExecProcessorOtherInfo(int Family, int Model)
{
	bool state = true;
	switch (Family) {
	case 0x06: {
		switch (Model) {
		case 0x0F: // Intel Core 2 (65nm)
			Technology = 65;
			microarchitecture = "Core";
			break;
		case 0x17: // Intel Core 2 (45nm)
			Technology = 45;
			microarchitecture = "Core";
			break;
		case 0x1C: // Intel Atom (45nm)
			Technology = 45;
			microarchitecture = "Atom";
			break;
		case 0x1F: // Intel Core i5, i7 
			microarchitecture = "Nehalem";
			break;
		case 0x1A: // Intel Core i7 LGA1366 (45nm)
		case 0x1E: // Intel Core i5, i7 LGA1156 (45nm)
		case 0x2E: // Intel Xeon Processor 7500 series (45nm)
			Technology = 45;
			microarchitecture = "Nehalem";
			break;
		case 0x25: // Intel Core i3, i5, i7 LGA1156 (32nm)
		case 0x2C: // Intel Core i7 LGA1366 (32nm) 6 Core
		case 0x2F: // Intel Xeon Processor (32nm)
			Technology = 32;
			microarchitecture = "Nehalem";
			break;
		case 0x2A: // Intel Core i5, i7 2xxx LGA1155 (32nm)
		case 0x2D: // Next Generation Intel Xeon, i7 3xxx LGA2011 (32nm)
			Technology = 32;
			microarchitecture = "SandyBridge";
			break;
		case 0x3A: // Intel Core i5, i7 3xxx LGA1155 (22nm)
		case 0x3E: // Intel Core i7 4xxx LGA2011 (22nm)
			Technology = 22;
			microarchitecture = "IvyBridge";
			break;
		case 0x3C: // Intel Core i5, i7 4xxx LGA1150 (22nm)              
		case 0x3F: // Intel Xeon E5-2600/1600 v3, Core i7-59xx
				   // LGA2011-v3, Haswell-E (22nm)
		case 0x45: // Intel Core i5, i7 4xxxU (22nm)
		case 0x46:
			Technology = 22;
			microarchitecture = "Haswell";
			break;
		case 0x3D: // Intel Core M-5xxx (14nm)
		case 0x47: // Intel i5, i7 5xxx, Xeon E3-1200 v4 (14nm)
			Technology = 14;
			microarchitecture = "Broadwell";
			break;
		case 0x4F: // Intel Xeon E5-26xx v4
		case 0x56: // Intel Xeon D-15xx
			microarchitecture = "Broadwell";
			break;
		case 0x36: // Intel Atom S1xxx, D2xxx, N2xxx (32nm)
			Technology = 32;
			microarchitecture = "Atom";
			break;
		case 0x37: // Intel Atom E3xxx, Z3xxx (22nm)
		case 0x4D: // Intel Atom C2xxx (22nm)
			Technology = 22;
			microarchitecture = "Silvermont";
			break;
		case 0x4A:
		case 0x5A:
		case 0x5D:
			microarchitecture = "Silvermont";
			break;
		case 0x4E:
		case 0x5E: // Intel Core i5, i7 6xxxx LGA1151 (14nm)
		case 0x55: // Intel Core i7, i9 7xxxx LGA2066 (14nm)
			Technology = 14;
			microarchitecture = "Skylake";
			break;
		case 0x4C:
			microarchitecture = "Airmont";
			break;
		case 0x8E:
		case 0x9E: // Intel Core i5, i7 7xxxx (14nm)
			Technology = 14;
			microarchitecture = "KabyLake";
			break;
		case 0x5C: // Intel Atom processors
			microarchitecture = "ApolloLake";
			break;
		default:
			microarchitecture = "Unknown";
			break;
		}
	} break;
	case 0x0F: {
		switch (Model) {
		case 0x00: // Pentium 4 (180nm)
			Technology = 180;
			microarchitecture = "NetBurst";
			break;
		case 0x01: // Pentium 4 (130nm)
		case 0x02: // Pentium 4 (130nm)
			Technology = 130;
			microarchitecture = "NetBurst";
			break;
		case 0x03: // Pentium 4, Celeron D (90nm)
		case 0x04: // Pentium 4, Pentium D, Celeron D (90nm)
			Technology = 90;
			microarchitecture = "NetBurst";
			break;
		case 0x06: // Pentium 4, Pentium D, Celeron D (65nm)
			Technology = 65;
			microarchitecture = "NetBurst";
			break;
		default:
			microarchitecture = "Unknown";
			break;
		}
	} break;
	default:
		microarchitecture = "Unknown";
		break;
	}
	return state;
}
inline size_t Intel::findcpuid(int value)
{
	auto& it = find(nIdsLeaf.begin(), nIdsLeaf.end(), value);
	if (it != nIdsLeaf.end())
	{
		return distance(nIdsLeaf.begin(), it);
	}
	return -1;
}


