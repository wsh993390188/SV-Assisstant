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

const std::string & IgpuSensor::GetGPUDriverVersion()
{
	return this->GPUDriverVersion;
}

const std::string & IgpuSensor::GetGPUBranchVersion()
{
	return this->GPUBranchVersion;
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

size_t IgpuSensor::GetdedicatedVideoMemory()
{
	return this->dedicatedVideoMemory;
}

size_t IgpuSensor::GetsystemVideoMemory()
{
	return this->systemVideoMemory;
}

size_t IgpuSensor::GetsharedSystemMemory()
{
	return this->sharedSystemMemory;
}

