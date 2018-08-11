#pragma once
typedef std::map<std::string, std::string> MapString;

struct SMBIOS_Struct
{
	MapString BIOSinfo;	//Type0
	MapString Sysyteminfo;//Type1
	MapString BaseBroadinfo;//Type2
	MapString SystemEorC;//Type3
	MapString Processorinfo;//Type4
	MapString MemoryCtrlinfo;//Type5
	MapString MemoryModinfo;//Type6
	std::vector<MapString > Cacheinfo;//Type7
	std::vector<MapString > Portinfo;//Type8
	std::vector<MapString > SystemSlotinfo;//Type9
	MapString BroadDevinfo;//Type10
	MapString OEMString;//Type11
	MapString SystemConfigOption;//Type12
	MapString BIOSLanginfo;//Type13
	MapString GroupAssociations;//Type14
	MapString SysEventLog;//Type15
	std::vector<MapString > PhysicalMemoryArray;//Type16
	std::vector<MapString > MemoryDev;//Type17
	std::vector<MapString > MemoryErrinfo;//Type18
	std::vector<MapString > MemArrayMappedAddress;//Type19
	std::vector<MapString > MemDevMappedAddress;//Type20
	MapString Pointing_Dev;//Type21
	MapString Portable_Battery;//Type22
	MapString System_Reset;//Type23
	MapString Hardware_Security;//Type24
	MapString System_Power_Controls;//Type25
	std::vector<MapString > Voltage_Probe;//Type26
	std::vector<MapString > CoolingDev;//Type27
	std::vector<MapString > TemperatureProbe;//Type28
	std::vector<MapString > ElectricalCurrentProbe;//Type29
	std::vector<MapString > BandRemoteAccess;//Type30
	std::vector<MapString > Systembootstatus;//Type32
	std::vector<MapString > MemoryError64Bit;//Type33
	std::vector<MapString > ManagementDevice;//Type34
	std::vector<MapString > ManagementDeviceComponent;//Type35
	std::vector<MapString > ManagementDeviceComponentThresholdData;//Type36
	std::vector<MapString > MemoryChannel;//Type37
	std::vector<MapString > IPMIDeviceinfo;//Type38
	std::vector<MapString > SystemPowerSupply;//Type39
	std::vector<MapString > Additionalinfo;//Type40
	std::vector<MapString > BroadDevExtendedinfo;//Type41
	std::vector<MapString > ManagementControllerHostInterface;//Type42
	std::vector<MapString > TPMDevice;//Type43
	std::vector<MapString > Inactive;//Type126
	std::vector<MapString > EndofTable;//Type127
};