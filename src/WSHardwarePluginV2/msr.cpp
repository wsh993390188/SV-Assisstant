#include "stdafx.h"
#include "msr.h"
#ifdef NewRing0
#include "Sys\DriverLoad\WinRing0.h"
#else
#include "HardwareRing0.h"
#endif

using namespace Hardware::Utils::Ring0;

// here comes an implementatation for Windows
MsrHandle::MsrHandle(uint32_t cpu) : cpu_id(cpu)
{
}

bool MsrHandle::write(uint64_t msr_number, uint64_t value)
{
	Utils::CvtDs cvt;
	cvt.ui64 = value;

	Utils::ThreadGroupTempAffinity affinity(cpu_id);
#ifdef NewRing0
	DWORD status = WinRing0::Instance().WrMsr((DWORD)msr_number, cvt.ui64);
#else
	DWORD status = WinRing0::Instance().WrMsr((DWORD)msr_number, cvt.ui32.Eax, cvt.ui32.Edx);
#endif

	return status == TRUE;
}

bool MsrHandle::read(uint64_t msr_number, uint64_t& value)
{
	Utils::CvtDs cvt;
	cvt.ui64 = 0;

	Utils::ThreadGroupTempAffinity affinity(cpu_id);
#ifdef NewRing0
	DWORD status = WinRing0::Instance().RdMsr((DWORD)msr_number, cvt.ui64);
#else
	DWORD status = WinRing0::Instance().RdMsr((DWORD)msr_number, &(cvt.ui32.Eax), &(cvt.ui32.Edx));
#endif

	if (status)
		value = cvt.ui64;

	return status == TRUE;
}