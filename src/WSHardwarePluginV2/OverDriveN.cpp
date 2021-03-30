#include "stdafx.h"
#include "ADLSDKHelper.h"
#include "OverDriveN.h"

Hardware::GPU::ADLSDK::OverDriveN::OverDriveN(HMODULE hModule) : OverDriveN(hModule, nullptr)
{
}

Hardware::GPU::ADLSDK::OverDriveN::OverDriveN(HMODULE hModule, ADL_CONTEXT_HANDLE context) :
	context(context), ADL2_OverdriveN_PerformanceStatus_Get(nullptr), ADL2_OverdriveN_FanControl_Get(nullptr),
	ADL2_OverdriveN_Temperature_Get(nullptr)
{
	ADL2_OverdriveN_PerformanceStatus_Get = Utils::GetADLFuncAddress<ADL2_OVERDRIVEN_PERFORMANCESTATUS_GET>(hModule, "ADL2_OverdriveN_PerformanceStatus_Get");
	ADL2_OverdriveN_FanControl_Get = Utils::GetADLFuncAddress<ADL2_OVERDRIVEN_FANCONTROL_GET>(hModule, "ADL2_OverdriveN_FanControl_Get");
	ADL2_OverdriveN_Temperature_Get = Utils::GetADLFuncAddress<ADL2_OVERDRIVEN_TEMPERATURE_GET>(hModule, "ADL2_OverdriveN_Temperature_Get");
}

std::string Hardware::GPU::ADLSDK::OverDriveN::GetTemperature(const uint32_t& AdapterId)
{
	if (ADL2_OverdriveN_Temperature_Get)
	{
		int temperature{};
		if (auto adl_err = ADL2_OverdriveN_Temperature_Get(context, AdapterId, 1, &temperature); ADL_Success(adl_err))
		{
			return std::to_string(temperature) + " Degree";
		}
		else
		{
			spdlog::error("ADL2_OverdriveN_Temperature_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Main_Control_Create from dll");
	}
	throw std::exception("Not support OverdriveN to get temperature");
}

std::string Hardware::GPU::ADLSDK::OverDriveN::GetFanSpeed(const uint32_t& AdapterId)
{
	if (ADL2_OverdriveN_FanControl_Get)
	{
		ADLODNFanControl odNFanControl{};

		if (auto adl_err = ADL2_OverdriveN_FanControl_Get(context, AdapterId, &odNFanControl); ADL_Success(adl_err))
		{
			if (odNFanControl.iCurrentFanSpeedMode == ADL_OD6_FANSPEED_TYPE_RPM)
				return std::to_string(odNFanControl.iCurrentFanSpeed) + " RPM";
			else
				return std::to_string(odNFanControl.iCurrentFanSpeed) + " %";
		}
		else
		{
			spdlog::error("ADL2_OverdriveN_FanControl_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL2_OverdriveN_FanControl_Get from dll");
	}
	throw std::exception("Not support OverdriveN to get fan speed");
}

std::string Hardware::GPU::ADLSDK::OverDriveN::GetGPUMemoryClock(const uint32_t& AdapterId)
{
	if (ADL2_OverdriveN_PerformanceStatus_Get)
	{
		ADLODNPerformanceStatus odNPerformanceStatus{};
		if (auto adl_err = ADL2_OverdriveN_PerformanceStatus_Get(context, AdapterId, &odNPerformanceStatus); ADL_Success(adl_err))
		{
			if (odNPerformanceStatus.iMemoryClock > 0)
				return Utils::to_string_with_precision(odNPerformanceStatus.iMemoryClock / 100.0) + " Mhz";
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
	throw std::exception("Not support OverdriveN to get MemoryClock");
}

std::string Hardware::GPU::ADLSDK::OverDriveN::GetGPUCoreClock(const uint32_t& AdapterId)
{
	if (ADL2_OverdriveN_PerformanceStatus_Get)
	{
		ADLODNPerformanceStatus odNPerformanceStatus{};
		if (auto adl_err = ADL2_OverdriveN_PerformanceStatus_Get(context, AdapterId, &odNPerformanceStatus); ADL_Success(adl_err))
		{
			if (odNPerformanceStatus.iCoreClock > 0)
				return Utils::to_string_with_precision(odNPerformanceStatus.iCoreClock / 100.0) + " Mhz";
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

std::string Hardware::GPU::ADLSDK::OverDriveN::GetGPUVoltage(const uint32_t& AdapterId)
{
	if (ADL2_OverdriveN_PerformanceStatus_Get)
	{
		ADLODNPerformanceStatus odNPerformanceStatus{};
		if (auto adl_err = ADL2_OverdriveN_PerformanceStatus_Get(context, AdapterId, &odNPerformanceStatus); ADL_Success(adl_err))
		{
			if (odNPerformanceStatus.iVDDC > 0)
				return Utils::to_string_with_precision(odNPerformanceStatus.iVDDC / 1000.0) + " V";
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
	throw std::exception("Not support OverdriveN to get Voltage");
}