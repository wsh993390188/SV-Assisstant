#pragma once

namespace SV_ASSIST
{
	namespace CPU
	{
		namespace ZHAOXINMSR
		{
			DRIVERDLL_API BOOL ZX_Temperature(IN DWORD_PTR threadAffinityMask, OUT int temperature);

			DRIVERDLL_API BOOL ZX_FIDVID(IN DWORD_PTR threadAffinityMask, OUT DWORD FID, OUT DWORD VID);

			DRIVERDLL_API BOOL ZX_TargetFIDVID(IN DWORD_PTR threadAffinityMask, OUT DWORD FID, OUT DWORD VID);
		}
	}
}