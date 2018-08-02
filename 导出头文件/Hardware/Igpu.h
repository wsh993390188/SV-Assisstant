#pragma once 
class IgpuSensor
{
public:
	const std::string& GetGPUName();
	const std::string& GetGPUBiosVersion();
	const std::string& GetGPUDriverVersion();
	const std::string& GetGPUBranchVersion();
	INT GetTemperature();
	double GetGPUClock();
	double GetGPUMemoryClock();
	ULONG GetGPUusage();
	ULONG GetGPUfans();
	size_t GetdedicatedVideoMemory();
	size_t GetsystemVideoMemory();
	size_t GetsharedSystemMemory();
	~IgpuSensor();
protected:
	IgpuSensor();
	std::string GPUname;
	std::string GPUBiosVersion;
	std::string GPUDriverVersion;
	std::string GPUBranchVersion;
	INT	Temperature;
	double GPUClock;
	double GPUMemclock;
	ULONG GPUusage;
	ULONG fans;
	size_t dedicatedVideoMemory;
	size_t systemVideoMemory;
	size_t sharedSystemMemory;
};