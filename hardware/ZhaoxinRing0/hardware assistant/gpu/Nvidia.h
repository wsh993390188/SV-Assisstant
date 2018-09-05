#pragma once

#include "Gpudata.h"

#define NVAPI_GPU_UTILIZATION_DOMAIN_GPU 0
#define NVAPI_GPU_UTILIZATION_DOMAIN_FB  1
#define NVAPI_GPU_UTILIZATION_DOMAIN_VID 2
#define NVAPI_GPU_UTILIZATION_DOMAIN_BUS 3

class CNvidia : public CGPU
{
public:
	CNvidia();
	using CGPU::CGPU;
	~CNvidia();
	bool CheckdllStatus();
	GPUTypes exec();
	const void* Returninfo();
	GPUTypes UpdateData();
private:
	NvAPI_Status PhysicalGPUs(INT Index, NvidiaInfo& nvinfo);
	NvAPI_Status UpdatePhysicalGPUs(INT Index, NvidiaInfo& nvinfo);
	NvAPI_Status DisplayGPUS();

	NvAPI_Status EnumPhysicalGPUs();
	NvAPI_Status EnumNvidiaDisplayHandle();

	BOOL GetPCIESpeed(NvidiaInfo & nvinfo);
	BOOL GetInterfaceVersion(std::string& version);
	BOOL GetsysDriverbranchVersion(std::string& driver, std::string& Branch);
	BOOL GetChipInfo();

	BOOL GetGPUFullName(std::string& GPUName, INT Index);
	BOOL GetGPUMem(NV_DISPLAY_DRIVER_MEMORY_INFO& mem, INT Index);
	BOOL GetPCIIdentify(INT Index, NvU32 & pDeviceId, NvU32 & pSubSystemId, NvU32 & pRevisionId, NvU32 & pExtDeviceId);
	BOOL GetGPUtemperature(INT Index, INT& MinTemp, INT& CurrentTemp, INT& MaxTemp);
	BOOL GetGPUFans(INT Index, ULONG& FansValue);
	BOOL GetGPUClock(INT Index, DEVICE_CLOCK& Device_clock);
	BOOL GetHDCPSupport(INT Index, BOOL& HDCPState);
	BOOL GetGpuCoreCount(INT Index, UINT& corecount);
	BOOL GetDynamicPstatesInfo(INT Index, NV_GPU_DYNAMIC_PSTATES_INFO_EX& percentage);
	BOOL GetVbiosVersion(INT Index, std::string& VbiosVersion);
	BOOL GetGpuType(INT Index, std::string& GPUType);
	BOOL GetSysType(INT Index, std::string& SysType);
	BOOL GetPhysicalFrameBufferSize(INT Index, ULONG& PhysicalFrame);
	BOOL GetVirtualFrameBufferSize(INT Index, ULONG& VirtualFrame);
	BOOL GetGPUPState(INT Index, NV_GPU_PERF_PSTATES20_INFO& NV_PState);
	BOOL GetCurrentPState(INT Index, UINT& CurrentPState);
	BOOL GetDynamicInfo(INT Index, NVIDIA_USAGE& Nvidia_Usage);
private:
	NvAPI_Status m_succeed_Nvidia;	//判断加载dll是否成功
	NvDisplayHandle hDisplay[NVAPI_MAX_PHYSICAL_GPUS * 2];
	NvPhysicalGpuHandle phys[NVAPI_MAX_PHYSICAL_GPUS];	//物理GPU的总数
	NvU32 physcount;
	//out
	std::vector<NvidiaInfo> realnv;

	NV_CHIPSET_INFO Chipset;
};
