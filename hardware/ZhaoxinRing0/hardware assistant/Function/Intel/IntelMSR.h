#pragma once

namespace SV_ASSIST
{
	namespace CPU
	{
		namespace INTELMSR
		{
			DRIVERDLL_API BOOL Intel_Temperature(IN DWORD threadAffinityMask, OUT int& temperature);
			DRIVERDLL_API BOOL Intel_PackageTemperature(OUT int & temperature);
			DRIVERDLL_API BOOL Intel_FIDVID(IN DWORD threadAffinityMask, OUT DWORD& FID, OUT DWORD& VID);
		}
	}
}