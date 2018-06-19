#pragma once
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "NVAPI/nvapi.h"
#include "ADL/adl_sdk.h"

using namespace std;
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
	AdapterInfo						adapterInfo;
	string							adapterActive;
	CHAR							Aspects[ADL_MAX_CHAR];
	ADLBiosInfo						biosInfo;
	int								ASICFamilyTypes;
	int								ASICFamilyValids;
	int								AdapterSpeedCurrent;
	int								AdapterSpeedDefault;
	string							AdapterAccessibility;
	int								AdapterID;
	ADLAdapterCapsX2				adapterCaps;

	// Display
// 	int								NumberOfControllers;
// 	int								NumberOfDisplays;
// 	int								Connections;
// 	ADLDisplayInfo					DisplayInfo;
// 	ADLDisplayDPMSTInfo				DisplayDPMSTInfo;
// 	ADLAdapterODClockInfo			OdClockInfo;
// 	ADLMVPUCaps						MvpuCaps;
// 	ADLMVPUStatus					MvpuStatus;
// 	ADLDisplayTarget				DisplayTarget;
// 	ADLDisplayMap					DisplayMap;
	OVERDRIVE5						OverDrive5;
//	vector<AMDISPLAY>				Amdisplay;
}AMDINFO;

typedef struct _NvidiaInfo
{
	string InterfaceVersion;
	string DriverVersion;
	string BranchVersion;
	string FUllName;
	string GPUType;
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
	BOOL HDCP;
	ULONG physcount;
	UINT corecount;
	NV_GPU_DYNAMIC_PSTATES_INFO_EX percentage;
	string VbiosVersion;
	string VbiosRevision; 
	string VbiosOEMRevision;
	ULONG BusID;
	ULONG BuSlotID;
	string BusType;
	ULONG AGPAperture;
	ULONG AGPRate;
	string SysType;
	ULONG PhysicalFrame;
	ULONG VirtualFrame;
	union 
	{
		NV_GPU_PERF_PSTATES20_INFO NV_PState;	
// 			P0/P1 - Maximum 3D performance 
// 			P2 / P3 - Balanced 3D performance - power
// 			P8 - Basic HD video playback
// 			P10 - DVD playback
// 			P12 - Minimum idle power consumption
	}PState;
	UINT CurrentPState;
	NV_CHIPSET_INFO Chipset;
	ULONG CurrentPCIEWidth;
	NVIDIA_USAGE Nvidia_Usage;
	NV_GPU_ECC_CONFIGURATION_INFO ECC_Config;
}NvidiaInfo;

struct IntelGPUInfo
{
	wstring FUllName;
	struct IntelDeviceInfoV2
	{
		DWORD GPUMaxFreq;
		DWORD GPUMinFreq;
		DWORD GTGeneration;
		DWORD EUCount;
		DWORD PackageTDP;
		DWORD MaxFillRate;
	};
};

class CGPU
{
public:
	enum GPUTypes
	{
		NVIDIA_GPU = 0,
		AMD_GPU = 1,
		INTEL_GPU = 2,
		OTHERS_GPU = 0xFF
	};
public:
	CGPU();
	virtual ~CGPU();
	virtual GPUTypes exec() = 0;
	virtual GPUTypes UpdateData() = 0;
	virtual const void* Returninfo() = 0;
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
	const AMDINFO* amdinfo;
	const NvidiaInfo *nvinfo;
	const IntelGPUInfo* Intelinfo;
private:
	tr1::shared_ptr<CGPU> m_gpu;
	explicit GPUData(const GPUData& x);
	GPUData& operator=(const GPUData& x);
};