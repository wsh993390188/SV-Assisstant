#include "stdafx.h"
#include "ADLSDKHelper.h"
#include "GPUDeviceFactory.h"

#pragma region OD
#include "OverDrive5.h"
#include "OverDrive6.h"
#include "OverDriveN.h"
#include "OverDrive8.h"
#pragma endregion
namespace
{
	// Memory allocation function
	void* __stdcall ADL_Main_Memory_Alloc(int iSize)
	{
		void* lpBuffer = malloc(iSize);
		return lpBuffer;
	}

	// Optional Memory de-allocation function
	void __stdcall ADL_Main_Memory_Free(void** lpBuffer)
	{
		if (NULL != *lpBuffer)
		{
			free(*lpBuffer);
			*lpBuffer = NULL;
		}
	}

	template<typename ...Args>
	std::unique_ptr<Hardware::GPU::ADLSDK::Overdrive> CreateOverDrive(const Hardware::GPU::ADLSDK::OverDriveVersion& ODVersion, Args&& ...args)
	{
		switch (ODVersion)
		{
		case Hardware::GPU::ADLSDK::OverDriveVersion::OverDrive5:
			return std::make_unique<Hardware::GPU::ADLSDK::OverDrive5>(std::forward<Args>(args)...);
			break;
		case Hardware::GPU::ADLSDK::OverDriveVersion::OverDrive6:
			return std::make_unique<Hardware::GPU::ADLSDK::OverDrive6>(std::forward<Args>(args)...);
			break;
		case Hardware::GPU::ADLSDK::OverDriveVersion::OverDriveN:
			return std::make_unique<Hardware::GPU::ADLSDK::OverDriveN>(std::forward<Args>(args)...);
			break;
		case Hardware::GPU::ADLSDK::OverDriveVersion::OverDrive8:
			return std::make_unique<Hardware::GPU::ADLSDK::OverDrive8>(std::forward<Args>(args)...);
			break;
		}
		return nullptr;
	}
	/// @brief ADLSDK专用的VendorID
	constexpr auto AMDVendorIDADLOnly = 1002;
}

Hardware::GPU::ADLSDKHelper& Hardware::GPU::ADLSDKHelper::Instance()
{
	static ADLSDKHelper inst;
	return inst;
}

HMODULE Hardware::GPU::ADLSDKHelper::GetADLModule() const
{
	return hModule;
}

std::string Hardware::GPU::ADLSDKHelper::GetTemperature(const uint32_t& DeviceId)
{
	if (!this->IsADLEnable)
	{
		throw std::exception("ADL SDK Not Enable");
	}
	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceId && gpu.OD)
		{
			return gpu.OD->GetTemperature(gpu.AdapterIndex);
		}
	}
	throw ADLDeviceNotExist(DeviceId);
}

std::string Hardware::GPU::ADLSDKHelper::GetFanSpeed(const uint32_t& DeviceId)
{
	if (!this->IsADLEnable)
	{
		throw std::exception("ADL SDK Not Enable");
	}
	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceId && gpu.OD)
		{
			return gpu.OD->GetFanSpeed(gpu.AdapterIndex);
		}
	}
	throw ADLDeviceNotExist(DeviceId);
}

std::string Hardware::GPU::ADLSDKHelper::GetGPUMemoryClock(const uint32_t& DeviceId)
{
	if (!this->IsADLEnable)
	{
		throw std::exception("ADL SDK Not Enable");
	}
	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceId && gpu.OD)
		{
			return gpu.OD->GetGPUMemoryClock(gpu.AdapterIndex);
		}
	}
	throw ADLDeviceNotExist(DeviceId);
}

std::string Hardware::GPU::ADLSDKHelper::GetGPUCoreClock(const uint32_t& DeviceId)
{
	if (!this->IsADLEnable)
	{
		throw std::exception("ADL SDK Not Enable");
	}
	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceId && gpu.OD)
		{
			return gpu.OD->GetGPUCoreClock(gpu.AdapterIndex);
		}
	}
	throw ADLDeviceNotExist(DeviceId);
}

std::string Hardware::GPU::ADLSDKHelper::GetGPUVoltage(const uint32_t& DeviceId)
{
	if (!this->IsADLEnable)
	{
		throw std::exception("ADL SDK Not Enable");
	}
	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceId && gpu.OD)
		{
			return gpu.OD->GetGPUVoltage(gpu.AdapterIndex);
		}
	}
	throw ADLDeviceNotExist(DeviceId);
}

std::string Hardware::GPU::ADLSDKHelper::GetGPUName(const uint32_t& DeviceId)
{
	if (!this->IsADLEnable)
	{
		throw std::exception("ADL SDK Not Enable");
	}

	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceId)
		{
			return gpu.FullName;
		}
	}

	throw ADLDeviceNotExist(DeviceId);
}

std::string Hardware::GPU::ADLSDKHelper::GetGPUDriverVersion(const uint32_t&)
{
	if (!this->IsADLEnable)
	{
		throw std::exception("ADL SDK Not Enable");
	}

	static auto ADL_Graphics_Versions_Get = Utils::GetADLFuncAddress<ADL_GRAPHICS_VERSIONS_GET>(hModule, "ADL_Graphics_Versions_Get");

	if (ADL_Graphics_Versions_Get)
	{
		ADLVersionsInfo	Versioninfo{};
		if (auto adl_err = ADL_Graphics_Versions_Get(&Versioninfo); ADL_Success(adl_err))
		{
			return Versioninfo.strDriverVer;
		}
		else
		{
			spdlog::error("ADL_Graphics_Versions_Get Error: {0}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Graphics_Versions_Get from dll");
	}

	throw std::exception("ADL Not Support get driver version");
}

std::string Hardware::GPU::ADLSDKHelper::GetGPUBIOSVersion(const uint32_t& DeviceId)
{
	if (!this->IsADLEnable)
	{
		throw std::exception("ADL SDK Not Enable");
	}

	if (ADL_Adapter_VideoBiosInfo_Get)
	{
		for (const auto& gpu : GpuQueryDatas)
		{
			if (gpu.DeviceId == DeviceId)
			{
				ADLBiosInfo	Versioninfo{};
				if (auto adl_err = ADL_Adapter_VideoBiosInfo_Get(gpu.AdapterIndex, &Versioninfo); ADL_Success(adl_err))
				{
					return Versioninfo.strVersion;
				}
				else
				{
					spdlog::error("ADL_Adapter_VideoBiosInfo_Get Error: {0}", GetADLStatus(adl_err).c_str());
				}
			}
		}
		throw ADLDeviceNotExist(DeviceId);
	}
	else
	{
		spdlog::error("cant load ADL_Adapter_VideoBiosInfo_Get from dll");
	}

	throw std::exception("ADL Not Support get vbios version");
}

std::string Hardware::GPU::ADLSDKHelper::GetGPUBIOSDate(const uint32_t& DeviceId)
{
	if (!this->IsADLEnable)
	{
		throw std::exception("ADL SDK Not Enable");
	}

	if (ADL_Adapter_VideoBiosInfo_Get)
	{
		for (const auto& gpu : GpuQueryDatas)
		{
			if (gpu.DeviceId == DeviceId)
			{
				ADLBiosInfo	Versioninfo{};
				if (auto adl_err = ADL_Adapter_VideoBiosInfo_Get(gpu.AdapterIndex, &Versioninfo); ADL_Success(adl_err))
				{
					return Versioninfo.strDate;
				}
				else
				{
					spdlog::error("ADL_Adapter_VideoBiosInfo_Get Error: {0}", GetADLStatus(adl_err).c_str());
				}
			}
		}
		throw ADLDeviceNotExist(DeviceId);
	}
	else
	{
		spdlog::error("cant load ADL_Adapter_VideoBiosInfo_Get from dll");
	}

	throw std::exception("ADL Not Support get vbios date");
}

bool Hardware::GPU::ADLSDKHelper::EnumAdapter()
{
	if (auto ADL_Adapter_NumberOfAdapters_Get = Utils::GetADLFuncAddress<ADL_ADAPTER_NUMBEROFADAPTERS_GET>(hModule, "ADL_Adapter_NumberOfAdapters_Get"); ADL_Adapter_NumberOfAdapters_Get)
	{
		int iAdapterNumbers = 0;
		if (auto adl_err = ADL_Adapter_NumberOfAdapters_Get(&iAdapterNumbers); ADL_Success(adl_err))
		{
			if (auto ADL_Adapter_AdapterInfo_Get = Utils::GetADLFuncAddress<ADL_ADAPTER_ADAPTERINFO_GET>(hModule, "ADL_Adapter_AdapterInfo_Get"); ADL_Adapter_AdapterInfo_Get && iAdapterNumbers > 0)
			{
				auto AdapterInfos = std::make_unique<AdapterInfo[]>(iAdapterNumbers);
				if (auto adl_err = ADL_Adapter_AdapterInfo_Get(AdapterInfos.get(), iAdapterNumbers * sizeof(AdapterInfo)); ADL_Success(adl_err))
				{
					for (decltype(iAdapterNumbers) i = 0; i < iAdapterNumbers; ++i)
					{
						if (AdapterInfos[i].iVendorID == AMDVendorIDADLOnly || AdapterInfos[i].iVendorID == AMDVendorId)
						{
							spdlog::info("ADL Adapter Index:{}", AdapterInfos[i].iAdapterIndex);
							spdlog::info("ADL Adapter UDID: {}", AdapterInfos[i].strUDID);
							spdlog::info("ADL Adapter Bus:{:x} Dev:{:x} Func:{:x}", AdapterInfos[i].iBusNumber, AdapterInfos[i].iDeviceNumber, AdapterInfos[i].iFunctionNumber);
							spdlog::info("ADL Adapter Name: {}", AdapterInfos[i].strAdapterName);
							spdlog::info("ADL Adapter DisplayName: {}", AdapterInfos[i].strDisplayName);
							spdlog::info("ADL Adapter DriverPath: {}", AdapterInfos[i].strDriverPath);
							spdlog::info("ADL Adapter DriverPathExt: {}", AdapterInfos[i].strDriverPathExt);
							spdlog::info("ADL Adapter PNPString: {}", AdapterInfos[i].strPNPString);
							spdlog::info("ADL Adapter Present: {} Exist: {}", AdapterInfos[i].iPresent, AdapterInfos[i].iExist);
							if (AdapterInfos[i].iExist == 1 &&
								!FindDeviceOnQueryDatas(AdapterInfos[i].iBusNumber, AdapterInfos[i].iDeviceNumber, AdapterInfos[i].iFunctionNumber))
							{
								ADLQueryData QueryInfo{};
								ULONG   ven, dev, subsys, rev;
								ven = dev = subsys = rev = 0;
								sscanf_s(AdapterInfos[i].strPNPString,
									"PCI\\VEN_%x&DEV_%x&SUBSYS_%x&REV_%x",
									&ven, &dev, &subsys, &rev);
								QueryInfo.FullName = AdapterInfos[i].strAdapterName;
								QueryInfo.UDID = AdapterInfos[i].strUDID;
								QueryInfo.DeviceId = dev;
								QueryInfo.AdapterIndex = AdapterInfos[i].iAdapterIndex;
								QueryInfo.BusNumber = AdapterInfos[i].iBusNumber;
								QueryInfo.DeviceNumber = AdapterInfos[i].iDeviceNumber;
								QueryInfo.FunctionNumber = AdapterInfos[i].iFunctionNumber;
								QueryInfo.OD = CreateOverDrive(CheckOverDriveSupportVersion(AdapterInfos[i].iAdapterIndex), hModule);
								GpuQueryDatas.emplace_back(std::move(QueryInfo));
							}
						}
					}
				}
				else
				{
					spdlog::error("ADL_Adapter_AdapterInfo_Get Error: {}", GetADLStatus(adl_err).c_str());
				}
			}
		}
		else
		{
			spdlog::error("ADL_Adapter_NumberOfAdapters_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	return false;
}

bool Hardware::GPU::ADLSDKHelper::FindDeviceOnQueryDatas(const uint32_t& BusNumber, const uint32_t& DevNumber, const uint32_t& FuncNumber)
{
	for (const auto& QueryData : GpuQueryDatas)
	{
		if (QueryData.BusNumber == BusNumber && QueryData.DeviceNumber == DevNumber && QueryData.FunctionNumber == FuncNumber)
			return true;
	}
	return false;
}

Hardware::GPU::ADLSDK::OverDriveVersion Hardware::GPU::ADLSDKHelper::CheckOverDriveSupportVersion(const int& AdapterIndex)
{
	int iSupported{}, iEnabled{}, iVersion{};
	ADLSDK::OverDriveVersion retVersion = ADLSDK::OverDriveVersion::NotSupport;
	if (auto ADL2_Overdrive_Caps = Utils::GetADLFuncAddress<ADL2_OVERDRIVE_CAPS>(hModule, "ADL2_Overdrive_Caps"); ADL2_Overdrive_Caps)
	{
		if (auto adl_err = ADL2_Overdrive_Caps(context, AdapterIndex, &iSupported, &iEnabled, &iVersion); ADL_Success(adl_err))
		{
			spdlog::info("ADL AdapterId:{0} ADL2_Overdrive_Caps get Overdrive Support:{1}, Enable:{2}, Version:{3}", AdapterIndex, iSupported, iEnabled, iVersion);
			if (iSupported)
			{
				if (iVersion == 7)
				{
					retVersion = ADLSDK::OverDriveVersion::OverDriveN;
				}
				else if (iVersion == 8)
				{
					retVersion = ADLSDK::OverDriveVersion::OverDrive8;
				}
				else if (iVersion == 6)
				{
					retVersion = ADLSDK::OverDriveVersion::OverDrive6;
				}
				else if (iVersion == 5)
				{
					retVersion = ADLSDK::OverDriveVersion::OverDrive5;
				}
				else
				{
					spdlog::error("ADL SDK Unknown OD Version: {}", iVersion);
				}
			}
			else
			{
				spdlog::error("ADL2_Overdrive_Caps Not support OD");
			}
		}
		else
		{
			spdlog::error("ADL2_Overdrive_Caps Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::warn("cant load ADL2_Overdrive_Caps from dll");
	}

	if (retVersion == ADLSDK::OverDriveVersion::NotSupport)
	{
		if (auto ADL_Overdrive_Caps = Utils::GetADLFuncAddress<ADL_OVERDRIVE_CAPS>(hModule, "ADL_Overdrive_Caps"); ADL_Overdrive_Caps)
		{
			if (auto adl_err = ADL_Overdrive_Caps(AdapterIndex, &iSupported, &iEnabled, &iVersion); ADL_Success(adl_err))
			{
				spdlog::info("ADL AdapterId:{0} ADL_Overdrive_Caps get Overdrive Support:{1}, Enable:{2}, Version:{3}", AdapterIndex, iSupported, iEnabled, iVersion);
				if (iSupported)
				{
					if (iVersion == 6)
					{
						retVersion = ADLSDK::OverDriveVersion::OverDrive6;
					}
					else if (iVersion == 5)
					{
						retVersion = ADLSDK::OverDriveVersion::OverDrive5;
					}
					else
					{
						spdlog::error("ADL SDK Unknown OD Version: {}", iVersion);
					}
				}
				else
				{
					spdlog::error("ADL_Overdrive_Caps Not support OD");
				}
			}
			else
			{
				spdlog::error("ADL_Overdrive_Caps Error: {}", GetADLStatus(adl_err).c_str());
			}
		}
		else
		{
			spdlog::warn("cant load ADL_Overdrive_Caps from dll");
		}
	}
	return retVersion;
}

Hardware::GPU::ADLSDKHelper::ADLSDKHelper() :hModule(nullptr), IsADLEnable(false), context(nullptr), ADL_Adapter_VideoBiosInfo_Get(nullptr)
{
	hModule = LoadLibraryEx(_T("atiadlxx.dll"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (hModule == NULL)
	{
		spdlog::warn("A 32 bit calling application on 64 bit OS will fail to LoadLibrary \nTry to load the 32 bit library (atiadlxy.dll) instead");
		hModule = LoadLibraryEx(_T("atiadlxy.dll"), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	}
	ADL_Adapter_VideoBiosInfo_Get = Utils::GetADLFuncAddress<decltype(ADL_Adapter_VideoBiosInfo_Get)>(hModule, "ADL_Adapter_VideoBiosInfo_Get");
	if (auto ADL_Main_Control_Create = Utils::GetADLFuncAddress<ADL_MAIN_CONTROL_CREATE>(hModule, "ADL_Main_Control_Create"); ADL_Main_Control_Create)
	{
		if (auto adl_err = ADL_Main_Control_Create(ADL_Main_Memory_Alloc, 1); ADL_Success(adl_err))
		{
			spdlog::info("ADL_Main_Control_Create Success");
			IsADLEnable = true;
			EnumAdapter();
		}
		else
		{
			spdlog::error("ADL_Main_Control_Create Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Main_Control_Create from dll");
	}
}

Hardware::GPU::ADLSDKHelper::~ADLSDKHelper()
{
	if (auto ADL_Main_Control_Destroy = Utils::GetADLFuncAddress<ADL_MAIN_CONTROL_DESTROY>(hModule, "ADL_Main_Control_Destroy"); ADL_Main_Control_Destroy)
	{
		ADL_Main_Control_Destroy();
	}
	if (hModule)
		FreeLibrary(hModule);
}

char const* Hardware::GPU::ADLDeviceNotExist::what() const
{
	return Msg.c_str();
}

bool Hardware::GPU::ADL_Success(const int& status)
{
	return status >= ADL_OK;
}

std::string Hardware::GPU::GetADLStatus(const int& status)
{
	switch (status)
	{
	case ADL_OK_WAIT:
		return "All OK, but need to wait";
	case ADL_OK_RESTART:
		return "All OK, but need restart";
	case ADL_OK_MODE_CHANGE:
		return "All OK but need mode change";
	case ADL_OK_WARNING:
		return "All OK, but with warning";
	case ADL_OK:
		return "ADL function completed successfully";
	case ADL_ERR:
		return "Generic Error. Most likely one or more of the Escape calls to the driver failed!";
	case ADL_ERR_NOT_INIT:
		return "ADL not initialized";
	case ADL_ERR_INVALID_PARAM:
		return "One of the parameter passed is invalid";
	case ADL_ERR_INVALID_PARAM_SIZE:
		return "One of the parameter size is invalid";
	case ADL_ERR_INVALID_ADL_IDX:
		return "Invalid ADL index passed";
	case ADL_ERR_INVALID_CONTROLLER_IDX:
		return "Invalid controller index passed";
	case ADL_ERR_INVALID_DIPLAY_IDX:
		return "Invalid display index passed";
	case ADL_ERR_NOT_SUPPORTED:
		return "Function  not supported by the driver";
	case ADL_ERR_NULL_POINTER:
		return "Null Pointer error";
	case ADL_ERR_DISABLED_ADAPTER:
		return "Call can't be made due to disabled adapter";
	case ADL_ERR_INVALID_CALLBACK:
		return "Invalid Callback";
	case ADL_ERR_RESOURCE_CONFLICT:
		return "Display Resource conflict";
	case ADL_ERR_SET_INCOMPLETE:
		return "Failed to update some of the values. Can be returned by set request that include multiple values if not all values were successfully committed.";
	case ADL_ERR_NO_XDISPLAY:
		return "There's no Linux XDisplay in Linux Console environment";
	}
	return {};
}
