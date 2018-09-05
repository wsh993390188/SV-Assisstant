#pragma once
#include "NVAPI/nvapi.h"
#include "ADL/adl_sdk.h"

typedef struct _ALL_DEVICE_CLOCK
{
	ULONG GraphicsCurrent;
	ULONG GraphicsBase;
	ULONG GraphicsBoost;

	ULONG MemoryCurrent;
	ULONG MemoryBase;
	ULONG MemoryBoost;
}DEVICE_CLOCK;

typedef struct _All_DEVICE_TEM
{
	INT GPUMinTem;
	INT GPUCurrTem;
	INT GPUMAXtem;
}DEVICE_TEM;

typedef struct _NVIDIA_USAGE
{
	ULONG GPUUsage;
	ULONG FrameBufferUsage;
	ULONG VideoEngineUsage;
	ULONG BUSInterFaceUsage;
}NVIDIA_USAGE;

typedef struct _AMDISPLAYPOSITION
{
	int X;
	int Y;
	int XDefault;
	int YDefault;
	int MinX;
	INT MinY;
	int MaxX;
	int MaxY;
	int StepX;
	int StepY;
}AMDISPLAYPOSITION;

typedef struct _AMDISPLAYSIZE
{
	int Width;
	int Height;
	int DefaultWidth;
	int DefaultHeight;
	int MinWidth;
	INT MinHeight;
	int MaxWidth;
	int MaxHeight;
	int StepWidth;
	int StepHeight;
}AMDISPLAYSIZE;

typedef struct _AMDISPLAY
{
	int								PreservedAspectRatio_Support;
	int								PreservedAspectRatio_Current;
	int								PreservedAspectRatio_Default;

	int								ImageExpansion_Support;
	int								ImageExpansion_Current;
	int								ImageExpansion_Default;

	AMDISPLAYPOSITION				DisplayPosition;
	AMDISPLAYSIZE					DisplaySize;
	ADLDisplayConfig				DisplayConfig;
	ADLDisplayProperty				DisplayProperty;

	int								DitherState;
	int								SupportedPixelFormat;
	int								CurrentedPixelFormat;
	int								SupportColorDepth;
	int								CurrentColorDepth;
	int								AdjustmentCoherentCurrent;
	int								AdjustmentCoherentDefault;
	int								ReducedBlankingCurrent;
	int								ReducedBlankingDefault;
	int								OverrideSettingsSupportedEx;
	int								OverrideSettingsSupported;
	int								OverrideCurSettings;
}AMDISPLAY;

typedef struct _TEM_AND_FAN
{
	int								ThermalControllerIndex;
	int								temperatureInDegreesCelsius;
	ADLTemperature					adlTemperature;
	ADLFanSpeedValue				fanSpeedValue;
}TEM_AND_FAN;

typedef struct _OVERDRIVE5
{
	TEM_AND_FAN						TemperatureAndFans;
	ADLODPerformanceLevel			DefaultperformanceLevels[16];
	ADLODPerformanceLevel			CurrentperformanceLevels[16];
	ADLPowerControlInfo				powerControlInfo;
	int								powerControlCurrent;
	int								powerControlDefault;
	ADLODParameters					overdriveParameters;

	ADLPMActivity					activity;
}OVERDRIVE5;

typedef struct _AMDINFO
{
	/// The BUS number associated with this adapter.
	int iBusNumber;
	/// The driver number associated with this adapter.
	int iDeviceNumber;
	/// The function number.
	int iFunctionNumber;
	/// The vendor ID associated with this adapter.
	int iVendorID;

	int iDeviceId;
	std::string						FullName;
	std::string						adapterActive;
	std::string						Aspects;
	ADLBiosInfo						biosInfo;
	int								ASICFamilyTypes;
	int								ASICFamilyValids;
	int								AdapterSpeedCurrent;
	int								AdapterSpeedDefault;
	std::string						AdapterAccessibility;
	int								AdapterID;
	ADLAdapterCapsX2				adapterCaps;
	OVERDRIVE5						OverDrive5;
	size_t dedicatedVideoMemory;
	size_t systemVideoMemory;
	size_t sharedSystemMemory;
}AMDINFO;

typedef struct _NvidiaInfo
{
	std::string FullName;
	std::string GPUType;
	std::string VbiosVersion;
	std::string SysType;
	ULONG PhysicalFrame;
	ULONG VirtualFrame;
	BOOL HDCP;
	UINT corecount;
	struct
	{
		USHORT bus;
		USHORT dev;
		USHORT func;

		ULONG pDeviceId;
		ULONG pSubSystemId;
		ULONG pRevisionId;
		ULONG pExtDeviceId;
	}PCIIdentify;

	ULONG dedicatedVideoMemory;
	ULONG availableDedicatedVideoMemory;
	ULONG systemVideoMemory;
	ULONG sharedSystemMemory;
	ULONG curAvailableDedicatedVideoMemory;
	ULONG dedicatedVideoMemoryEvictionsSize;
	ULONG dedicatedVideoMemoryEvictionCount;
	ULONG fans;
	DEVICE_TEM Device_Tem;
	DEVICE_CLOCK Device_clock;	//khz
	NV_GPU_DYNAMIC_PSTATES_INFO_EX percentage;
	UINT CurrentPState;
	PCISPEED MaxPCIESpeed;
	PCISPEED CurrentPCIESpeed;
	NVIDIA_USAGE Nvidia_Usage;
	NV_GPU_PERF_PSTATES20_INFO NV_PState;	
// 			P0/P1 - Maximum 3D performance 
// 			P2 / P3 - Balanced 3D performance - power
// 			P8 - Basic HD video playback
// 			P10 - DVD playback
// 			P12 - Minimum idle power consumption
}NvidiaInfo;

struct IntelDeviceInfoV1
{
	DWORD GPUMaxFreq;
	DWORD GPUMinFreq;
};

struct IntelDeviceInfoV2
{
	DWORD GPUMaxFreq;
	DWORD GPUMinFreq;
	DWORD GTGeneration;
	DWORD EUCount;
	DWORD PackageTDP;
	DWORD MaxFillRate;
};

struct IntelDeviceInfoHeader
{
	DWORD Size;
	DWORD Version;
};

struct IntelGPUInfo
{
	std::wstring FullName;
	unsigned int VendorId;
	unsigned int DeviceId;
	unsigned __int64 VideoMemory;
	IntelDeviceInfoV2 DeviceInfo;
};

enum GPUTypes
{
	OTHERS_GPU = 0,
	NVIDIA_GPU = 1,
	AMD_GPU = 2,
	INTEL_GPU = 4,
	NVIDIA_AMD = 3,
	NVIDIA_I = 5,
	AMD_INTEL = 6,
	N_A_INTEL = 7
};

class CGPU
{
public:
	CGPU();
	virtual ~CGPU();
	virtual GPUTypes exec() = 0;
	virtual GPUTypes UpdateData() = 0;
	virtual const void* Returninfo() = 0;
	const std::string GetDriverVersion();
	const std::string GetBranchVersion();
protected:
	GPUTypes types;
	std::string DriverVer;
	std::string BranchVersion;
private:
	explicit CGPU(const CGPU& x);
	CGPU& operator=(const CGPU& x);
};

class GPUData
{
public:
	GPUData(); 
	~GPUData();
	void UpdateData();
	const std::vector<AMDINFO>* amdinfo;
	const std::vector<NvidiaInfo> *nvinfo;
	const std::vector<IntelGPUInfo> *Intelinfo;
	std::string NV_DriverVer;
	std::string NV_BranchVersion;

	std::string AMD_DriverVer;
	std::string AMD_BranchVersion;

	std::string IN_DriverVer;
	std::string IN_BranchVersion;
private:
	int 			Types;
	std::shared_ptr<CGPU> nv_gpu;
	std::shared_ptr<CGPU> amd_gpu;
	std::shared_ptr<CGPU> intel_gpu;
	explicit GPUData(const GPUData& x);
	GPUData& operator=(const GPUData& x);
};