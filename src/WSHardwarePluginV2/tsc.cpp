#include "stdafx.h"
#include "tsc.h"
#ifdef NewRing0
#include "Sys\DriverLoad\WinRing0.h"
#else
#include "HardwareRing0.h"
#endif

using namespace Hardware::Utils::Ring0;

// here comes an implementatation for Windows
TscIntervalHandle::TscIntervalHandle(uint32_t cpu) : cpu_id(cpu)
{
}

bool TscIntervalHandle::read(uint64_t& value)
{
	Utils::CvtDs cvt;
	cvt.ui64 = 0;

	Utils::ThreadGroupTempAffinity affinity(cpu_id);
	DWORD status = WinRing0::Instance().RdTsc(cvt.ui64);

	if (status)
		value = cvt.ui64;

	return status == TRUE;
}