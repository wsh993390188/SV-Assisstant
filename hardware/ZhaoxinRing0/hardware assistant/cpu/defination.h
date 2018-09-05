#pragma once
struct Intel_FEATURE_INFO
{
	bool SSE3;
	bool PCLMULQDQ;
	bool DTES64;
	bool MONITOR;
	bool DS_CPL;
	bool VMX;
	bool SMX;
	bool EIST;
	bool TM2;
	bool SSSE3;
	bool CNXT_ID;
	bool SDBG;
	bool FMA;
	bool CMPXCHG16B;
	bool xTPR_Update_Control;
	bool PDCM;
	bool PCID;
	bool DCA;
	bool SSE4_1;
	bool SSE4_2;
	bool x2APIC;
	bool MOVBE;
	bool POPCNT;
	bool TSC_Deadline;
	bool AESNI;
	bool XSAVE;
	bool OSXSAVE;
	bool AVX;
	bool F16C;
	bool RDRAND;

	bool FPU;
	bool VME;
	bool DE;
	bool PSE;
	bool TSC;
	bool MSR;
	bool PAE;
	bool MCE;
	bool CX8;
	bool APIC;
	bool SEP;
	bool MTRR;
	bool PGE;
	bool MCA;
	bool CMOV;
	bool PAT;
	bool PSE_36;
	bool PSN;
	bool CLFSH;
	bool DS;
	bool ACPI;
	bool MMX;
	bool FXSR;
	bool SSE;
	bool SSE2;
	bool SS;
	bool HTT;
	bool TM;
	bool PBE;

	bool FSGSBASE;
	bool IA32_TSC_ADJUST_MSR;
	bool SGX;
	bool BMI1;
	bool HLE;
	bool AVX2;
	bool FDP_EXCPTN_ONLY;
	bool SMEP;
	bool BMI2;
	bool Enhanced_REP_MOVSB_OR_STOSB;
	bool INVPCID;
	bool RTM;
	bool RDT_M;
	bool FPU_CS_DS;
	bool MPX;
	bool RDT_A;
	bool RDSEED;
	bool ADX;
	bool SMAP;
	bool CLFLUSHOPT;
	bool CLWB;
	bool Intel_Processor_Trace;
	bool SHA;

	bool PREFETCHWT1;
	bool UMIP;
	bool PKU;
	bool OSPKE;
	bool RDPID;
	bool SGX_LC;
};

struct Zhaoxin_FEATURE_INFO
{
	bool SSE3;
	bool MMWAIT;
	bool VMX;
	bool EPS;
	bool TM2;
	bool SSSE3;
	bool CMPXCHG16B;
	bool EchoTPR;
	bool SSE4_1;
	bool SSE4_2;

	bool FPU;
	bool VME;
	bool DE;
	bool PSE;
	bool TSC;
	bool MSR;
	bool PAE;
	bool MCE;
	bool CX8;
	bool APIC;
	bool SEP;
	bool MTRR;
	bool PGE;
	bool CMOV;
	bool PAT;
	bool CLFSH;
	bool ACPI;
	bool MMX;
	bool FXSR;
	bool SSE;
	bool SSE2;
	bool SS;
	bool TM;
	bool PBE;
};

struct AMD_FEATURE_INFO
{
	//ECX
	bool SSE3;
	bool PCLMULQDQ;
	bool MONITOR;
	bool SSSE3;
	bool FMA;
	bool CMPXCHG16B;
	bool SSE41;
	bool SSE42;
	bool POPCNT;
	bool AES;
	bool XSAVE;
	bool OSXSAVE;
	bool AVX;
	bool F16C;
	bool RDRAND;

	//EDX
	bool FPU;
	bool VME;
	bool DE;
	bool PSE;
	bool TSC;
	bool MSR;
	bool PAE;
	bool MCE;
	bool CMPXCHG8B;
	bool APIC;
	bool SysEnterSysExit;
	bool MTRR;
	bool PGE;
	bool MCA;
	bool CMOV;
	bool PAT;
	bool PSE36;
	bool CLFSH;
	bool MMX;
	bool FXSR;
	bool SSE;
	bool SSE2;
	bool HTT;

	//0x80000001Ê± ECX¡¢EDX
	//ECX
	bool LahfSahf;
	bool CmpLegacy;
	bool SVM;
	bool ExtApicSpace;
	bool AltMovCr8;
	bool ABM;
	bool SSE4A;
	bool MisAlignSse;
	bool _3DNowPrefetch;
	bool OSVW;
	bool IBS;
	bool XOP;
	bool SKINIT;
	bool WDT;
	bool LWP;
	bool FMA4;
	bool TBM;
	bool TopologyExtensions;
	bool PerfCtrExtCore;
	bool PerfCtrExtNB;
	bool DataBreakpointExtension;
	bool PerfTsc;
	//EDX
	bool SysCallSysRet;
	bool NX;
	bool MmxExt;
	bool FFXSR;
	bool Page1GB;
	bool RDTSCP;
	bool LM;
	bool _3DNowExt;
	bool _3DNow;

};

union CPUFeature 
{
	Intel_FEATURE_INFO IntelFeature;
	AMD_FEATURE_INFO AMDFeature;
	Zhaoxin_FEATURE_INFO ZhaoxinFeature;
};

struct Cache_info
{
	char Cache_type;
	unsigned int Cache_level;
	unsigned int Cache_Ways;
	unsigned int Cache_line_size;
	unsigned int Cache_partitions;
	unsigned int Cache_Set;
	unsigned int Cache_Size;
//	unsigned int Cache_max_addr_logical;
//	unsigned int Cache_max_addr_physical;
//	bool CACHE_Invalidate;
//	bool Cache_Inclusiveness;
//	bool Cache_Complex_Indexing;
};