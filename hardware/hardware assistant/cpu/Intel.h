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
	~Intel();
protected:
	void Init(void);
	void UpDateData(void);
	void ExecFeature();
	void ExecCache();
	bool ExecProcessorOtherInfo(int DF_DM);
private:	
	inline int findcpuid(int value);
	std::bitset<32> f1_ecx;
	std::bitset<32> f1_edx;
	std::bitset<32> f7_ebx;
	std::bitset<32> f7_ecx;
};