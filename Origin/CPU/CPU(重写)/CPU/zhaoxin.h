#pragma once
#include "CPUBase.h"
#include "CPUWMI.h"
#include <bitset>


class Zhaoxin : public CPUBASE
{
public:
	Zhaoxin();
	~Zhaoxin();
protected:
	void Init(void);
	void UpDateData(void);
	void ExecFeature();
	void ExecCache();
private:
	inline int findcpuid(int value);
	std::bitset<32> f1_ecx;
	std::bitset<32> f1_edx;
	std::bitset<32> f7_ebx;
	std::bitset<32> f7_ecx;
	std::bitset<32> f81_ecx;
	std::bitset<32> f81_edx;
};
