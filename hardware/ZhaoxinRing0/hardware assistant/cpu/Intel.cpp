#pragma region Include Header
#include "stdafx.h"
#include <algorithm>
#include "Intel.h"
#include "..\driver\Driverdll.h"
using namespace std;
#pragma endregion

#pragma region Initialize
Intel::Intel() :f1_ecx{ 0 }, f1_edx{ 0 }, f7_ebx{ 0 }, f7_ecx{ 0 }, DisplayFamily{ 0 }, DisplayModel{0},
IA32_THERM_STATUS_MSR{ 0x019C }, IA32_TEMPERATURE_TARGET{ 0x01A2 }, IA32_PERF_STATUS{ 0x198 }, MSR_PLATFORM_INFO{ 0xCE },
IA32_PACKAGE_THERM_STATUS{ 0x1B1 }, MSR_RAPL_POWER_UNIT{ 0x606 }, MSR_PKG_ENERY_STATUS{ 0x611 }, MSR_DRAM_ENERGY_STATUS{ 0x619 },
MSR_PP0_ENERY_STATUS{ 0x639 }, MSR_PP1_ENERY_STATUS{ 0x641 }, MaxNonTurboFre{0}
{
	this->Name += "Intel ";
	for (int i = 0; i < Core; i++)
	{
		Temperature.emplace_back(INFINITY);
		Tjmax.emplace_back(INFINITY);
	}
	this->GetTjMaxFromMSR();

	DWORD64 data = 0;
	if (SV_ASSIST::Ring0::RdMsr(MSR_PLATFORM_INFO, data) == 1)
	{
		MaxNonTurboFre = ((data & 0xFF00) >> 8) * ExtClock;
	}
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
	{

		char vendor[0x20];
		memset(vendor, 0, 0x20);
		*reinterpret_cast<int*>(vendor) = data__[0][1];
		*reinterpret_cast<int*>(vendor + 4) = data__[0][3];
		*reinterpret_cast<int*>(vendor + 8) = data__[0][2];
		Manufacturer = vendor;
	}
	{
		char brand[0x40] = {};
		memcpy(brand, extdata__[2].data(), sizeof(std::array<int, 4>));
		memcpy(brand + 16, extdata__[3].data(), sizeof(std::array<int, 4>));
		memcpy(brand + 32, extdata__[4].data(), sizeof(std::array<int, 4>));
		Brand = brand;
		boost::trim(Brand);
	}
	if (nIds_ > 1)
	{
		Family = (data__[1][0] & CPUID_Family) >> 8;
		Model = (data__[1][0] & CPUID_Model) >> 4;
		Stepping = (data__[1][0] & CPUID_Stepping) >> 0;
		ExtFamily = ((data__[1][0] & CPUID_Extended_Family) >> 20) + Family;
		ExtModel = (((data__[1][0] & CPUID_Extended_Model) >> 16) << 4) + Model;
		f1_ecx = data__[1][2];
		f1_edx = data__[1][3];

		if (this->Family != 0x0F)		DisplayFamily = this->Family;
		else	DisplayFamily = this->ExtFamily;
		if (this->Family == 0x06 || this->Family == 0x0F)	DisplayModel = this->ExtModel;
		else	DisplayModel = this->Model;
		this->ExecProcessorOtherInfo(DisplayFamily, DisplayModel);
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

	// Interpret CPU brand string if reported  

	ExecFeature();

	ExecCache();
}

#pragma endregion

#pragma region Update
void Intel::UpDateData(void)
{
	ExecTemperature();
	ExecVoltageByFMS();
	GetBusSpeed();
	GetCurrentSpeed();
}
#pragma endregion

#pragma region Exec
#pragma region Feature
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
#pragma endregion

#pragma region Cache
void Intel::ExecCache()
{
	for (int i = 0; i < 4; i++)
	{
		size_t t = CPUBASE::findcpuid(0x00040000 | i);
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
#pragma endregion

#pragma region SouthBridge CodeName Technology Package MaxTDP 
bool Intel::ExecProcessorOtherInfo(int Family, int Model)
{
	bool state = true;
	this->ExecSouthBridge();
	this->ExecCPUName();
	if (this->Name.find("Core") != string::npos)
	{
		if (this->Name.find("i9") != string::npos)
		{
			string Model1 = {};
			size_t Index = this->Name.find_last_of(" ");
			Model1 = Name.substr(Index + 1);
			if(CPUDB["Core i9"].count(Model1)) {microarchitecture = CPUDB["Core i9"][Model1].at(0); SocketDesignation = CPUDB["Core i9"][Model1].at(1); Technology = stoi(CPUDB["Core i9"][Model1].at(2)); MaxTDP = stoi(CPUDB["Core i9"][Model1].at(3)); }
			else ExecCodeNameByFMS(Family, Model);
		}
		else if (this->Name.find("i7") != string::npos)
		{
			string Model1 = {  };
			size_t Index = this->Name.find_last_of(" ");
			Model1 = Name.substr(Index + 1);
			if(CPUDB["Core i7"].count(Model1))
			{microarchitecture = CPUDB["Core i7"][Model1].at(0); SocketDesignation = CPUDB["Core i7"][Model1].at(1); Technology = stoi(CPUDB["Core i7"][Model1].at(2)); MaxTDP = stoi(CPUDB["Core i7"][Model1].at(3)); } 
			else ExecCodeNameByFMS(Family, Model);
		}
		else if (this->Name.find("i5") != string::npos)
		{
			string Model1 = {};
			size_t Index = this->Name.find_last_of(" ");
			Model1 = Name.substr(Index + 1);
			if (CPUDB["Core i5"].count(Model1)) {microarchitecture = CPUDB["Core i5"][Model1].at(0); SocketDesignation = CPUDB["Core i5"][Model1].at(1); Technology = stoi(CPUDB["Core i5"][Model1].at(2)); MaxTDP = stoi(CPUDB["Core i5"][Model1].at(3)); } 
			else ExecCodeNameByFMS(Family, Model);
		}
		else if (this->Name.find("i3") != string::npos)
		{
			string Model1 = {};
			size_t Index = this->Name.find_last_of(" ");
			Model1 = Name.substr(Index + 1);
			if (CPUDB["Core i3"].count(Model1)) {microarchitecture = CPUDB["Core i3"][Model1].at(0); SocketDesignation = CPUDB["Core i3"][Model1].at(1); Technology = stoi(CPUDB["Core i3"][Model1].at(2)); MaxTDP = stoi(CPUDB["Core i3"][Model1].at(3)); } 
			else ExecCodeNameByFMS(Family, Model);
		}
	}
	else if (this->Name.find("Pentium Silver") != string::npos)
	{
		string Model1 = {};
		size_t Index = this->Name.find_last_of(" ");
		Model1 = Name.substr(Index + 1);
		if (CPUDB["Pentium Silver"].count(Model1)) { microarchitecture = CPUDB["Pentium Silver"][Model1].at(0); SocketDesignation = CPUDB["Pentium Silver"][Model1].at(1); Technology = stoi(CPUDB["Pentium Silver"][Model1].at(2)); MaxTDP = stoi(CPUDB["Pentium Silver"][Model1].at(3)); }
		else ExecCodeNameByFMS(Family, Model);
	}
	else if (this->Name.find("Pentium Gold") != string::npos)
	{
		string Model1 = {};
		size_t Index = this->Name.find_last_of(" ");
		Model1 = Name.substr(Index + 1);
		if (CPUDB["Pentium Gold"].count(Model1)) { microarchitecture = CPUDB["Pentium Gold"][Model1].at(0); SocketDesignation = CPUDB["Pentium Gold"][Model1].at(1); Technology = stoi(CPUDB["Pentium Gold"][Model1].at(2)); MaxTDP = stoi(CPUDB["Pentium Gold"][Model1].at(3)); }
		else ExecCodeNameByFMS(Family, Model);
	}
	else if (this->Name.find("Pentium") != string::npos)
	{
		string Model1 = {};
		size_t Index = this->Name.find_last_of(" ");
		Model1 = Name.substr(Index + 1);
		if (CPUDB["Pentium"].count(Model1)) { microarchitecture = CPUDB["Pentium"][Model1].at(0); SocketDesignation = CPUDB["Pentium"][Model1].at(1); Technology = stoi(CPUDB["Pentium"][Model1].at(2)); MaxTDP = stoi(CPUDB["Pentium"][Model1].at(3)); }
		else ExecCodeNameByFMS(Family, Model);
	}
	else if (this->Name.find("Celeron") != string::npos)
	{
		string Model1 = {};
		size_t Index = this->Name.find_last_of(" ");
		Model1 = Name.substr(Index + 1);
		if (CPUDB["Celeron"].count(Model1)) { microarchitecture = CPUDB["Celeron"][Model1].at(0); SocketDesignation = CPUDB["Celeron"][Model1].at(1); Technology = stoi(CPUDB["Celeron"][Model1].at(2)); MaxTDP = stoi(CPUDB["Celeron"][Model1].at(3)); }
		else ExecCodeNameByFMS(Family, Model);
	}
	else if (this->Name.find("Atom") != string::npos)
	{
		string Model1 = {};
		size_t Index = this->Name.find_last_of(" ");
		Model1 = Name.substr(Index + 1);
		if (CPUDB["Atom"].count(Model1)) { microarchitecture = CPUDB["Atom"][Model1].at(0); SocketDesignation = CPUDB["Atom"][Model1].at(1); Technology = stoi(CPUDB["Atom"][Model1].at(2)); MaxTDP = stoi(CPUDB["Atom"][Model1].at(3)); }
		else ExecCodeNameByFMS(Family, Model);
	}
	else if (this->Name.find("Xeon") != string::npos)
	{
		if (this->Name.find("E-") != string::npos)
		{
			string Model1 = {};
			size_t Index = this->Name.find_last_of(" ");
			Model1 = Name.substr(Index + 1);
			if (CPUDB["Xeon E"].count(Model1)) { microarchitecture = CPUDB["Xeon E"][Model1].at(0); SocketDesignation = CPUDB["Xeon E"][Model1].at(1); Technology = stoi(CPUDB["Xeon E"][Model1].at(2)); MaxTDP = stoi(CPUDB["Xeon E"][Model1].at(3)); }
			else ExecCodeNameByFMS(Family, Model);
		}
	}
#pragma endregion

	return state;
}

bool Intel::ExecSouthBridge()
{
	PCI_COMMON_CONFIG pci = {};
	SV_ASSIST::Ring0::ReadPci(0, 31, 0, pci);

	switch (pci.DeviceID)
	{
	case 0x1c40:
	case 0x1c42:
	case 0x1c48:
	case 0x1c45:
	case 0x1c51:
	case 0x1c55:
	case 0x1c57:
	case 0x1c53:
	case 0x1c5b:
	case 0x1c5e:
	case 0x1c5f:
		SouthBridge = "C200";
		break;
	case 0x1d40:
	case 0x1D41:
		SouthBridge = "X79";
		break;
	case 0x1c44:
		SouthBridge = "Z68";
		break;
	case 0x1c46:
		SouthBridge = "P67";
		break;
	case 0x1c47:
		SouthBridge = "UM67";
		break;
	case 0x1c49:
		SouthBridge = "HM65";
		break;
	case 0x1c4a:
		SouthBridge = "H67";
		break;
	case 0x1c4b:
		SouthBridge = "HM67";
		break;
	case 0x1c4c:
		SouthBridge = "Q65";
		break;
	case 0x1c4d:
		SouthBridge = "QS67";
		break;
	case 0x1c4e:
		SouthBridge = "Q67";
		break;
	case 0x1c4f:
		SouthBridge = "QM67";
		break;
	case 0x1c50:
		SouthBridge = "B65";
		break;
	case 0x1c52:
		SouthBridge = "C202";
		break;
	case 0x1c54:
		SouthBridge = "C204";
		break;
	case 0x1c56:
		SouthBridge = "C206";
		break;
	case 0x1c58:
		SouthBridge = "Upgraded B65";
		break;
	case 0x1c59:
		SouthBridge = "Upgraded HM67";
		break;
	case 0x1c5a:
		SouthBridge = "Upgraded Q67";
		break;
	case 0x1c5c:
		SouthBridge = "H61";
		break;
	case 0x1e44:
		SouthBridge = "Z77";
		break;
	case 0x1e46:
		SouthBridge = "Z75";
		break;
	case 0x1e47:
		SouthBridge = "Q77";
		break;
	case 0x1e48:
		SouthBridge = "Q75";
		break;
	case 0x1e49:
		SouthBridge = "B75";
		break;
	case 0x1e4a:
		SouthBridge = "H77";
		break;
	case 0x1e53:
		SouthBridge = "C216";
		break;
	case 0x1e55:
		SouthBridge = "QM77";
		break;
	case 0x1e56:
		SouthBridge = "QS77";
		break;
	case 0x1e57:
		SouthBridge = "HM77";
		break;
	case 0x1e58:
		SouthBridge = "UM77";
		break;
	case 0x1e59:
		SouthBridge = "HM76";
		break;
	case 0x2310:
		SouthBridge = "DH89xxCC";
		break;
	case 0x2390:
		SouthBridge = "DH895XCC";
		break;
	case 0x2410:
		SouthBridge = "82801AA";
		break;
	case 0x2420:
		SouthBridge = "82801AB";
		break;
	case 0x2440:
		SouthBridge = "82801BA";
		break;
	case 0x244c:
		SouthBridge = "82801BAM";
		break;
	case 0x2450:
		SouthBridge = "82801E";
		break;
	case 0x2480:
		SouthBridge = "82801CA";
		break;
	case 0x248c:
		SouthBridge = "82801CAM";
		break;
	case 0x24c0:
		SouthBridge = "82801DB/DBL (ICH4/ICH4-L)";
		break;
	case 0x24cc:
		SouthBridge = "82801DBM (ICH4-M)";
		break;
	case 0x24d0:
		SouthBridge = "82801EB/ER (ICH5/ICH5R)";
		break;
	case 0x24dc:
		SouthBridge = "82801EB (ICH5)";
		break;
	case 0x25a1:
		SouthBridge = "6300ESB";
		break;
	case 0x2640:
		SouthBridge = "82801FB/FR (ICH6/ICH6R)";
		break;
	case 0x2641:
		SouthBridge = "82801FBM (ICH6M)";
		break;
	case 0x2642:
		SouthBridge = "82801FW/FRW (ICH6W/ICH6RW)";
		break;
	case 0x2670:
		SouthBridge = "631xESB/632xESB/3100";
		break;
	case 0x27b0:
		SouthBridge = "82801GH (ICH7DH)";
		break;
	case 0x27b8:
		SouthBridge = "82801GB (ICH7 Family)";
		break;
	case 0x27b9:
		SouthBridge = "82801GBM (ICH7-M)";
		break;
	case 0x27bc:
		SouthBridge = "NM10";
		break;
	case 0x27bd:
		SouthBridge = "82801GHM (ICH7-M DH)";
		break;
	case 0x2810:
		SouthBridge = "82801HB/HR (ICH8/R)";
		break;
	case 0x2811:
		SouthBridge = "82801HEM (ICH8M-E)";
		break;
	case 0x2812:
		SouthBridge = "82801HH (ICH8DH)";
		break;
	case 0x2814:
		SouthBridge = "82801HO (ICH8DO)";
		break;
	case 0x2815:
		SouthBridge = "82801HM (ICH8M)";
		break;
	case 0x2912:
		SouthBridge = "82801IH (ICH9DH)";
		break;
	case 0x2914:
		SouthBridge = "82801IO (ICH9DO)";
		break;
	case 0x2916:
		SouthBridge = "82801IR (ICH9R)";
		break;
	case 0x2917:
		SouthBridge = "ICH9M-E";
		break;
	case 0x2918:
		SouthBridge = "82801IB (ICH9)";
		break;
	case 0x2919:
		SouthBridge = "ICH9M";
		break;
	case 0x3482:
		SouthBridge = "Ice Lake-LP";
		break;
	case 0x3a14:
		SouthBridge = "82801JDO (ICH10DO)";
		break;
	case 0x3a16:
		SouthBridge = "82801JIR (ICH10R)";
		break;
	case 0x3a18:
		SouthBridge = "82801JIB (ICH10)";
		break;
	case 0x3a1a:
		SouthBridge = "82801JD (ICH10D)";
		break;
	case 0x3b02:
		SouthBridge = "P55";
		break;
	case 0x3b03:
		SouthBridge = "PM55";
		break;
	case 0x3b06:
		SouthBridge = "H55";
		break;
	case 0x3b07:
		SouthBridge = "QM57";
		break;
	case 0x3b09:
		SouthBridge = "HM55";
		break;
	case 0x3b08:
		SouthBridge = "H57";
		break;
	case 0x3b0b:
		SouthBridge = "HM57";
		break;
	case 0x3b0f:
		SouthBridge = "QS57";
		break;

	case 0x3b14:
		SouthBridge = "3420 Chipset";
		break;
	case 0x3b16:
		SouthBridge = "3450 Chipset";
		break;
	case 0x3b17:
	case 0x3b18:
	case 0x3b19:
	case 0x3b1a:
	case 0x3b1b:
	case 0x3b1c:
	case 0x3b1d:
	case 0x3b1e:
	case 0x3b1f:
	case 0x3b12:
		SouthBridge = "3400 Chipset";
		break;
	case 0x5031:
		SouthBridge = "EP80579";
		break;
	case 0x8119:
		SouthBridge = "SCH Poulsbo";
		break;
	case 0x8186:
		SouthBridge = "E6xx";
		break;
	case 0x8c43:
	case 0x8c45:
	case 0x8c47:
	case 0x8c48:
	case 0x8c4d:
	case 0x8c51:
	case 0x8c53:
	case 0x8c55:
	case 0x8c57:
	case 0x8c58:
	case 0x8c59:
	case 0x8c5a:
	case 0x8c5b:
	case 0x8c5d:
	case 0x8c5e:
	case 0x8c5f:
		SouthBridge = "C220";
		break;
	case 0x8C44:
		SouthBridge = "Z87";
		break;
	case 0x8C46:
		SouthBridge = "Z85";
		break;
	case 0x8C49:
		SouthBridge = "HM86";
		break;
	case 0x8C4a:
		SouthBridge = "H87";
		break;
	case 0x8C4B:
		SouthBridge = "HM87";
		break;
	case 0x8C4C:
		SouthBridge = "Q85";
		break;
	case 0x8C4E:
		SouthBridge = "Q87";
		break;
	case 0x8C4F:
		SouthBridge = "QM87";
		break;
	case 0x8c50:
		SouthBridge = "B85";
		break;
	case 0x8c5c:
		SouthBridge = "H81";
		break;
	case 0x8c52:
		SouthBridge = "C222";
		break;
	case 0x8c54:
		SouthBridge = "C224";
		break;
	case 0x8c56:
		SouthBridge = "C226";
		break;
	case 0x8cc3:
		SouthBridge = "HM97";
		break;
	case 0x8cc4:
		SouthBridge = "Z97";
		break;
	case 0x8d40:
	case 0x8d41:
	case 0x8d42:
	case 0x8d43:
	case 0x8d44:
	case 0x8d45:
	case 0x8d46:
	case 0x8d47:
	case 0x8d48:
	case 0x8d49:
	case 0x8d4a:
	case 0x8d4b:
	case 0x8d4c:
	case 0x8d4d:
	case 0x8d4e:
	case 0x8d4f:
		SouthBridge = "X99";
		break;
	case 0x9cc1:
	case 0x9cc2:
	case 0x9cc3:
	case 0x9cc5:
	case 0x9cc6:
	case 0x9cc7:
	case 0x9cc9:
		SouthBridge = "Wildcat Point-LP";
		break;
	case 0x9d43:
	case 0x9d48:
	case 0x9d56:
	case 0x9d58:
		SouthBridge = "Sunrise Point-LP";
		break;
	case 0x9d4e:
		SouthBridge = "Intel(R) 100 Series Chipset";
		break;
	case 0xa140:
	case 0xa141:
	case 0xa142:
	case 0xa143:
	case 0xa144:
	case 0xa145:
	case 0xa146:
	case 0xa147:
	case 0xa148:
	case 0xa149:
	case 0xa14a:
	case 0xa14b:
	case 0xa14c:
	case 0xa14d:
	case 0xa14e:
	case 0xa14f:
	case 0xa150:
	case 0xa151:
	case 0xa152:
	case 0xa153:
	case 0xa154:
	case 0xa155:
	case 0xa156:
	case 0xa157:
	case 0xa158:
	case 0xa159:
	case 0xa15a:
	case 0xa15b:
	case 0xa15c:
	case 0xa15d:
	case 0xa15f:
	case 0xa15e:
		SouthBridge = "Sunrise  Point-H";
		break;
	case 0xa1c1:
	case 0xa1c2:
	case 0xa1c3:
	case 0xa1c4:
	case 0xa1c5:
	case 0xa1c6:
	case 0xa1c7:
	case 0xa242:
	case 0xa243:
		SouthBridge = "Lewisburg";
		break;
	case 0xa2c4:
		SouthBridge = "H270 (PCH)";
		break;
	case 0xa2c5:
		SouthBridge = "Z270 (PCH)";
		break;
	case 0xa2c6:
		SouthBridge = "Q270 (PCH)";
		break;
	case 0xa2c7:
		SouthBridge = "Q250 (PCH)";
		break;
	case 0xa2c8:
		SouthBridge = "B250 (PCH)";
		break;
	case 0xa304:
		SouthBridge = "H370";
		break;
	default:
		boost::format fmt = boost::format("ID%X") % pci.DeviceID;
		SouthBridge = fmt.str();
		break;
	}
	return true;
}

void Intel::ExecCPUName()
{
	try
	{
		if (this->Brand.find("Core") != string::npos)
		{
			Name += "Core ";
			if (this->Brand.find("i9") != string::npos || this->Brand.find("i7") != string::npos || this->Brand.find("i5") != string::npos || this->Brand.find("i3") != string::npos)
			{
				string Model1 = {};
				size_t last = 0;
				size_t Index = this->Brand.find_first_of(" ", last);
				last = Index + 1;
				Index = this->Brand.find_first_of(" ", last);
				last = Index + 1;
				Index = this->Brand.find_first_of(" ", last);
				Model1 = Brand.substr(last, Index - last);
				Index = Model1.find_first_of("-");
				Name += Model1.substr(0, Index);
				Name += " ";
				Name += Model1.substr(Index + 1);
			}
		}
		else if (this->Brand.find("Pentium Silver") != string::npos)
		{
			Name += "Pentium Silver ";
			size_t last = 0;
			size_t Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			Name += this->Brand.substr(last, Index - last);
		}
		else if (this->Brand.find("Pentium Gold") != string::npos)
		{
			Name += "Pentium Gold ";
			size_t last = 0;
			size_t Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			Name += this->Brand.substr(last, Index - last);
		}
		else if (this->Brand.find("Pentium") != string::npos)
		{
			Name += "Pentium ";
			size_t last = 0;
			size_t Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			Name += this->Brand.substr(last, Index - last);
		}
		else if (this->Brand.find("Celeron") != string::npos)
		{
			Name += "Celeron ";
			size_t last = 0;
			size_t Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			Name += this->Brand.substr(last, Index - last);
		}
		else if (this->Brand.find("Atom") != string::npos)
		{
			Name += "Atom ";
			size_t last = 0;
			size_t Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			last = Index + 1;
			Index = this->Brand.find_first_of(" ", last);
			Name += this->Brand.substr(last, Index - last);
		}
		else if (this->Brand.find("Xeon") != string::npos)
		{
			Name += "Xeon ";
			if (this->Brand.find("E-") != string::npos || this->Brand.find("E3-") != string::npos || this->Brand.find("E5-") != string::npos || this->Brand.find("E7-") != string::npos)
			{
				string Model1 = {};
				size_t last = 0, temp = 0;
				size_t Index = Brand.find_first_of(" ", last);
				last = Index + 1;
				Index = Brand.find_first_of(" ", last);
				last = Index + 1;
				Index = Brand.find_first_of(" ", last);
				last = Index + 1;
				Index = Brand.find_first_of(" ", last);
				temp = Index;
				Model1 = Brand.substr(last, Index - last);
				Index = Model1.find_first_of("-");
				Name += Model1.substr(0, Index);
				Name += " ";
				Name += Model1.substr(Index + 1);
				if (Brand.rfind("V2") != string::npos || Brand.rfind("V3") != string::npos || Brand.rfind("V6") != string::npos || Brand.rfind("V4") != string::npos || Brand.rfind("V5") != string::npos)
				{
					Name += " ";
					last = temp + 1;
					Index = Brand.find_first_of(" ", last);
					Name += Brand.substr(last, Index - last);
				}
			}
			else
			{
				if (this->Brand.find("Silver") != string::npos)
					Name += "Silver ";
				else if (this->Brand.find("Gold") != string::npos)
					Name += "Gold ";
				else if (this->Brand.find("Bronze") != string::npos)
					Name += "Bronze ";
				else if (this->Brand.find("Platinum") != string::npos)
					Name += "Platinum ";
				else if (this->Brand.find("MP") != string::npos)
					Name += "MP ";
				else if (this->Brand.find("Phi") != string::npos)
					Name += "Phi ";
				string Model1 = {};
				size_t last = 0;
				size_t Index = Brand.find_first_of(" ", last);
				last = Index + 1;
				Index = Brand.find_first_of(" ", last);
				last = Index + 1;
				Index = Brand.find_first_of(" ", last);
				last = Index + 1;
				Index = Brand.find_first_of(" ", last);
				Name += Brand.substr(last, Index - last);
			}
		}
		else
		{
			Name = "Intel Processor";
		}
	}
	catch (const std::exception&)
	{
		Name = "Intel Processor";
	}

}

void Intel::ExecCodeNameByFMS(int Family, int Model)
{
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
}
#pragma endregion

#pragma region Voltage
void Intel::ExecVoltageByFMS()
{
	double VCore = INFINITY;
	DWORD64 msrdata = 0;
	PCI_COMMON_CONFIG pci = {};
	SV_ASSIST::Ring0::ReadPci(0, 0, 0, pci);
	DWORD64 VIDmask = 0xFFFF00000000;
	if (pci.DeviceID == 0x3C00)
		VIDmask = 0x3F00000000;
	if ((DisplayFamily == 0x06 && DisplayModel == 0x46) || (DisplayModel == 0x2D && DisplayFamily == 0x06))
	{
		for (DWORD threadAffinityMask = 0; threadAffinityMask < this->Core; threadAffinityMask++)
		{
			if (SV_ASSIST::Ring0::RdMsrTx(IA32_PERF_STATUS, msrdata, threadAffinityMask) == 1 && msrdata)
			{
				WORD VID = (msrdata & VIDmask) >> 32;
				VCore = VID / pow(2, 13);
				if (VCore < 0.0)
					VCore = INFINITY;
				CoreVID[threadAffinityMask] = VCore;
			}
		}
	}
}
#pragma endregion

#pragma region Temperature
void Intel::ExecTemperature()
{
	DWORD64 msrdata = 0;
	for (DWORD threadAffinityMask = 0; threadAffinityMask < Core; threadAffinityMask++)
	{
		if (SV_ASSIST::Ring0::RdMsrTx(IA32_THERM_STATUS_MSR, msrdata, threadAffinityMask) == 1)
		{
			auto temperature = (Tjmax[threadAffinityMask] - ((msrdata & 0x007F0000) >> 16));
			Temperature[threadAffinityMask] = temperature;
		}
	}

	if (SV_ASSIST::Ring0::RdMsr(IA32_PACKAGE_THERM_STATUS, msrdata) == 1)
	{
		PackageTemperature = (PackageTjmax - ((msrdata & 0x007F0000) >> 16));
	}
}

BOOL Intel::GetTjMaxFromMSR()
{
	DWORD64 msrdata = 0;
	for (DWORD threadAffinityMask = 0; threadAffinityMask < this->Core; threadAffinityMask++)
	{
		if (SV_ASSIST::Ring0::RdMsrTx(IA32_TEMPERATURE_TARGET, msrdata, threadAffinityMask) == 1)
		{
			Tjmax[threadAffinityMask] = ((DWORD)msrdata >> 16) & 0xFF;
		}
		else
		{
			Tjmax[threadAffinityMask] = 100.0;
		}
	}
	PackageTjmax = Tjmax[0];
	return TRUE;
}
#pragma endregion

#pragma region Frequency
void Intel::GetBusSpeed()
{
	DWORD64 data = 0;
	if (SV_ASSIST::Ring0::RdMsr(0xCD, data) == 1)
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
	else if(MaxNonTurboFre)
	{
		LARGE_INTEGER nFreq, nBeginTime, nEndTime;
		double time;
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nBeginTime);
		auto tsc1 = __rdtsc();
		Sleep(1);
		auto tsc2 = __rdtsc();
		QueryPerformanceCounter(&nEndTime);

		time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;

		BusSpeed = (tsc2 - tsc1) / time / (MaxNonTurboFre * 10000.0);
	}
}

void Intel::GetCurrentSpeed()
{
	DWORD64 msrdata = 0;
	if (BusSpeed != INFINITY)
	{
		for (DWORD threadAffinityMask = 0; threadAffinityMask < this->Core; threadAffinityMask++)
		{
			if (SV_ASSIST::Ring0::RdMsrTx(IA32_PERF_STATUS, msrdata, threadAffinityMask) == 1)
			{
				CurrentClockSpeed[threadAffinityMask] = (((DWORD)msrdata >> 8) & 0xFF) * BusSpeed;
			}
		}
	}
}
#pragma endregion
#pragma endregion