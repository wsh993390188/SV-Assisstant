#include "stdafx.h"
#include "Intel.h"
#include <algorithm>

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
	}
	if (nIds_ > 7)
	{
		int t = findcpuid(0x00070000);
		if (t != -1)
		{
			f7_ecx = data__[t][2];
			f7_ebx = data__[t][1];
		}
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
		int t = findcpuid(0x00040000 | i);
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

inline int Intel::findcpuid(int value)
{
	auto& it = find(nIdsLeaf.begin(), nIdsLeaf.end(), value);
	if (it != nIdsLeaf.end())
	{
		return distance(nIdsLeaf.begin(), it);
	}
	return -1;
}


