#include "stdafx.h"
#include "ADLSDKHelper.h"
#include "OverDrive8.h"

namespace
{
	typedef int(*ADL2_OVERDRIVE8_INIT_SETTING_GET) (ADL_CONTEXT_HANDLE, int, ADLOD8InitSetting*);
	typedef int(*ADL2_OVERDRIVE8_CURRENT_SETTING_GET) (ADL_CONTEXT_HANDLE, int, ADLOD8CurrentSetting*);
	typedef int(*ADL2_OVERDRIVE8_SETTING_SET) (ADL_CONTEXT_HANDLE, int, ADLOD8SetSetting*, ADLOD8CurrentSetting*);
	typedef int(*ADL2_OVERDRIVE8_INIT_SETTINGX2_GET) (ADL_CONTEXT_HANDLE context, int iAdapterIndex, int* lpOverdrive8Capabilities, int* lpNumberOfFeatures, ADLOD8SingleInitSetting** lppInitSettingList);
	typedef int(*ADL2_OVERDRIVE8_CURRENT_SETTINGX2_GET) (ADL_CONTEXT_HANDLE context, int iAdapterIndex, int* lpNumberOfFeatures, int** lppCurrentSettingList);
}

Hardware::GPU::ADLSDK::OverDrive8::OverDrive8(HMODULE hModule) : OverDrive8(hModule, nullptr)
{
}

Hardware::GPU::ADLSDK::OverDrive8::OverDrive8(HMODULE hModule, ADL_CONTEXT_HANDLE context) : context(context)
{
	ADL2_New_QueryPMLogData_Get = Utils::GetADLFuncAddress<ADL2_NEW_QUERYPMLOGDATA_GET>(hModule, "ADL2_New_QueryPMLogData_Get");
}

std::string Hardware::GPU::ADLSDK::OverDrive8::GetTemperature(const uint32_t& AdapterId)
{
	if (ADL2_New_QueryPMLogData_Get)
	{
		//make ADL call for VEGA12
		ADLPMLogDataOutput odlpDataOutput{};
		if (auto adl_err = ADL2_New_QueryPMLogData_Get(context, AdapterId, &odlpDataOutput); ADL_Success(adl_err))
		{
			return std::to_string(odlpDataOutput.sensors[PMLOG_TEMPERATURE_GFX].value / 1000) + " Degree";
		}
		else
		{
			spdlog::error("ADL2_OverdriveN_PerformanceStatus_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL2_OverdriveN_PerformanceStatus_Get from dll");
	}
	throw std::exception("Not support OverdriveN to get Temperature");
}

std::string Hardware::GPU::ADLSDK::OverDrive8::GetFanSpeed(const uint32_t& AdapterId)
{
	if (ADL2_New_QueryPMLogData_Get)
	{
		//make ADL call for VEGA12
		ADLPMLogDataOutput odlpDataOutput{};
		if (auto adl_err = ADL2_New_QueryPMLogData_Get(context, AdapterId, &odlpDataOutput); ADL_Success(adl_err))
		{
			if (odlpDataOutput.sensors[PMLOG_FAN_RPM].value > 0)
				return std::to_string(odlpDataOutput.sensors[PMLOG_FAN_RPM].value) + " RPM";
			else if (odlpDataOutput.sensors[PMLOG_FAN_PERCENTAGE].value > 0)
				return std::to_string(odlpDataOutput.sensors[PMLOG_FAN_PERCENTAGE].value) + " %";
		}
		else
		{
			spdlog::error("ADL2_OverdriveN_PerformanceStatus_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL2_OverdriveN_PerformanceStatus_Get from dll");
	}
	throw std::exception("Not support OverdriveN to get fan speed");
}

std::string Hardware::GPU::ADLSDK::OverDrive8::GetGPUMemoryClock(const uint32_t& AdapterId)
{
	if (ADL2_New_QueryPMLogData_Get)
	{
		//make ADL call for VEGA12
		ADLPMLogDataOutput odlpDataOutput{};
		if (auto adl_err = ADL2_New_QueryPMLogData_Get(context, AdapterId, &odlpDataOutput); ADL_Success(adl_err))
		{
			if (odlpDataOutput.sensors[PMLOG_CLK_MEMCLK].value > 0)
				return Utils::to_string_with_precision(odlpDataOutput.sensors[PMLOG_CLK_MEMCLK].value / 100.0) + " Mhz";
		}
		else
		{
			spdlog::error("ADL2_OverdriveN_PerformanceStatus_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL2_OverdriveN_PerformanceStatus_Get from dll");
	}
	throw std::exception("Not support OverdriveN to get memoryClock");
}

std::string Hardware::GPU::ADLSDK::OverDrive8::GetGPUCoreClock(const uint32_t& AdapterId)
{
	if (ADL2_New_QueryPMLogData_Get)
	{
		//make ADL call for VEGA12
		ADLPMLogDataOutput odlpDataOutput{};
		if (auto adl_err = ADL2_New_QueryPMLogData_Get(context, AdapterId, &odlpDataOutput); ADL_Success(adl_err))
		{
			if (odlpDataOutput.sensors[PMLOG_CLK_MEMCLK].value > 0)
				return Utils::to_string_with_precision(odlpDataOutput.sensors[PMLOG_CLK_GFXCLK].value / 100.0) + " Mhz";
		}
		else
		{
			spdlog::error("ADL2_OverdriveN_PerformanceStatus_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL2_OverdriveN_PerformanceStatus_Get from dll");
	}
	throw std::exception("Not support OverdriveN to get CoreClock");
}

std::string Hardware::GPU::ADLSDK::OverDrive8::GetGPUVoltage(const uint32_t& AdapterId)
{
	if (ADL2_New_QueryPMLogData_Get)
	{
		//make ADL call for VEGA12
		ADLPMLogDataOutput odlpDataOutput{};
		if (auto adl_err = ADL2_New_QueryPMLogData_Get(context, AdapterId, &odlpDataOutput); ADL_Success(adl_err))
		{
			if (odlpDataOutput.sensors[PMLOG_GFX_VOLTAGE].value > 0)
				return  Utils::to_string_with_precision(odlpDataOutput.sensors[PMLOG_GFX_VOLTAGE].value / 1000.0) + " V";
		}
		else
		{
			spdlog::error("ADL2_OverdriveN_PerformanceStatus_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL2_OverdriveN_PerformanceStatus_Get from dll");
	}
	throw std::exception("Not support OverdriveN to get voltage");
}