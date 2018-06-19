#include "stdafx.h"
#include "Igpu.h"


IgpuSensor::IgpuSensor() : GPUname{}, GPUBiosVersion{}, Temperature{}, GPUClock{}, GPUusage{}
, GPUMemclock{}
{
}


IgpuSensor::~IgpuSensor()
{
}

const std::string & IgpuSensor::GetGPUName()
{
	return this->GPUname;
}

const std::string & IgpuSensor::GetGPUBiosVersion()
{
	return this->GPUBiosVersion;
}

INT IgpuSensor::GetTemperature()
{
	return this->Temperature;
}

double IgpuSensor::GetGPUClock()
{
	return this->GPUClock;
}

double IgpuSensor::GetGPUMemoryClock()
{
	return this->GPUMemclock;
}

ULONG IgpuSensor::GetGPUusage()
{
	return this->GPUusage;
}

ULONG IgpuSensor::GetGPUfans()
{
	return this->fans;
}

BOOL IgpuSensor::SetGPUName(std::string name)
{
	BOOL status = FALSE;
	if (name.size())
	{
		this->GPUname = name;
		status = TRUE;
	}
	return status;
}

BOOL IgpuSensor::SetGPUBiosVersion(std::string biosversion)
{
	BOOL status = FALSE;
	if (biosversion.size())
	{
		this->GPUBiosVersion = biosversion;
		status = TRUE;
	}
	return status;
}

BOOL IgpuSensor::SetTemperature(INT Temperature)
{
	BOOL status = FALSE;
	if (Temperature)
	{
		this->Temperature = Temperature;
		status = TRUE;
	}
	return status;
}

BOOL IgpuSensor::SetGPUClock(double gpuclock)
{
	BOOL status = FALSE;
	if (gpuclock)
	{
		this->GPUClock = gpuclock;
		status = TRUE;
	}
	return status;
}

BOOL IgpuSensor::SetGPUMemoryClock(double gpumemoryclock)
{
	BOOL status = FALSE;
	if (gpumemoryclock)
	{
		this->GPUMemclock = gpumemoryclock;
		status = TRUE;
	}
	return status;
}

BOOL IgpuSensor::SetGPUusage(ULONG gpusage)
{
	BOOL status = FALSE;
	if (gpusage)
	{
		this->GPUusage = gpusage;
		status = TRUE;
	}
	return status;
}

BOOL IgpuSensor::SetGPUfans(ULONG fans)
{
	BOOL status = FALSE;
	if (fans)
	{
		this->fans = fans;
		status = TRUE;
	}
	return status;
}
