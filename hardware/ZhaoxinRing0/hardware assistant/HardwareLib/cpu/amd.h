#pragma once
#include "CPUBase.h"
#include "CPUWMI.h"
#include <bitset>

class AMD final : public CPUBASE
{
public:
	AMD();
	~AMD();
protected:
	void Init(void) override;
	void UpDateData(void) override;
	void ExecFeature();
	void ExecCache();
private:
	void FIDVID_Family17(IN DWORD threadAffinityMask, IN ULONG Index, OUT double & COF, OUT double & CpuIdd, OUT double & CpuVID);
	void GetCurrentPState_17Family(IN DWORD threadAffinityMask, OUT double & COF, OUT double & CpuIdd, OUT double & CpuVID);
	void GetBusSpeed(DWORD threadAffinityMask = 0);
	void GetVoltage();
	void GetFrequency();
#pragma region MSR Register
	const DWORD AMD_PSTATE[8];
	const DWORD AMD_PState_Status;
	const DWORD AMD_CurrentState;
#pragma endregion
	std::bitset<32> f1_ecx;
	std::bitset<32> f1_edx;
	std::bitset<32> f7_ebx;
	std::bitset<32> f7_ecx;
	std::bitset<32> f81_ecx;
	std::bitset<32> f81_edx;
};
