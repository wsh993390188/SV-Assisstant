#pragma once

namespace SV_ASSIST
{
	namespace CPU
	{
		namespace AMD_DRIVER
		{
			const DWORD AMD_PSTATE[]
			{
				0xC0010064,
				0xC0010065,
				0xC0010066,
				0xC0010067,
				0xC0010068,
				0xC0010069,
				0xC001006A,
				0xC001006B
			};
			const DWORD AMD_PState_Status = 0xC0010063;
			const DWORD AMD_CurrentState = 0xC0010293;
		}
	}
}
