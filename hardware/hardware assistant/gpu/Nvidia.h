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
	~CNvidia();
	bool CheckdllStatus();
	CGPU::GPUTypes exec();
	const void* Returninfo();
	CGPU::GPUTypes UpdateData();
private:
	BOOL EnumPhysicalGPUs();
	BOOL EnumNvidiaDisplayHandle();

	BOOL GetInterfaceVersion(string& version);
	BOOL GetsysDriverbranchVersion(string& driver, string& Branch);
	BOOL GetGPUFullName(string& GPUName, INT Index);
	BOOL GetGPUMem(NV_DISPLAY_DRIVER_MEMORY_INFO& mem, INT Index);
	BOOL GetGPUtemperature(INT Index, INT& MinTemp, INT& CurrentTemp, INT& MaxTemp);
	BOOL GetGPUFans(INT Index, ULONG& FansValue);
	BOOL GetGPUClock(INT Index);
	BOOL GetHDCPSupport(INT Index, BOOL& HDCPState);
	BOOL GetGpuCoreCount(INT Index, UINT& corecount);
	BOOL GetDynamicPstatesInfo(INT Index, NV_GPU_DYNAMIC_PSTATES_INFO_EX& percentage);
	BOOL GetVbiosVersion(INT Index, string& VbiosVersion);
	BOOL GetVbiosRevision(INT Index, string& VbiosRevision);
	BOOL GetVbiosOEMRevision(INT Index, string& VbiosOEMRevision);
	BOOL GetGpuType(INT Index, string& GPUType);
	BOOL GetSysType(INT Index, string& SysType);
	BOOL GetAGPAperture(INT Index, ULONG& AGPAperture);
	BOOL GetCurrentAGPRate(INT Index, ULONG& AGPRate);
	BOOL GetBUSID(INT Index, ULONG& BusID);
	BOOL GetBUSSlotID(INT Index, ULONG& BusSlotID);
	BOOL GetBusType(INT Index, string& BusType);
	BOOL GetPhysicalFrameBufferSize(INT Index, ULONG& PhysicalFrame);
	BOOL GetVirtualFrameBufferSize(INT Index, ULONG& VirtualFrame);
	BOOL GetGPUPState(INT Index);
	BOOL GetCurrentPState(INT Index, UINT& CurrentPState);
	BOOL GetChipInfo();
	BOOL GetDynamicInfo(INT Index);
	BOOL GetPCIEWidth(INT Index);
	BOOL GetECCConfigurationInfo(INT Index);
	private:
	NvAPI_Status m_succeed_Nvidia;	//判断加载dll是否成功
	NvDisplayHandle hDisplay[NVAPI_MAX_PHYSICAL_GPUS * 2];
	NvPhysicalGpuHandle phys[NVAPI_MAX_PHYSICAL_GPUS];	//物理GPU的总数
	NvU32 physcount;
	NvidiaInfo nvinfo;
};
