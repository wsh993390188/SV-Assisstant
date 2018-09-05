#pragma once 
struct GPUBaseInfo
{
	GPUBaseInfo();
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

struct GPUSensorInfo 
{

};