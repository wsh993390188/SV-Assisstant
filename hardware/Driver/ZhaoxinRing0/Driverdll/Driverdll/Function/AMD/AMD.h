#pragma once
#include "AMDMSR.h"

namespace SV_ASSIST
{
	namespace CPU
	{
		namespace AMD_DRIVER
		{
			DRIVERDLL_API BOOL FIDVID_Family17(IN DWORD_PTR threadAffinityMask, IN ULONG Index, OUT double& COF, OUT double& CpuIdd, OUT ULONG& CpuVID );
		}
	}
}


