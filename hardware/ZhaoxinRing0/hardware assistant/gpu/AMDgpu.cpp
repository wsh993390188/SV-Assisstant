#include "stdafx.h"
#include "AMDgpu.h"
#include "..\driver\DriverOrigin.h"

#include <InitGuid.h>
#include <D3D11.h>
#include <D3DCommon.h>
#ifdef _WIN32_WINNT_WIN10 
#include <DXGI1_4.h>
#else
#include <DXGI1_3.h>
#endif

#include <oleauto.h>
#include <wbemidl.h>
#include <ObjBase.h>

CAMD::CAMD() : re(OTHERS_GPU)
{
	AmdInfo = {};
	BOOL status = ADL_ERR;
	status = InitializeADL();
#ifdef ZX_OutputLog
	Logger::Instance()->OutputLogInfo(el::Level::Debug, "Initiallize AMD ADL Library");
#endif 
	if (status == ADL_OK)
	{
#ifdef ZX_OutputLog
		Logger::Instance()->OutputLogInfo(el::Level::Debug, "Initiallize AMD ADL Library Success");
#endif 
	}
#ifdef ZX_OutputLog
	else
	{
		Logger::Instance()->OutputLogInfo(el::Level::Debug, "Initiallize AMD ADL Library Failed");
		switch (status)
		{
		case -1:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "Generic Error. Most likely one or more of the Escape calls to the driver failed!");
			break;
		case -2:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "ADL not initialized");
			break;
		case -3:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "One of the parameter passed is invalid");
			break;
		case -4:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "One of the parameter size is invalid");
			break;
		case -5:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "Invalid ADL index passed");
			break;
		case -6:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "Invalid controller index passed");
			break;
		case -7:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "Invalid display index passed");
			break;
		case -8:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "Function  not supported by the driver");
			break;
		case -9:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "Null Pointer error");
			break;
		case -10:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "Call can't be made due to disabled adapter");
			break;
		case -11:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "Invalid Callback");
			break;
		case -12:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "Display Resource conflict");
			break;
		case -20:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "Failed to update some of the values. Can be returned by set request that include multiple values if not all values were successfully committed.");
			break;
		case -21:
			Logger::Instance()->OutputLogInfo(el::Level::Debug, "There's no Linux XDisplay in Linux Console environment");
			break;
		default:
			break;
		}
	}
#endif 
}

CAMD::~CAMD()
{
	DestoryADL();
}

GPUTypes CAMD::exec()
{
	GetBaseInfo();
	GetAdapterinfo();
	return re;
}

const void* CAMD::Returninfo()
{
	return &this->AmdInfo;
}

GPUTypes CAMD::UpdateData()
{
	AmdInfo.clear();
	if (re == AMD_GPU)
		GetAdapterinfo();
	return re;
}

// Memory allocation function
void* __stdcall CAMD::ADL_Main_Memory_Alloc(int iSize)
{
	void* lpBuffer = malloc(iSize);
	return lpBuffer;
}

// Optional Memory de-allocation function
void __stdcall CAMD::ADL_Main_Memory_Free(void** lpBuffer)
{
	if (NULL != *lpBuffer)
	{
		free(*lpBuffer);
		*lpBuffer = NULL;
	}
}


BOOL CAMD::InitializeADL()
{
	hDLL = LoadLibraryEx(_T("atiadlxx.dll"),NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hDLL == NULL)
		// A 32 bit calling application on 64 bit OS will fail to LoadLIbrary.
		// Try to load the 32 bit library (atiadlxy.dll) instead
		hDLL = LoadLibraryEx(_T("atiadlxy.dll"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (NULL == hDLL)
	{
		OutputDebugString(_T("ADL library not found!\n"));
		return ADL_ERR;
	}
	int adl_err = ADL_ERR;
	ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE)GetProcAddress(hDLL, "ADL_Main_Control_Create");
	ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY)GetProcAddress(hDLL, "ADL_Main_Control_Destroy");
	ADL_Graphics_Versions_Get = (ADL_GRAPHICS_VERSIONS_GET)GetProcAddress(hDLL, "ADL_Graphics_Versions_Get");
	ADL_Graphics_Platform_Get = (ADL_GRAPHICS_PLATFORM_GET)GetProcAddress(hDLL, "ADL_Graphics_Platform_Get");
	ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET)GetProcAddress(hDLL, "ADL_Adapter_NumberOfAdapters_Get");
	ADL_Adapter_Active_Get = (ADL_ADAPTER_ACTIVE_GET)GetProcAddress(hDLL, "ADL_Adapter_Active_Get");
	ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET)GetProcAddress(hDLL, "ADL_Adapter_AdapterInfo_Get");
	ADL_Adapter_Aspects_Get = (ADL_ADAPTER_ASPECTS_GET)GetProcAddress(hDLL, "ADL_Adapter_Aspects_Get");
	ADL_Adapter_ASICFamilyType_Get = (ADL_ADAPTER_ASICFAMILYTYPE_GET)GetProcAddress(hDLL, "ADL_Adapter_ASICFamilyType_Get");
	ADL_Adapter_Speed_Caps = (ADL_ADAPTER_SPEED_CAPS)GetProcAddress(hDLL, "ADL_Adapter_Speed_Caps");
	ADL_Adapter_Speed_Get = (ADL_ADAPTER_SPEED_GET)GetProcAddress(hDLL, "ADL_Adapter_Speed_Get");
	ADL_Adapter_VideoBiosInfo_Get = (ADL_ADAPTER_VIDEOBIOSINFO_GET)GetProcAddress(hDLL, "ADL_Adapter_VideoBiosInfo_Get");
	ADL_Adapter_Accessibility_Get = (ADL_ADAPTER_ACCESSIBILITY_GET)GetProcAddress(hDLL, "ADL_Adapter_Accessibility_Get");
	ADL_Adapter_ID_Get = (ADL_ADAPTER_ID_GET)GetProcAddress(hDLL, "ADL_Adapter_ID_Get");
	ADL_AdapterX2_Caps = (ADL_ADAPTERX2_CAPS)GetProcAddress(hDLL, "ADL_AdapterX2_Caps");
	ADL_Display_DisplayInfo_Get = (ADL_DISPLAY_DISPLAYINFO_GET)GetProcAddress(hDLL, "ADL_Display_DisplayInfo_Get");
	ADL_Display_DpMstInfo_Get = (ADL_DISPLAY_DPMSTINFO_GET)GetProcAddress(hDLL, "ADL_Display_DpMstInfo_Get");
	ADL_Display_PreservedAspectRatio_Get = (ADL_DISPLAY_PRESERVEDASPECTRATIO_GET)GetProcAddress(hDLL, "ADL_Display_PreservedAspectRatio_Get");
	ADL_Display_ImageExpansion_Get = (ADL_DISPLAY_IMAGEEXPANSION_GET)GetProcAddress(hDLL, "ADL_Display_ImageExpansion_Get");
	ADL_Display_Position_Get = (ADL_DISPLAY_POSITION_GET)GetProcAddress(hDLL, "ADL_Display_Position_Get");
	ADL_Display_Size_Get = (ADL_DISPLAY_SIZE_GET)GetProcAddress(hDLL, "ADL_Display_Size_Get");
	ADL_Display_AdjustCaps_Get = (ADL_DISPLAY_ADJUSTCAPS_GET)GetProcAddress(hDLL, "ADL_Display_AdjustCaps_Get");
	ADL_Display_Capabilities_Get = (ADL_DISPLAY_CAPABILITIES_GET)GetProcAddress(hDLL, "ADL_Display_Capabilities_Get");
	ADL_Display_ConnectedDisplays_Get = (ADL_DISPLAY_CONNECTEDDISPLAYS_GET)GetProcAddress(hDLL, "ADL_Display_ConnectedDisplays_Get");
	ADL_Display_DeviceConfig_Get = (ADL_DISPLAY_DEVICECONFIG_GET)GetProcAddress(hDLL, "ADL_Display_DeviceConfig_Get");
	ADL_Display_Property_Get = (ADL_DISPLAY_PROPERTY_GET)GetProcAddress(hDLL, "ADL_Display_Property_Get");
	ADL_Display_SwitchingCapability_Get = (ADL_DISPLAY_SWITCHINGCAPABILITY_GET)GetProcAddress(hDLL, "ADL_Display_SwitchingCapability_Get");
	ADL_Display_DitherState_Get = (ADL_DISPLAY_DITHERSTATE_GET)GetProcAddress(hDLL, "ADL_Display_DitherState_Get");
	ADL_Display_SupportedPixelFormat_Get = (ADL_DISPLAY_SUPPORTEDPIXELFORMAT_GET)GetProcAddress(hDLL, "ADL_Display_SupportedPixelFormat_Get");
	ADL_Display_PixelFormat_Get = (ADL_DISPLAY_PIXELFORMAT_GET)GetProcAddress(hDLL, "ADL_Display_PixelFormat_Get");
	ADL_Display_SupportedColorDepth_Get = (ADL_DISPLAY_SUPPORTEDCOLORDEPTH_GET)GetProcAddress(hDLL, "ADL_Display_SupportedColorDepth_Get");
	ADL_Display_ColorDepth_Get = (ADL_DISPLAY_COLORDEPTH_GET)GetProcAddress(hDLL, "ADL_Display_ColorDepth_Get");
	ADL_Display_ODClockInfo_Get = (ADL_DISPLAY_ODCLOCKINFO_GET)GetProcAddress(hDLL, "ADL_Display_ODClockInfo_Get");
	ADL_Display_AdjustmentCoherent_Get = (ADL_DISPLAY_ADJUSTMENTCOHERENT_GET)GetProcAddress(hDLL, "ADL_Display_AdjustmentCoherent_Get");
	ADL_Display_ReducedBlanking_Get = (ADL_DISPLAY_REDUCEDBLANKING_GET)GetProcAddress(hDLL, "ADL_Display_ReducedBlanking_Get");
	ADL_Display_FormatsOverride_Get = (ADL_DISPLAY_FORMATSOVERRIDE_GET)GetProcAddress(hDLL, "ADL_Display_FormatsOverride_Get");
	ADL_Display_MVPUCaps_Get = (ADL_DISPLAY_MVPUCAPS_GET)GetProcAddress(hDLL, "ADL_Display_MVPUCaps_Get");
	ADL_Display_MVPUStatus_Get = (ADL_DISPLAY_MVPUSTATUS_GET)GetProcAddress(hDLL, "ADL_Display_MVPUStatus_Get");
	ADL_Display_Modes_Get = (ADL_DISPLAY_MODES_GET)GetProcAddress(hDLL, "ADL_Display_Modes_Get");
	ADL_Display_DisplayMapConfig_Get = (ADL_DISPLAY_DISPLAYMAPCONFIG_GET)GetProcAddress(hDLL, "ADL_Display_DisplayMapConfig_Get");
	ADL_Display_ModeTimingOverrideX2_Get = (ADL_DISPLAY_MODETIMINGOVERRIDEX2_GET)GetProcAddress(hDLL, "ADL_Display_ModeTimingOverrideX2_Get");
	ADL_Adapter_ModeTimingOverride_Caps = (ADL_ADAPTER_MODETIMINGOVERRIDE_CAPS)GetProcAddress(hDLL, "ADL_Adapter_ModeTimingOverride_Caps");
	ADL_Overdrive5_ThermalDevices_Enum = (ADL_OVERDRIVE5_THERMALDEVICES_ENUM)GetProcAddress(hDLL, "ADL_Overdrive5_ThermalDevices_Enum");
	ADL_Overdrive5_ODParameters_Get = (ADL_OVERDRIVE5_ODPARAMETERS_GET)GetProcAddress(hDLL, "ADL_Overdrive5_ODParameters_Get");
	ADL_Overdrive5_Temperature_Get = (ADL_OVERDRIVE5_TEMPERATURE_GET)GetProcAddress(hDLL, "ADL_Overdrive5_Temperature_Get");
	ADL_Overdrive5_FanSpeed_Get = (ADL_OVERDRIVE5_FANSPEED_GET)GetProcAddress(hDLL, "ADL_Overdrive5_FanSpeed_Get");
	ADL_Overdrive5_FanSpeedInfo_Get = (ADL_OVERDRIVE5_FANSPEEDINFO_GET)GetProcAddress(hDLL, "ADL_Overdrive5_FanSpeedInfo_Get");
	ADL_Overdrive5_ODPerformanceLevels_Get = (ADL_OVERDRIVE5_ODPERFORMANCELEVELS_GET)GetProcAddress(hDLL, "ADL_Overdrive5_ODPerformanceLevels_Get");
	ADL_Overdrive5_CurrentActivity_Get = (ADL_OVERDRIVE5_CURRENTACTIVITY_GET)GetProcAddress(hDLL, "ADL_Overdrive5_CurrentActivity_Get");
	ADL_Overdrive5_FanSpeed_Set = (ADL_OVERDRIVE5_FANSPEED_SET)GetProcAddress(hDLL, "ADL_Overdrive5_FanSpeed_Set");
	ADL_Overdrive5_ODPerformanceLevels_Set = (ADL_OVERDRIVE5_ODPERFORMANCELEVELS_SET)GetProcAddress(hDLL, "ADL_Overdrive5_ODPerformanceLevels_Set");
	ADL_Overdrive5_PowerControl_Caps = (ADL_OVERDRIVE5_POWERCONTROL_CAPS)GetProcAddress(hDLL, "ADL_Overdrive5_PowerControl_Caps");
	ADL_Overdrive5_PowerControlInfo_Get = (ADL_OVERDRIVE5_POWERCONTROLINFO_GET)GetProcAddress(hDLL, "ADL_Overdrive5_PowerControlInfo_Get");
	ADL_Overdrive5_PowerControl_Get = (ADL_OVERDRIVE5_POWERCONTROL_GET)GetProcAddress(hDLL, "ADL_Overdrive5_PowerControl_Get");
	ADL_Overdrive5_PowerControl_Set = (ADL_OVERDRIVE5_POWERCONTROL_SET)GetProcAddress(hDLL, "ADL_Overdrive5_PowerControl_Set");
	ADL_Overdrive6_State_Set = (ADL_OVERDRIVE6_STATE_SET)GetProcAddress(hDLL, "ADL_Overdrive6_State_Set");
	if (ADL_Main_Control_Create)
	{
		adl_err = ADL_Main_Control_Create(this->ADL_Main_Memory_Alloc, 1);
	}
	return adl_err;
}

void CAMD::DestoryADL()
{
	if (hDLL)
	{
		ADL_Main_Control_Destroy();
		FreeLibrary(hDLL);
	}
}

void CAMD::GetBaseInfo()
{
	if (NULL != ADL_Graphics_Versions_Get)
	{
		ADLVersionsInfo	Versioninfo = {};
		ADL_Graphics_Versions_Get(&Versioninfo);
		DriverVer = Versioninfo.strDriverVer;
		BranchVersion = Versioninfo.strCatalystVersion;
	}
	int Platform = 0;
	if (NULL != ADL_Graphics_Platform_Get)
	{
		ADL_Graphics_Platform_Get(&Platform);
	}
}

void CAMD::GetAdapterinfo()
{
	int iAdapterNumbers = 0;
	int iActiveID = 0;
	int ADL_ERROR_Code = ADL_ERR;
	LPAdapterInfo lpAdapterInfo = NULL;
	if (ADL_Adapter_NumberOfAdapters_Get)
	{
		ADL_ERROR_Code = ADL_Adapter_NumberOfAdapters_Get(&iAdapterNumbers);
		if (ADL_ERROR_Code == ADL_OK)
		{
			lpAdapterInfo = new AdapterInfo[iAdapterNumbers];
			if (ADL_Adapter_AdapterInfo_Get && 0 < iAdapterNumbers)
			{
				memset(lpAdapterInfo, '\0', sizeof(AdapterInfo) * iAdapterNumbers);
				ADL_ERROR_Code = ADL_Adapter_AdapterInfo_Get(lpAdapterInfo, sizeof(AdapterInfo) * iAdapterNumbers);
			}
			if (iAdapterNumbers)
				re = AMD_GPU;
			for (int i = 0; i < iAdapterNumbers; ++i)
			{
				bool pass = false;
				AMDINFO info = {};
				AdapterInfo adapterInfo = lpAdapterInfo[i];
				int adapterActive = 0;
				ADL_Adapter_Active_Get(adapterInfo.iAdapterIndex, &adapterActive);
				
				if(adapterInfo.iVendorID == 1002 || adapterInfo.iVendorID == 0x1002) // 1002ÊÇË­Ï¹Ð´½ø¿âµÄ À¬»øAMD
				{
					for each (auto var in AmdInfo)
					{
						if (var.iBusNumber == adapterInfo.iBusNumber &&
							var.iDeviceNumber == adapterInfo.iDeviceNumber)
							pass = true;
					}
					if(pass)
						continue;
					ULONG   ven, dev, subsys, rev;
					ven = dev = subsys = rev = 0;

					sscanf_s(adapterInfo.strPNPString,
						"PCI\\VEN_%x&DEV_%x&SUBSYS_%x&REV_%x",
						&ven, &dev, &subsys, &rev);
					info.iBusNumber = adapterInfo.iBusNumber;
					info.iVendorID = ven;
					info.iDeviceNumber = adapterInfo.iDeviceNumber;
					info.iFunctionNumber = adapterInfo.iFunctionNumber;
					info.FullName = adapterInfo.strAdapterName;
					info.iDeviceId = dev;
					adapterActive ? info.adapterActive = "yes" : info.adapterActive = "no";

					GetAdapterMemory(info);

					if (ADL_Adapter_Aspects_Get)
					{
						int iSize = ADL_MAX_CHAR;
						CHAR lpAspects[ADL_MAX_CHAR];
						ADL_ERROR_Code = ADL_Adapter_Aspects_Get(i, lpAspects, iSize);
						info.Aspects = lpAspects;
					}

					if (ADL_Adapter_VideoBiosInfo_Get)
					{
						ADLBiosInfo biosInfo;
						if (ADL_OK == ADL_Adapter_VideoBiosInfo_Get(i, &biosInfo)) {
							info.biosInfo = biosInfo;
						}
					}

					if (ADL_Adapter_ID_Get)
					{
						int lpAdapterID = 0;
						if (ADL_OK == ADL_Adapter_ID_Get(i, &lpAdapterID))
						{
							info.AdapterID = lpAdapterID;
						}
					}

					if (ADL_AdapterX2_Caps)
					{
						ADLAdapterCapsX2 adapterCaps;
						if (ADL_OK == ADL_AdapterX2_Caps(i, &adapterCaps))
						{
							info.adapterCaps = adapterCaps;
						}
					}
					GetOverDrive5(i, info);
					AmdInfo.emplace_back(info);
				}
			}
		}
	}

	if (lpAdapterInfo)
	{	
		delete[] lpAdapterInfo;
		lpAdapterInfo = NULL;
	}
}

#pragma region PCI Function
BOOL CAMD::GetPCIESpeed(AMDINFO & amdinfo)
{
	PCI_COMMON_CONFIG pci = {};
	if (ZhaoxinDriver::Instance()->ReadPci(amdinfo.iBusNumber, amdinfo.iDeviceNumber, amdinfo.iFunctionNumber, pci) == 0)
	{
		UCHAR* capaaddr = reinterpret_cast<UCHAR*>(&pci);
		USHORT CapabilitiesPtr = pci.u.type0.CapabilitiesPtr;
		bool IsPCIE10H = false;
		while (true)
		{
			if (capaaddr[CapabilitiesPtr] == 0x10)
			{
				IsPCIE10H = true;
				break;
			}
			if (capaaddr[CapabilitiesPtr + 1] == 0x00)
				break;
			CapabilitiesPtr = capaaddr[CapabilitiesPtr + 1];
		}
		if (IsPCIE10H)
		{
			amdinfo.MaxPCIESpeed.LinkSpeed = capaaddr[CapabilitiesPtr + 0x0C] & 0xFF;
			USHORT temp = capaaddr[CapabilitiesPtr + 0x0C] | (capaaddr[CapabilitiesPtr + 0x0D] << 8);
			amdinfo.MaxPCIESpeed.LinkWidth = (temp & 0x3F0) >> 4;

			amdinfo.CurrentPCIESpeed.LinkSpeed = capaaddr[CapabilitiesPtr + 0x12] & 0xFF;
			temp = capaaddr[CapabilitiesPtr + 0x12] | (capaaddr[CapabilitiesPtr + 0x13] << 8);
			amdinfo.CurrentPCIESpeed.LinkWidth = (temp & 0x3F0) >> 4;
			return TRUE;
		}
	}
	return FALSE;
}
#pragma endregion

BOOL CAMD::GetOverDrive5(int adapterId, AMDINFO& info)
{
	int  ADL_Err = ADL_ERR;
	int  iNumberAdapters = 0;
	int  iOverdriveSupported = 0;
	int  iOverdriveEnabled = 0;
	ADLFanSpeedInfo fanSpeedInfo = { 0 };
	int	 iOverdriveVersion = 0;
	int fanSpeedReportingMethod = 0;
	int maxThermalControllerIndex = 0;
	ADLODPerformanceLevels* performanceLevels = 0;
	int powerControlSupported = 0;
	ADLPowerControlInfo powerControlInfo = { 0 };
	int powerControlCurrent = 0;
	int powerControlDefault = 0;
	ADLThermalControllerInfo termalControllerInfo = { 0 };
	termalControllerInfo.iSize = sizeof(ADLThermalControllerInfo);
	for (int iThermalControllerIndex = 0; iThermalControllerIndex < 10; iThermalControllerIndex++)
	{
		TEM_AND_FAN temandfan = {};
		ADL_Err = ADL_Overdrive5_ThermalDevices_Enum(adapterId, iThermalControllerIndex, &termalControllerInfo);

		if (ADL_Err == ADL_WARNING_NO_DATA)
		{
			maxThermalControllerIndex = iThermalControllerIndex;
			break;
		}

		if (termalControllerInfo.iThermalDomain == ADL_DL_THERMAL_DOMAIN_GPU)
		{
			ADLTemperature adlTemperature = { 0 };
			adlTemperature.iSize = sizeof(ADLTemperature);
			if (ADL_OK != ADL_Overdrive5_Temperature_Get(adapterId, iThermalControllerIndex, &adlTemperature))
			{

			}
			int temperatureInDegreesCelsius = adlTemperature.iTemperature / 1000; // The temperature is returned in millidegrees Celsius.
			temandfan.ThermalControllerIndex = iThermalControllerIndex;
			temandfan.adlTemperature = adlTemperature;
			temandfan.temperatureInDegreesCelsius = temperatureInDegreesCelsius;

			fanSpeedInfo.iSize = sizeof(ADLFanSpeedInfo);
			if (ADL_OK == ADL_Overdrive5_FanSpeedInfo_Get(adapterId, iThermalControllerIndex, &fanSpeedInfo))
			{
				ADLFanSpeedValue fanSpeedValue = { 0 };
				fanSpeedReportingMethod = ((fanSpeedInfo.iFlags & ADL_DL_FANCTRL_SUPPORTS_PERCENT_READ) == ADL_DL_FANCTRL_SUPPORTS_PERCENT_READ) ? ADL_DL_FANCTRL_SPEED_TYPE_PERCENT : ADL_DL_FANCTRL_SPEED_TYPE_RPM;
				//Set to ADL_DL_FANCTRL_SPEED_TYPE_RPM or to ADL_DL_FANCTRL_SPEED_TYPE_PERCENT to request fan speed to be returned in rounds per minute or in percentage points.
				//Note that the call might fail if requested fan speed reporting method is not supported by the GPU.
				fanSpeedValue.iSpeedType = fanSpeedReportingMethod;
				fanSpeedValue.iSize = sizeof(ADLFanSpeedValue);
				ADL_Err = ADL_Overdrive5_FanSpeed_Get(adapterId, iThermalControllerIndex, &fanSpeedValue);
				if (ADL_OK == ADL_Err)
				{
					temandfan.fanSpeedValue = fanSpeedValue;
					info.OverDrive5.TemperatureAndFans = temandfan;
				}
			}
		}
	}

	if (ADL_OK == ADL_Overdrive5_PowerControl_Caps(adapterId, &powerControlSupported))
	{
		if (powerControlSupported)
		{
			if (ADL_OK == ADL_Overdrive5_PowerControlInfo_Get(adapterId, &powerControlInfo))
			{
				if (ADL_OK == ADL_Overdrive5_PowerControl_Get(adapterId, &powerControlCurrent, &powerControlDefault))
				{
					info.OverDrive5.powerControlInfo = powerControlInfo;
					info.OverDrive5.powerControlCurrent = powerControlCurrent;
					info.OverDrive5.powerControlDefault = powerControlDefault;
				}
			}
#ifdef _DEBUG
			else
				OutputDebugString(_T("Failed to get Power Controls Info\n"));
#endif // _DEBUG
		}
	}

	


	ADLODParameters overdriveParameters = { 0 };
	overdriveParameters.iSize = sizeof(ADLODParameters);

	if (ADL_OK == ADL_Overdrive5_ODParameters_Get(adapterId, &overdriveParameters))
	{
		info.OverDrive5.overdriveParameters = overdriveParameters;
	}

	if (overdriveParameters.iNumberOfPerformanceLevels > 0)
	{
		//Overdrive5 GPUs support few performance levels. Application can set distinct engine clocks, memory clocks, core voltage 
		//for each of the performance levels. Performance level with index 0 corresponds to lowest performance system state. 
		//Performance level with highest index corresponds to highest performance system state ?3D game playing for example.
		//Users are usually interested in overclocking highest index performance level.

		int size = sizeof(ADLODPerformanceLevels) + sizeof(ADLODPerformanceLevel) * (overdriveParameters.iNumberOfPerformanceLevels - 1);
		void* performanceLevelsBuffer = malloc(size);
		memset(performanceLevelsBuffer, 0, size);
		performanceLevels = (ADLODPerformanceLevels*)performanceLevelsBuffer;
		performanceLevels->iSize = size;

		if (ADL_OK == ADL_Overdrive5_ODPerformanceLevels_Get(adapterId, 1/*Getting default values first*/, performanceLevels))
		{
			for (int i = 0; i < overdriveParameters.iNumberOfPerformanceLevels; i++)
			{
				info.OverDrive5.DefaultperformanceLevels[i] = performanceLevels->aLevels[i];
			}
			memset(performanceLevelsBuffer, 0, size);
			performanceLevels->iSize = size;

			if (ADL_OK == ADL_Overdrive5_ODPerformanceLevels_Get(adapterId, 0/*Getting current values first*/, performanceLevels))
			{
				for (int i = 0; i < overdriveParameters.iNumberOfPerformanceLevels; i++)
				{
					info.OverDrive5.CurrentperformanceLevels[i] = performanceLevels->aLevels[i];
				}
			}
			if (performanceLevelsBuffer)
			{
				free(performanceLevelsBuffer);
				performanceLevelsBuffer = NULL;
			}
		}
	}

	//Getting real current values for clocks, performance levels, voltage effective in the system.
	ADLPMActivity activity = { 0 };
	activity.iSize = sizeof(ADLPMActivity);
	if (ADL_OK == ADL_Overdrive5_CurrentActivity_Get(adapterId, &activity))
	{
		info.OverDrive5.activity = activity;
	}

// 	if (overdriveParameters.iActivityReportingSupported)
// 	{
// 		printf("Current Engine Clock: %d persent\n", activity.iActivityPercent);
// 	}
	return TRUE;
}

BOOL CAMD::GetAdapterMemory(AMDINFO& info)
{
	//
	// DXGI is supported on Windows Vista and later. Define a function pointer to the
	// CreateDXGIFactory function. DXGIFactory1 is supported by Windows Store Apps so
	// try that first.
	//
	HMODULE hDXGI = LoadLibrary(L"dxgi.dll");
	if (hDXGI == NULL) {
		return false;
	}

	typedef HRESULT(WINAPI*LPCREATEDXGIFACTORY)(REFIID riid, void** ppFactory);

	LPCREATEDXGIFACTORY pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory1");
	if (pCreateDXGIFactory == NULL) {
		pCreateDXGIFactory = (LPCREATEDXGIFACTORY)GetProcAddress(hDXGI, "CreateDXGIFactory");

		if (pCreateDXGIFactory == NULL) {
			FreeLibrary(hDXGI);
			return false;
		}
	}

	//
	// We have the CreateDXGIFactory function so use it to actually create the factory and enumerate
	// through the adapters. Here, we are specifically looking for the Intel gfx adapter. 
	//
	IDXGIAdapter*     pAdapter;
	IDXGIFactory*     pFactory;
	DXGI_ADAPTER_DESC AdapterDesc;
	if (FAILED((*pCreateDXGIFactory)(__uuidof(IDXGIFactory), (void**)(&pFactory)))) {
		FreeLibrary(hDXGI);
		return false;
	}

	if (FAILED(pFactory->EnumAdapters(0, (IDXGIAdapter**)&pAdapter))) {
		FreeLibrary(hDXGI);
		return false;
	}

	unsigned int intelAdapterIndex = 0;
	while (pFactory->EnumAdapters(intelAdapterIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
		pAdapter->GetDesc(&AdapterDesc);
		if (AdapterDesc.VendorId == info.iVendorID && AdapterDesc.DeviceId == info.iDeviceId) {
			break;
		}
		intelAdapterIndex++;
	}

	if (pAdapter == NULL) {
		FreeLibrary(hDXGI);
		return false;
	}

	//
	// If we are on Windows 10 then the Adapter3 interface should be available. This is recommended over using
	// the AdapterDesc for getting the amount of memory available.
	//
// #ifdef _WIN32_WINNT_WIN10
// 	IDXGIAdapter3* pAdapter3;
// 	pAdapter->QueryInterface(IID_IDXGIAdapter3, (void**)&pAdapter3);
// 	if (pAdapter3) {
// 		DXGI_QUERY_VIDEO_MEMORY_INFO memInfo;
// 		pAdapter3->QueryVideoMemoryInfo(intelAdapterIndex, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo);
// 		info.sharedSystemMemory = memInfo.;
// 		info.dedicatedVideoMemory = AdapterDesc.DedicatedVideoMemory;
// 		info.systemVideoMemory = AdapterDesc.DedicatedSystemMemory;
// 		pAdapter3->Release();
// 	}
// 	else
// #endif // DEBUG
	{
		info.sharedSystemMemory = AdapterDesc.SharedSystemMemory;
		info.dedicatedVideoMemory = AdapterDesc.DedicatedVideoMemory;
		info.systemVideoMemory = AdapterDesc.DedicatedSystemMemory;
	}


	pAdapter->Release();
	FreeLibrary(hDXGI);
	return true;
}
// void CAMD::GetDisplay(int AdapterNum, AMDINFO& info)
// {
// 	int lpNumDisplays = 0;
// 	int ADL_ERROR_Code = ADL_ERR;
// 	LPADLDisplayInfo DisplayInfo = NULL;
// 	LPADLDisplayDPMSTInfo DisplayDPMSTInfo = NULL;
// 	LPADLDisplayMap lppDisplayMap = NULL;
// 	LPADLDisplayTarget lppDisplayTarget = NULL;
// 	if (ADL_Display_Capabilities_Get)
// 	{
// 		int lpNumberOfControlers = 0, lpNumberOfDisplays = 0;
// 		if (ADL_OK == ADL_Display_Capabilities_Get(AdapterNum, &lpNumberOfControlers, &lpNumberOfDisplays))
// 		{
// //			cout << "get the number of displays and controllers supported by an adapter" << endl;
// 			info.NumberOfControllers = lpNumberOfControlers;
// 			info.NumberOfDisplays = lpNumberOfDisplays;
// 		}
// 	}
// 
// 	if (ADL_Display_ConnectedDisplays_Get)
// 	{
// 		int lpConnections = 0;
// 		if (ADL_OK == ADL_Display_ConnectedDisplays_Get(AdapterNum, &lpConnections))
// 		{
// //			cout << " indicate whether displays are physically connected to an adapter." << endl;
// 			info.Connections = lpConnections;
// 		}
// 	}
// 
// 	if (ADL_Display_DisplayInfo_Get)
// 	{
// 		ADL_ERROR_Code = ADL_Display_DisplayInfo_Get(AdapterNum, &lpNumDisplays, &DisplayInfo, 0);
// 		if (ADL_OK == ADL_ERROR_Code)
// 		{
// // 			cout << "Display Info" << endl;
// // 			cout << "The display's EDID name:" << DisplayInfo->strDisplayName << endl;
// // 			cout << "The display's manufacturer name:" << DisplayInfo->strDisplayManufacturerName << endl;
// // 			cout << "Display Type:" << DisplayInfo->iDisplayType << endl;
// // 			cout << "Display output Type:" << DisplayInfo->iDisplayOutputType << endl;
// // 			cout << "The connector type for the device:" << DisplayInfo->iDisplayConnector << endl;
// // 			//info.DisplayInfo = *DisplayInfo;
// 			memcpy(&info.DisplayInfo, DisplayInfo, sizeof(ADLDisplayInfo));
// 		}
// 		ADL_Main_Memory_Free((void**)&DisplayInfo);
// 	}
// 
// 	if (ADL_Display_DpMstInfo_Get)
// 	{
// 		int iNumDisplays = 0;
// 		if (ADL_OK == ADL_Display_DpMstInfo_Get(AdapterNum, &iNumDisplays, &DisplayDPMSTInfo, 0))
// 		{
// // 			cout << "Display DPMST Info" << endl;
// // 			cout << "total bandwidth available on the DP connector :" << DisplayDPMSTInfo->iTotalAvailableBandwidthInMpbs << endl;
// // 			cout << "bandwidth allocated to this display :" << DisplayDPMSTInfo->iAllocatedBandwidthInMbps << endl;
// // 			cout << "string identifier for the display :" << DisplayDPMSTInfo->strGlobalUniqueIdentifier << endl;
// // 			cout << "The link count of relative address£º" << DisplayDPMSTInfo->radLinkCount << endl;
// // 			cout << "The physical connector ID£º" << DisplayDPMSTInfo->iPhysicalConnectorID << endl;
// // 			cout << "Relative address£º" << DisplayDPMSTInfo->rad << endl;
// 			memcpy(&info.DisplayDPMSTInfo, DisplayDPMSTInfo, sizeof(ADLDisplayDPMSTInfo));
// 			//info.DisplayDPMSTInfo = *DisplayDPMSTInfo;
// 		}
// 		ADL_Main_Memory_Free((void**)&DisplayDPMSTInfo);
// 	}
// 
// 	if (ADL_Display_ODClockInfo_Get)
// 	{
// 		ADLAdapterODClockInfo  lpOdClockInfo = { 0 };
// 		if (ADL_OK == ADL_Display_ODClockInfo_Get(AdapterNum, &lpOdClockInfo))
// 		{
// // 			cout << "retrieve the OD clock information." << endl;
// // 			cout << "Memory Current Clock:" << lpOdClockInfo.sMemoryClock.iCurrentClock << endl;
// // 			cout << "Memory Default Clock:" << lpOdClockInfo.sMemoryClock.iDefaultClock << endl;
// // 			cout << "Memory Max Clock:" << lpOdClockInfo.sMemoryClock.iMaxClock << endl;
// // 			cout << "Memory Min Clock:" << lpOdClockInfo.sMemoryClock.iMinClock << endl;
// // 			cout << "Memory Requested Clock:" << lpOdClockInfo.sMemoryClock.iRequestedClock << endl;
// // 			cout << "Memory Step Clock:" << lpOdClockInfo.sMemoryClock.iStepClock << endl;
// // 
// // 			cout << "Engine Current Clock:" << lpOdClockInfo.sEngineClock.iCurrentClock << endl;
// // 			cout << "Engine Default Clock:" << lpOdClockInfo.sEngineClock.iDefaultClock << endl;
// // 			cout << "Engine Max Clock:" << lpOdClockInfo.sEngineClock.iMaxClock << endl;
// // 			cout << "Engine Min Clock:" << lpOdClockInfo.sEngineClock.iMinClock << endl;
// // 			cout << "Engine Requested Clock:" << lpOdClockInfo.sEngineClock.iRequestedClock << endl;
// // 			cout << "Engine Step Clock:" << lpOdClockInfo.sEngineClock.iStepClock << endl;
// 			info.OdClockInfo = lpOdClockInfo;
// 		}
// 	}
// 
// 	if (ADL_Display_MVPUCaps_Get)
// 	{
// 		ADLMVPUCaps lpMvpuCaps;
// 		if (ADL_OK == ADL_Display_MVPUCaps_Get(AdapterNum, &lpMvpuCaps))
// 		{
// //			cout << "MVPUCaps" << endl;
// 			info.MvpuCaps = lpMvpuCaps;
// 		}
// 	}
// 
// 	if (ADL_Display_MVPUStatus_Get)
// 	{
// 		ADLMVPUStatus lpMvpuStatus;
// 		if (ADL_OK == ADL_Display_MVPUStatus_Get(AdapterNum, &lpMvpuStatus))
// 		{
// //			cout << "MVPUStatus" << endl;
// 			info.MvpuStatus = lpMvpuStatus;
// 		}
// 	}
// 
// 	if (ADL_Display_DisplayMapConfig_Get)
// 	{
// 		int lpNumDisplayMap = 0, lpNumDisplayTarget = 0;
// 		if (ADL_OK == ADL_Display_DisplayMapConfig_Get(AdapterNum, &lpNumDisplayMap, &lppDisplayMap, &lpNumDisplayTarget, &lppDisplayTarget, 0))
// 		{
// //			cout << "Display Map" << endl;
// 			memcpy(&info.DisplayMap, lppDisplayMap, sizeof(ADLDisplayMap));
// 			memcpy(&info.DisplayTarget, lppDisplayTarget, sizeof(ADLDisplayTarget));
// // 			info.DisplayMap = *lppDisplayMap;
// // 			info.DisplayTarget = *lppDisplayTarget;
// 		}
// 
// 		ADL_Main_Memory_Free((void**)&lppDisplayMap);
// 		ADL_Main_Memory_Free((void**)&lppDisplayTarget);
// 	}
// 
// 	for (int j = 0; j < lpNumDisplays; j++)
// 	{
// 		AMDISPLAY Amdisplay = {};
// 		if (ADL_Display_PreservedAspectRatio_Get)
// 		{
// 			int lpSupport = 0, lpCurrent = 0, lpDefault = 0;
// 			ADL_ERROR_Code = ADL_Display_PreservedAspectRatio_Get(AdapterNum, j, &lpSupport, &lpCurrent, &lpDefault);
// 			if (ADL_OK == ADL_ERROR_Code)
// 			{
// // 				cout << "Display perserved aspect ratio of an adapter.supported values of the display preserved aspect ratio." << endl;
// // 				cout << "Support:" << (lpSupport ? "yes" : "no") << endl;
// // 				cout << "Current:" << (lpCurrent ? "yes" : "no") << endl;
// // 				cout << "Default:" << (lpDefault ? "yes" : "no") << endl;
// 				Amdisplay.PreservedAspectRatio_Current = lpCurrent;
// 				Amdisplay.PreservedAspectRatio_Default = lpDefault;
// 				Amdisplay.PreservedAspectRatio_Support = lpSupport;
// 			}
// 		}
// 
// 		if (ADL_Display_ImageExpansion_Get)
// 		{
// 			int lpSupport = 0, lpCurrent = 0, lpDefault = 0;
// 			if (ADL_OK == ADL_Display_ImageExpansion_Get(AdapterNum, j, &lpSupport, &lpCurrent, &lpDefault))
// 			{
// // 				cout << "display image expansion setting.supported values of the display image expansion setting." << endl;
// // 				cout << "Support:" << (lpSupport ? "yes" : "no") << endl;
// // 				cout << "Current:" << (lpCurrent ? "yes" : "no") << endl;
// // 				cout << "Default:" << (lpDefault ? "yes" : "no") << endl;
// 				Amdisplay.ImageExpansion_Current = lpCurrent;
// 				Amdisplay.ImageExpansion_Default = lpDefault;
// 				Amdisplay.ImageExpansion_Support = lpSupport;
// 			}
// 		}
// 
// 		if (ADL_Display_Position_Get)
// 		{
// 			int lpX = 0, lpY = 0, lpXDefault = 0, lpYDefault = 0, lpMinX = 0, lpMinY = 0, lpMaxX = 0, lpMaxY = 0, lpStepX = 0, lpStepY = 0;
// 			if (ADL_OK == ADL_Display_Position_Get(AdapterNum, j, &lpX, &lpY, &lpXDefault, &lpYDefault, &lpMinX, &lpMinY, &lpMaxX, &lpMaxY, &lpStepX, &lpStepY))
// 			{
// // 				cout << "Device Display Position" << endl;
// // 				cout << "current X coordinate display position:" << lpX << endl;
// // 				cout << "current Y coordinate display position:" << lpY << endl;
// // 				cout << "default X coordinate display position:" << lpXDefault << endl;
// // 				cout << "default Y coordinate display position.:" << lpYDefault << endl;
// // 				cout << "minimum X display size:" << lpMinX << endl;
// // 				cout << "minimum Y display size:" << lpMinY << endl;
// // 				cout << "maximum X display size:" << lpMaxX << endl;
// // 				cout << "maximum Y display size:" << lpMaxY << endl;
// // 				cout << "step size along the X axis:" << lpStepX << endl;
// // 				cout << "step size along the Y axis:" << lpStepY << endl;
// 				Amdisplay.DisplayPosition.X = lpX;
// 				Amdisplay.DisplayPosition.Y = lpY;
// 				Amdisplay.DisplayPosition.XDefault = lpXDefault;
// 				Amdisplay.DisplayPosition.YDefault = lpYDefault;
// 				Amdisplay.DisplayPosition.MaxX = lpMaxX;
// 				Amdisplay.DisplayPosition.MaxY = lpMaxY;
// 				Amdisplay.DisplayPosition.MinX = lpMinX;
// 				Amdisplay.DisplayPosition.MinY = lpMinY;
// 				Amdisplay.DisplayPosition.StepX = lpStepX;
// 				Amdisplay.DisplayPosition.StepY = lpStepY;
// 			}
// 		}
// 
// 		if (ADL_Display_Size_Get)
// 		{
// 			int lpWidth = 0, lpHeight = 0, lpDefaultWidth = 0, lpDefaultHeight = 0, lpMinWidth = 0, lpMinHeight = 0, lpMaxWidth = 0, lpMaxHeight = 0, lpStepWidth = 0, lpStepHeight = 0;
// 			if (ADL_OK == ADL_Display_Size_Get(AdapterNum, j, &lpWidth, &lpHeight, &lpDefaultWidth, &lpDefaultHeight, &lpMinWidth, &lpMinHeight, &lpMaxWidth, &lpMaxHeight, &lpStepWidth, &lpStepHeight))
// 			{
// // 				cout << "Device Display Size" << endl;
// // 				cout << "The pointer to the current display width:" << lpWidth << endl;
// // 				cout << "The pointer to the current display height:" << lpHeight << endl;
// // 				cout << "The pointer to the default display width:" << lpDefaultWidth << endl;
// // 				cout << " The pointer to the default display height:" << lpDefaultHeight << endl;
// // 				cout << "The pointer to the minimum display width:" << lpMinWidth << endl;
// // 				cout << "The pointer to the minimum display height:" << lpMinHeight << endl;
// // 				cout << "The pointer to the maximum display height:" << lpMaxWidth << endl;
// // 				cout << "The pointer to the maximum display height:" << lpMaxHeight << endl;
// // 				cout << "The pointer to the step width:" << lpStepWidth << endl;
// // 				cout << "The pointer to the step height:" << lpStepHeight << endl;
// 				Amdisplay.DisplaySize.Width = lpWidth;
// 				Amdisplay.DisplaySize.Height = lpHeight;
// 				Amdisplay.DisplaySize.DefaultWidth = lpDefaultWidth;
// 				Amdisplay.DisplaySize.DefaultHeight = lpDefaultHeight;
// 				Amdisplay.DisplaySize.MaxWidth = lpMaxWidth;
// 				Amdisplay.DisplaySize.MaxHeight = lpMaxHeight;
// 				Amdisplay.DisplaySize.MinWidth = lpMinWidth;
// 				Amdisplay.DisplaySize.MinHeight = lpMinHeight;
// 				Amdisplay.DisplaySize.StepWidth = lpStepWidth;
// 				Amdisplay.DisplaySize.StepHeight = lpStepHeight;
// 			}
// 		}
// 
// 		if (ADL_Display_DeviceConfig_Get)
// 		{
// 			ADLDisplayConfig lpDisplayConfig;
// 			if (ADL_OK == ADL_Display_DeviceConfig_Get(AdapterNum, j, &lpDisplayConfig))
// 			{
// //				cout << "get HDTV capability settings." << endl;
// 				Amdisplay.DisplayConfig = lpDisplayConfig;
// 			}
// 		}
// 
// 		if (ADL_Display_Property_Get)
// 		{
// 			ADLDisplayProperty lpDisplayProperty;
// 			if (ADL_OK == ADL_Display_Property_Get(AdapterNum, j, &lpDisplayProperty))
// 			{
// //				cout << "current display property value" << endl;
// 				Amdisplay.DisplayProperty = lpDisplayProperty;
// 			}
// 		}
// 
// 		if (ADL_Display_DitherState_Get)
// 		{
// 			int lpDitherState = 0;
// 			if (ADL_OK == ADL_Display_DitherState_Get(AdapterNum, j, &lpDitherState))
// 			{
// //				cout << "retrieve the dither state." << endl;
// 				Amdisplay.DitherState = lpDitherState;
// 			}
// 		}
// 
// 		if (ADL_Display_SupportedPixelFormat_Get)
// 		{
// 			int lpPixelFormat = 0;
// 			if (ADL_OK == ADL_Display_SupportedPixelFormat_Get(AdapterNum, j, &lpPixelFormat))
// 			{
// //				cout << "retrieve the supported pixel format. HDMI only." << endl;
// 				Amdisplay.SupportedPixelFormat = lpPixelFormat;
// 			}
// 		}
// 
// 		if (ADL_Display_PixelFormat_Get)
// 		{
// 			int lpPixelFormat = 0;
// 			if (ADL_OK == ADL_Display_PixelFormat_Get(AdapterNum, j, &lpPixelFormat))
// 			{
// //				cout << "retrieve the current pixel format. HDMI only." << endl;
// 				Amdisplay.CurrentedPixelFormat = lpPixelFormat;
// 			}
// 		}
// 
// 		if (ADL_Display_SupportedColorDepth_Get)
// 		{
// 			int lpColorDepth = 0;
// 			if (ADL_OK == ADL_Display_SupportedColorDepth_Get(AdapterNum, j, &lpColorDepth))
// 			{
// //				cout << " supported color depth" << endl;
// 				Amdisplay.SupportColorDepth = lpColorDepth;
// 			}
// 		}
// 
// 		if (ADL_Display_ColorDepth_Get)
// 		{
// 			int lpColorDepth = 0;
// 			if (ADL_OK == ADL_Display_ColorDepth_Get(AdapterNum, j, &lpColorDepth))
// 			{
// //				cout << "Current color depth" << endl;
// 				Amdisplay.CurrentColorDepth = lpColorDepth;
// 			}
// 		}
// 
// 		if (ADL_Display_AdjustmentCoherent_Get)
// 		{
// 			int lpAdjustmentCoherentCurrent = 0, lpAdjustmentCoherentDefault = 0;
// 			if (ADL_OK == ADL_Display_AdjustmentCoherent_Get(AdapterNum, j, &lpAdjustmentCoherentCurrent, &lpAdjustmentCoherentDefault))
// 			{
// //				cout << "adjustment coherent setting" << endl;
// 				Amdisplay.AdjustmentCoherentCurrent = lpAdjustmentCoherentCurrent;
// 				Amdisplay.AdjustmentCoherentDefault = lpAdjustmentCoherentDefault;
// 			}
// 		}
// 
// 		if (ADL_Display_ReducedBlanking_Get)
// 		{
// 			int lpReducedBlankingCurrent = 0, lpReducedBlankingDefault = 0;
// 			if (ADL_OK == ADL_Display_ReducedBlanking_Get(AdapterNum, j, &lpReducedBlankingCurrent, &lpReducedBlankingDefault))
// 			{
// //				cout << "Current color depth" << endl;
// 				Amdisplay.ReducedBlankingCurrent = lpReducedBlankingCurrent;
// 				Amdisplay.ReducedBlankingDefault = lpReducedBlankingDefault;
// 			}
// 		}
// 
// 		if (ADL_Display_FormatsOverride_Get)
// 		{
// 			int lpSettingsSupported = 0, lpSettingsSupportedEx = 0, lpCurSettings = 0;
// 			if (ADL_OK == ADL_Display_FormatsOverride_Get(AdapterNum, j, &lpSettingsSupported, &lpSettingsSupportedEx, &lpCurSettings))
// 			{
// //				cout << "adjustment coherent setting" << endl;
// 				Amdisplay.OverrideSettingsSupported = lpSettingsSupported;
// 				Amdisplay.OverrideSettingsSupportedEx = lpSettingsSupportedEx;
// 				Amdisplay.OverrideCurSettings = lpCurSettings;
// 			}
// 		}
// 
// 		info.Amdisplay.emplace_back(Amdisplay);
// 	}
// }
// 
// void CAMD::GetDisplayModes(const AdapterInfo adapterInfo, const int iNumberAdapters, AMDINFO& info)
// {
// 	int ADL_ERROR_Code = ADL_ERR;
// 	LPADLMode lpModes = NULL;
// 	const unsigned int g_uiVirtualDisplayWidth = 1028;
// 	const unsigned int g_uiVirtualDisplayHeight = 1028;
// 	const unsigned int g_uiRefreshRate = 60;
// 	if (ADL_Display_Modes_Get)
// 	{
// 		int lpNumModes = 0;
// 		if (ADL_OK == ADL_Display_Modes_Get(-1, -1, &lpNumModes, &lpModes))
// 		{
// 			cout << "Display Mode Info" << endl;
// 		}
// 
// 		for (int i = 0; i < lpNumModes; i++)
// 		{
// 			int iOverrideSupported = 0;
// 			if (ADL_OK != ADL_Adapter_ModeTimingOverride_Caps(adapterInfo.iAdapterIndex, &iOverrideSupported))
// 			{
// 				break;
// 			}
// 			if (iOverrideSupported)
// 			{
// 				ADLDisplayModeX2 lpModeIn = {};
// 				ADLDisplayModeInfo lpModeInfoOut = {};
// 				lpModeIn.iWidth = g_uiVirtualDisplayWidth;
// 				lpModeIn.iHeight = g_uiVirtualDisplayHeight;
// 				lpModeIn.iRefreshRate = g_uiRefreshRate;
// 				lpModeIn.iScanType = 0; // 0 = progressive / ADL_DL_TIMINGFLAG_INTERLACED = interlaced
// 				lpModeIn.iTimingStandard = ADL_DL_MODETIMING_STANDARD_CVT;
// 				if (lpModes[i].displayID.iDisplayLogicalAdapterIndex < iNumberAdapters)
// 				{
// 					if (ADL_Display_ModeTimingOverrideX2_Get)
// 					{
// 
// 
// 						ADL_ERROR_Code = ADL_Display_ModeTimingOverrideX2_Get(i, lpModes[i].displayID, &lpModeIn, &lpModeInfoOut);
// 						if (ADL_OK == ADL_ERROR_Code)
// 						{
// 							cout << "Ge ADL_Display_ModeTimingOverrideX2_Gett " << endl;
// 						}
// 					}
// 				}
// 			}
// 			break;
// 		}
// 		ADL_Main_Memory_Free((void**)&lpModes);
// 	}
// }