#pragma once
#include "CPUBase.h"
#include "CPUWMI.h"
#include <bitset>


class Zhaoxin final : public CPUBASE
{
public:
	Zhaoxin();
	~Zhaoxin();
protected:
	void Init(void) override;
	void UpDateData(void) override;
	void ExecFeature();
	void ExecCache();
	void ExecVoltage();
	void ExecTemperature();
private:
	void GetBusSpeed();
	void GetCurrentSpeed();
#pragma region MSR Register
	const DWORD Zhaoxin_Temperature;
	const DWORD Zhaoxin_FIDVID;
	const DWORD Zhaoxin_Target;
#pragma endregion

#pragma region CPUID Feature
	std::bitset<32> f1_ecx;
	std::bitset<32> f1_edx;
	std::bitset<32> f7_ebx;
	std::bitset<32> f7_ecx;
	std::bitset<32> f81_ecx;
	std::bitset<32> f81_edx;
#pragma endregion
};
