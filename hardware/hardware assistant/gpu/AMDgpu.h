#pragma once
#include "Gpudata.h"


#define	AMD_VendorID						1002
#define ADL_WARNING_NO_DATA					-100

typedef int(*ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int);
typedef int(*ADL_MAIN_CONTROL_DESTROY)(void);
typedef	int(*ADL_GRAPHICS_VERSIONS_GET)(ADLVersionsInfo*);
typedef int(*ADL_GRAPHICS_PLATFORM_GET)(int*);
typedef int(*ADL_ADAPTER_ACTIVE_GET)(int, int*);
typedef int(*ADL_ADAPTER_NUMBEROFADAPTERS_GET)(int*);
typedef int(*ADL_ADAPTER_ADAPTERINFO_GET)(LPAdapterInfo, int);
typedef int(*ADL_ADAPTER_ASPECTS_GET)(int, char*, int);
typedef int(*ADL_ADAPTER_ASICFAMILYTYPE_GET)(int, int*, int*);
typedef int(*ADL_ADAPTER_SPEED_CAPS)(int , int*, int*);
typedef	int(*ADL_ADAPTER_SPEED_GET)(int, int*, int*);
typedef int(*ADL_ADAPTER_ACCESSIBILITY_GET)(int, int*);
typedef int(*ADL_ADAPTER_VIDEOBIOSINFO_GET)(int, ADLBiosInfo*);
typedef int(*ADL_ADAPTER_ID_GET)(int, int *);
typedef int(*ADL_ADAPTERX2_CAPS)(int, ADLAdapterCapsX2*);
typedef int(*ADL_DISPLAY_DISPLAYINFO_GET)(int, int*, ADLDisplayInfo **, int);
typedef int(*ADL_DISPLAY_DPMSTINFO_GET)(int, int*, ADLDisplayDPMSTInfo **, int);
typedef int(*ADL_DISPLAY_PRESERVEDASPECTRATIO_GET)(int, int, int*, int*, int*);
typedef int(*ADL_DISPLAY_IMAGEEXPANSION_GET)(int, int, int*, int*, int*);
typedef int(*ADL_DISPLAY_POSITION_GET)(int, int, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*);
typedef int(*ADL_DISPLAY_SIZE_GET)(int, int, int*, int*, int*, int*, int*, int*, int*, int*, int*, int*);
typedef int(*ADL_DISPLAY_ADJUSTCAPS_GET)(int, int, int*);
typedef int(*ADL_DISPLAY_CAPABILITIES_GET)(int, int*, int*);
typedef int(*ADL_DISPLAY_CONNECTEDDISPLAYS_GET)(int, int*);
typedef int(*ADL_DISPLAY_DEVICECONFIG_GET)(int, int, ADLDisplayConfig *);
typedef int(*ADL_DISPLAY_PROPERTY_GET)(int, int, ADLDisplayProperty*);
typedef int(*ADL_DISPLAY_SWITCHINGCAPABILITY_GET)(int, int*);
typedef int(*ADL_DISPLAY_DITHERSTATE_GET)(int, int, int*);
typedef int(*ADL_DISPLAY_SUPPORTEDPIXELFORMAT_GET)(int, int, int*);
typedef int(*ADL_DISPLAY_PIXELFORMAT_GET)(int, int, int*);
typedef int(*ADL_DISPLAY_SUPPORTEDCOLORDEPTH_GET)(int, int, int*);
typedef int(*ADL_DISPLAY_COLORDEPTH_GET)(int, int, int*);
typedef int(*ADL_DISPLAY_ODCLOCKINFO_GET)(int, ADLAdapterODClockInfo *);
typedef int(*ADL_DISPLAY_ADJUSTMENTCOHERENT_GET)(int, int, int*, int*);
typedef int(*ADL_DISPLAY_REDUCEDBLANKING_GET)(int, int, int*, int*);
typedef int(*ADL_DISPLAY_FORMATSOVERRIDE_GET)(int, int, int*, int*, int*);
typedef int(*ADL_DISPLAY_MVPUCAPS_GET)(int, ADLMVPUCaps *);
typedef int(*ADL_DISPLAY_MVPUSTATUS_GET)(int, ADLMVPUStatus *);
typedef int(*ADL_DISPLAY_MODES_GET)(int, int, int*,	ADLMode **);
typedef int(*ADL_DISPLAY_DISPLAYMAPCONFIG_GET)(int, int*, ADLDisplayMap **, int*, ADLDisplayTarget **, int);
typedef int(*ADL_DISPLAY_MODETIMINGOVERRIDEX2_GET) (int, ADLDisplayID, ADLDisplayModeX2*, ADLDisplayModeInfo*);
typedef int(*ADL_ADAPTER_MODETIMINGOVERRIDE_CAPS) (int, int*);
typedef int(*ADL_OVERDRIVE_CAPS) (int iAdapterIndex, int *iSupported, int *iEnabled, int *iVersion);
typedef int(*ADL_OVERDRIVE5_THERMALDEVICES_ENUM) (int iAdapterIndex, int iThermalControllerIndex, ADLThermalControllerInfo *lpThermalControllerInfo);
typedef int(*ADL_OVERDRIVE5_ODPARAMETERS_GET) (int  iAdapterIndex, ADLODParameters *  lpOdParameters);
typedef int(*ADL_OVERDRIVE5_TEMPERATURE_GET) (int iAdapterIndex, int iThermalControllerIndex, ADLTemperature *lpTemperature);
typedef int(*ADL_OVERDRIVE5_FANSPEED_GET) (int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue *lpFanSpeedValue);
typedef int(*ADL_OVERDRIVE5_FANSPEEDINFO_GET) (int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedInfo *lpFanSpeedInfo);
typedef int(*ADL_OVERDRIVE5_ODPERFORMANCELEVELS_GET) (int iAdapterIndex, int iDefault, ADLODPerformanceLevels *lpOdPerformanceLevels);
typedef int(*ADL_OVERDRIVE5_ODPARAMETERS_GET) (int iAdapterIndex, ADLODParameters *lpOdParameters);
typedef int(*ADL_OVERDRIVE5_CURRENTACTIVITY_GET) (int iAdapterIndex, ADLPMActivity *lpActivity);
typedef int(*ADL_OVERDRIVE5_FANSPEED_SET)(int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue *lpFanSpeedValue);
typedef int(*ADL_OVERDRIVE5_ODPERFORMANCELEVELS_SET) (int iAdapterIndex, ADLODPerformanceLevels *lpOdPerformanceLevels);
typedef int(*ADL_OVERDRIVE5_POWERCONTROL_CAPS)(int iAdapterIndex, int *lpSupported);
typedef int(*ADL_OVERDRIVE5_POWERCONTROLINFO_GET)(int iAdapterIndex, ADLPowerControlInfo *lpPowerControlInfo);
typedef int(*ADL_OVERDRIVE5_POWERCONTROL_GET)(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int(*ADL_OVERDRIVE5_POWERCONTROL_SET)(int iAdapterIndex, int iValue);
typedef int(*ADL_OVERDRIVE6_FANSPEED_GET)(int iAdapterIndex, ADLOD6FanSpeedInfo *lpFanSpeedInfo);
typedef int(*ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS)(int iAdapterIndex, ADLOD6ThermalControllerCaps *lpThermalControllerCaps);
typedef int(*ADL_OVERDRIVE6_TEMPERATURE_GET)(int iAdapterIndex, int *lpTemperature);
typedef int(*ADL_OVERDRIVE6_CAPABILITIES_GET) (int iAdapterIndex, ADLOD6Capabilities *lpODCapabilities);
typedef int(*ADL_OVERDRIVE6_STATEINFO_GET)(int iAdapterIndex, int iStateType, ADLOD6StateInfo *lpStateInfo);
typedef int(*ADL_OVERDRIVE6_CURRENTSTATUS_GET)(int iAdapterIndex, ADLOD6CurrentStatus *lpCurrentStatus);
typedef int(*ADL_OVERDRIVE6_POWERCONTROL_CAPS) (int iAdapterIndex, int *lpSupported);
typedef int(*ADL_OVERDRIVE6_POWERCONTROLINFO_GET)(int iAdapterIndex, ADLOD6PowerControlInfo *lpPowerControlInfo);
typedef int(*ADL_OVERDRIVE6_POWERCONTROL_GET)(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int(*ADL_OVERDRIVE6_FANSPEED_SET)(int iAdapterIndex, ADLOD6FanSpeedValue *lpFanSpeedValue);
typedef int(*ADL_OVERDRIVE6_STATE_SET)(int iAdapterIndex, int iStateType, ADLOD6StateInfo *lpStateInfo);
typedef int(*ADL_OVERDRIVE6_POWERCONTROL_SET)(int iAdapterIndex, int iValue);


using namespace std;

class CAMD : public CGPU
{
public:
	CAMD();
	~CAMD();
	CGPU::GPUTypes exec();
	const void* Returninfo();
	CGPU::GPUTypes UpdateData();
protected:
	BOOL InitializeADL();
// 	void GetDisplay(int AdapterNum, AMDINFO& info);
// 	void GetDisplayModes(const AdapterInfo adapterInfo, const int iNumberAdapters, AMDINFO& info);
	void DestoryADL();
	void GetBaseInfo();
	void GetAdapterinfo();
	BOOL GetOverDrive5(int adapterId, AMDINFO& info);
private:
	static void * __stdcall ADL_Main_Memory_Alloc(int iSize);
	static void __stdcall ADL_Main_Memory_Free(void ** lpBuffer);
private:
	ADLVersionsInfo							Versioninfo;
	AMDINFO									AmdInfo;
	ADL_MAIN_CONTROL_CREATE					ADL_Main_Control_Create;
	ADL_MAIN_CONTROL_DESTROY				ADL_Main_Control_Destroy;
	ADL_GRAPHICS_VERSIONS_GET				ADL_Graphics_Versions_Get;
	ADL_GRAPHICS_PLATFORM_GET				ADL_Graphics_Platform_Get;
	ADL_ADAPTER_NUMBEROFADAPTERS_GET		ADL_Adapter_NumberOfAdapters_Get;
	ADL_ADAPTER_ACTIVE_GET					ADL_Adapter_Active_Get;
	ADL_ADAPTER_ADAPTERINFO_GET				ADL_Adapter_AdapterInfo_Get;
	ADL_ADAPTER_ASICFAMILYTYPE_GET			ADL_Adapter_ASICFamilyType_Get;
	ADL_ADAPTER_ASPECTS_GET					ADL_Adapter_Aspects_Get;
	ADL_ADAPTER_SPEED_CAPS					ADL_Adapter_Speed_Caps;
	ADL_ADAPTER_SPEED_GET					ADL_Adapter_Speed_Get;
	ADL_ADAPTER_VIDEOBIOSINFO_GET			ADL_Adapter_VideoBiosInfo_Get;
	ADL_ADAPTER_ACCESSIBILITY_GET			ADL_Adapter_Accessibility_Get;
	ADL_ADAPTER_ID_GET						ADL_Adapter_ID_Get;
	ADL_ADAPTERX2_CAPS						ADL_AdapterX2_Caps;
	ADL_DISPLAY_DISPLAYINFO_GET				ADL_Display_DisplayInfo_Get;
	ADL_DISPLAY_DPMSTINFO_GET				ADL_Display_DpMstInfo_Get;
	ADL_DISPLAY_PRESERVEDASPECTRATIO_GET	ADL_Display_PreservedAspectRatio_Get;
	ADL_DISPLAY_IMAGEEXPANSION_GET			ADL_Display_ImageExpansion_Get;
	ADL_DISPLAY_POSITION_GET				ADL_Display_Position_Get;
	ADL_DISPLAY_SIZE_GET					ADL_Display_Size_Get;
	ADL_DISPLAY_ADJUSTCAPS_GET				ADL_Display_AdjustCaps_Get;
	ADL_DISPLAY_CAPABILITIES_GET			ADL_Display_Capabilities_Get;
	ADL_DISPLAY_CONNECTEDDISPLAYS_GET		ADL_Display_ConnectedDisplays_Get;
	ADL_DISPLAY_DEVICECONFIG_GET			ADL_Display_DeviceConfig_Get;
	ADL_DISPLAY_PROPERTY_GET				ADL_Display_Property_Get;
	ADL_DISPLAY_SWITCHINGCAPABILITY_GET		ADL_Display_SwitchingCapability_Get;
	ADL_DISPLAY_DITHERSTATE_GET				ADL_Display_DitherState_Get;
	ADL_DISPLAY_SUPPORTEDPIXELFORMAT_GET	ADL_Display_SupportedPixelFormat_Get;
	ADL_DISPLAY_PIXELFORMAT_GET				ADL_Display_PixelFormat_Get;
	ADL_DISPLAY_SUPPORTEDCOLORDEPTH_GET		ADL_Display_SupportedColorDepth_Get;
	ADL_DISPLAY_COLORDEPTH_GET				ADL_Display_ColorDepth_Get;
	ADL_DISPLAY_ODCLOCKINFO_GET				ADL_Display_ODClockInfo_Get;
	ADL_DISPLAY_ADJUSTMENTCOHERENT_GET		ADL_Display_AdjustmentCoherent_Get;
	ADL_DISPLAY_FORMATSOVERRIDE_GET			ADL_Display_FormatsOverride_Get;
	ADL_DISPLAY_REDUCEDBLANKING_GET			ADL_Display_ReducedBlanking_Get;
	ADL_DISPLAY_MVPUCAPS_GET				ADL_Display_MVPUCaps_Get;
	ADL_DISPLAY_MVPUSTATUS_GET				ADL_Display_MVPUStatus_Get;
	ADL_DISPLAY_MODES_GET					ADL_Display_Modes_Get;
	ADL_DISPLAY_DISPLAYMAPCONFIG_GET		ADL_Display_DisplayMapConfig_Get;
	ADL_DISPLAY_MODETIMINGOVERRIDEX2_GET	ADL_Display_ModeTimingOverrideX2_Get;
	ADL_ADAPTER_MODETIMINGOVERRIDE_CAPS     ADL_Adapter_ModeTimingOverride_Caps;

	ADL_OVERDRIVE5_THERMALDEVICES_ENUM		ADL_Overdrive5_ThermalDevices_Enum;
	ADL_OVERDRIVE5_ODPARAMETERS_GET			ADL_Overdrive5_ODParameters_Get;
	ADL_OVERDRIVE5_TEMPERATURE_GET			ADL_Overdrive5_Temperature_Get;
	ADL_OVERDRIVE5_FANSPEED_GET				ADL_Overdrive5_FanSpeed_Get;
	ADL_OVERDRIVE5_FANSPEEDINFO_GET			ADL_Overdrive5_FanSpeedInfo_Get;
	ADL_OVERDRIVE5_ODPERFORMANCELEVELS_GET	ADL_Overdrive5_ODPerformanceLevels_Get;
	ADL_OVERDRIVE5_CURRENTACTIVITY_GET		ADL_Overdrive5_CurrentActivity_Get;
	ADL_OVERDRIVE5_FANSPEED_SET				ADL_Overdrive5_FanSpeed_Set;
	ADL_OVERDRIVE5_ODPERFORMANCELEVELS_SET  ADL_Overdrive5_ODPerformanceLevels_Set;
	ADL_OVERDRIVE5_POWERCONTROL_CAPS		ADL_Overdrive5_PowerControl_Caps;
	ADL_OVERDRIVE5_POWERCONTROLINFO_GET		ADL_Overdrive5_PowerControlInfo_Get;
	ADL_OVERDRIVE5_POWERCONTROL_GET			ADL_Overdrive5_PowerControl_Get;
	ADL_OVERDRIVE5_POWERCONTROL_SET			ADL_Overdrive5_PowerControl_Set;
	ADL_OVERDRIVE6_STATE_SET				ADL_Overdrive6_State_Set;
	HINSTANCE								hDLL;		// Handle to DLL
};

