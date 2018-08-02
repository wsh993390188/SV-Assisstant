#include "stdafx.h"
#include "..\..\Zhaoxin.h"
#include "IntelMSR.h"

namespace SV_ASSIST 
{
	namespace CPU
	{
		namespace INTELMSR
		{
			const DWORD IA32_THERM_STATUS_MSR = 0x019C;
			const DWORD IA32_TEMPERATURE_TARGET = 0x01A2;
			const DWORD IA32_PERF_STATUS = 0x0198;
			const DWORD MSR_PLATFORM_INFO = 0xCE;
			const DWORD IA32_PACKAGE_THERM_STATUS = 0x1B1;
			const DWORD MSR_RAPL_POWER_UNIT = 0x606;
			const DWORD MSR_PKG_ENERY_STATUS = 0x611;
			const DWORD MSR_DRAM_ENERGY_STATUS = 0x619;
			const DWORD MSR_PP0_ENERY_STATUS = 0x639;
			const DWORD MSR_PP1_ENERY_STATUS = 0x641;

			class _IntelMSR
			{
			public:
				static _IntelMSR* Instance() 
				{
					if (!m_Instance.get())
						m_Instance = make_shared<_IntelMSR>();
					return m_Instance.get(); 
				}
				auto GetTjmax(IN DWORD CoreNum)
				{
					if (CoreNums <= CoreNum)
						return 0.0;
					return Tjmax[CoreNum];
				}

				auto GetPackageTjmax()
				{
					return PackageTjmax;
				}
				_IntelMSR()
				{
					SYSTEM_INFO si;
					GetSystemInfo(&si);
					Tjmax = make_unique<double[]>(si.dwNumberOfProcessors);
					CoreNums = si.dwNumberOfProcessors;
					this->GetMSRInfo();
				}
			protected:
				BOOL GetMSRInfo()
				{
					return this->GetTjMaxFromMSR();
				}
			private:
				static std::shared_ptr<_IntelMSR> m_Instance;
				BOOL GetTjMaxFromMSR()
				{
					DWORD64 msrdata = 0;
					for(DWORD threadAffinityMask = 0; threadAffinityMask < CoreNums; threadAffinityMask++)
					{
						if (ZhaoxinDriver::Instance()->RdMsrTx(IA32_TEMPERATURE_TARGET, msrdata, threadAffinityMask) && msrdata)
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
			private:
				DWORD CoreNums;
				unique_ptr<double[]> Tjmax;
				double PackageTjmax;
			};
			std::shared_ptr<_IntelMSR> _IntelMSR::m_Instance = nullptr;
		}
	}
}

DRIVERDLL_API BOOL SV_ASSIST::CPU::INTELMSR::Intel_Temperature(IN DWORD threadAffinityMask, OUT int& temperature)
{
	DWORD64 msrdata = 0;
	auto Tjmax = SV_ASSIST::CPU::INTELMSR::_IntelMSR::Instance()->GetTjmax(threadAffinityMask);
	if (ZhaoxinDriver::Instance()->RdMsrTx(IA32_THERM_STATUS_MSR, msrdata, threadAffinityMask) && msrdata)
	{
		temperature = (int) (Tjmax - ((msrdata & 0x007F0000) >> 16));
		if(temperature > 0)
		return TRUE;
	}
	return FALSE;
}

DRIVERDLL_API BOOL SV_ASSIST::CPU::INTELMSR::Intel_PackageTemperature(OUT int& temperature)
{
	DWORD64 msrdata = 0;
	auto Tjmax = SV_ASSIST::CPU::INTELMSR::_IntelMSR::Instance()->GetPackageTjmax();
	if (ZhaoxinDriver::Instance()->RdMsrTx(IA32_PACKAGE_THERM_STATUS, msrdata, 0) && msrdata)
	{
		temperature = (int)(Tjmax - ((msrdata & 0x007F0000) >> 16));
		if (temperature > 0)
			return TRUE;
	}
	return FALSE;
}

DRIVERDLL_API BOOL SV_ASSIST::CPU::INTELMSR::Intel_FIDVID(IN DWORD threadAffinityMask, OUT DWORD& FID, OUT DWORD& VID)
{
	DWORD64 msrdata = 0;
	if (ZhaoxinDriver::Instance()->RdMsrTx(IA32_PERF_STATUS, msrdata, threadAffinityMask) && msrdata)
	{
		VID = (msrdata & 0xFF);
		FID = (msrdata & 0xFF00) >> 8;
		return TRUE;
	}
	return FALSE;
}