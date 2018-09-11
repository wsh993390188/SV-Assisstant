#pragma once
#include "CPUBase.h"
#include "CPUWMI.h"
#include <bitset>

/************
INTELµÄÐÅÏ¢
************/
//Deterministic Cache Parameters CPUID = 4
//EAX
#define CACHE_TYPE							0x0000001F
#define CACHE_LEVEL							0x000000E0
#define CACHE_MAX_ADDR_LOGICAL				0x03FFC000
#define CACHE_MAX_ADDR_PHYSICAL				0xFC000000
//EBX
#define CACHE_WAYS							0xFFC00000
#define CACHE_PARTITIONS					0x003FF000
#define CACHE_LINE_SIZE						0x00000FFF
//EDX
#define CACHE_INVALIDATE					0x00000000
#define CACHE_INCLUSIVENESS					0x00000001
#define CACHE_COMPLEX_INDEXING				0x00000002

class Intel : public CPUBASE
{
public:
	Intel();
//	using CPUBASE::CPUBASE;
	~Intel();
protected:
	void Init(void);
	void UpDateData(void);
	void ExecFeature();
	void ExecCache();
	bool ExecProcessorOtherInfo(int Family, int Model);
	bool ExecSouthBridge();
	void ExecTemperature();
	BOOL GetTjMaxFromMSR();
private:	
	void ExecCPUName();
	void ExecCodeNameByFMS(int Family, int Model);
	void ExecVoltageByFMS();
	void GetBusSpeed(DWORD threadAffinityMask);
	void GetCurrentSpeed();
#pragma region MSR Register
	const DWORD IA32_THERM_STATUS_MSR;
	const DWORD IA32_TEMPERATURE_TARGET;
	const DWORD IA32_PERF_STATUS;
	const DWORD MSR_PLATFORM_INFO;
	const DWORD IA32_PACKAGE_THERM_STATUS;
	const DWORD MSR_RAPL_POWER_UNIT;
	const DWORD MSR_PKG_ENERY_STATUS;
	const DWORD MSR_DRAM_ENERGY_STATUS;
	const DWORD MSR_PP0_ENERY_STATUS;
	const DWORD MSR_PP1_ENERY_STATUS;
#pragma endregion
	int DisplayFamily;
	int DisplayModel;
	std::vector<double> Tjmax; 
	double PackageTjmax;
	UINT MaxNonTurboFre;
	std::bitset<32> f1_ecx;
	std::bitset<32> f1_edx;
	std::bitset<32> f7_ebx;
	std::bitset<32> f7_ecx;
};