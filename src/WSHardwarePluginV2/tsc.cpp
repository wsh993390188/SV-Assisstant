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

bool TscIntervalHandle::read(double& value)
{
	Utils::ThreadGroupTempAffinity affinity(cpu_id);
	value = WinRing0::Instance().GetTscFrequency();
	return true;
}