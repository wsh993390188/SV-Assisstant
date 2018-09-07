#pragma once 
struct GPUBaseInfo
{
	std::string GPUname;
	std::string GPUBiosVersion;
	std::string GPUDriverVersion;
	std::string GPUBranchVersion;
	std::string GPUArchitecture;
	std::string GPUTechnology;
	std::string GPUTransistors;
	std::wstring GPUDieSize;
	std::string GPUCoreName;
	ULONG GPURevision;
	std::string GPUSubVendor;
	std::string GPUMemorySize;
	std::string GPUSharedMemorySize;
	std::string GPUMemoryType;
	std::string GPUMemoryBus;
	std::string GPUBusWidth;
	std::string GPUTDP;
	ULONG GPUShaders;
	ULONG GPUTMUs;
	ULONG GPUROPs;
	std::string GPUPixelRate;
	std::string GPUTextureRate;
	PCISPEED MaxPCIESpeed;
};

struct GPUSensorInfo 
{
	std::string	Temperature;
	std::string GPUClock;
	std::string GPUBaseclock;
	std::string GPUBoostClock;
	std::string GPUMemClock;
	std::string GPUMemBaseclock;
	std::string GPUMemBoostClock;
	std::string GPUUsage;
	std::string GPUFrameBufferUsage;
	std::string GPUVideoEngineUsage;
	std::string GPUBUSInterFaceUsage;
	std::string fans;
	std::string systemAvailableMemory;
	PCISPEED CurrentPCIESpeed;
};