#pragma once

namespace SV_ASSIST
{
	namespace CPU
	{
		namespace ZHAOXINMSR
		{
			DRIVERDLL_API BOOL ZX_Temperature(IN DWORD threadAffinityMask, OUT int& temperature);

			DRIVERDLL_API BOOL ZX_FIDVID(IN DWORD threadAffinityMask, OUT DWORD& FID, OUT DWORD& VID);

			DRIVERDLL_API BOOL ZX_TargetFIDVID(IN DWORD threadAffinityMask, OUT DWORD& FID, OUT DWORD& VID);
		}
	}
}