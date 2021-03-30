#include "stdafx.h"
#include "ADLSDKHelper.h"
#include "OverDrive5.h"

Hardware::GPU::ADLSDK::OverDrive5::OverDrive5(HMODULE hModule) : ADL_Overdrive5_Temperature_Get(nullptr),
ADL_Overdrive5_FanSpeed_Get(nullptr), ADL_Overdrive5_CurrentActivity_Get(nullptr)
{
	ADL_Overdrive5_Temperature_Get = Utils::GetADLFuncAddress<ADL_OVERDRIVE5_TEMPERATURE_GET>(hModule, "ADL_Overdrive5_Temperature_Get");
	ADL_Overdrive5_FanSpeed_Get = Utils::GetADLFuncAddress<ADL_OVERDRIVE5_FANSPEED_GET>(hModule, "ADL_Overdrive5_FanSpeed_Get");
	ADL_Overdrive5_CurrentActivity_Get = Utils::GetADLFuncAddress<ADL_OVERDRIVE5_CURRENTACTIVITY_GET>(hModule, "ADL_Overdrive5_CurrentActivity_Get");
}

std::string Hardware::GPU::ADLSDK::OverDrive5::GetTemperature(const uint32_t& AdapterIndex)
{
	if (ADL_Overdrive5_Temperature_Get)
	{
		ADLTemperature adlTemperature = { 0 };
		adlTemperature.iSize = sizeof(ADLTemperature);
		if (auto adl_err = ADL_Overdrive5_Temperature_Get(AdapterIndex, 0, &adlTemperature); ADL_Success(adl_err))
		{
			return std::to_string(adlTemperature.iTemperature / 1000) + " Degree";
		}
		else
		{
			spdlog::error("ADL_Overdrive5_Temperature_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Overdrive5_Temperature_Get from dll");
	}
	throw std::exception("Not support Overdrive5 to get temperature");
}

std::string Hardware::GPU::ADLSDK::OverDrive5::GetFanSpeed(const uint32_t& AdapterId)
{
	try
	{
		return GetFanSpeedRPM(AdapterId);
	}
	catch (const std::exception&)
	{
	}

	return GetFanSpeedPercent(AdapterId);
}

std::string Hardware::GPU::ADLSDK::OverDrive5::GetGPUMemoryClock(const uint32_t& AdapterId)
{
	if (ADL_Overdrive5_CurrentActivity_Get)
	{
		ADLPMActivity activity = { 0 };
		activity.iSize = sizeof(ADLPMActivity);
		if (auto adl_err = ADL_Overdrive5_CurrentActivity_Get(AdapterId, &activity); ADL_Success(adl_err))
		{
			if (activity.iMemoryClock > 0)
				return Utils::to_string_with_precision(activity.iMemoryClock / 100.0) + " Mhz";
		}
		else
		{
			spdlog::error("ADL_Overdrive5_CurrentActivity_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Overdrive5_CurrentActivity_Get from dll");
	}
	throw std::exception("Not support Overdrive5 to get MemoryClock");
}

std::string Hardware::GPU::ADLSDK::OverDrive5::GetGPUCoreClock(const uint32_t& AdapterId)
{
	if (ADL_Overdrive5_CurrentActivity_Get)
	{
		ADLPMActivity activity = { 0 };
		activity.iSize = sizeof(ADLPMActivity);
		if (auto adl_err = ADL_Overdrive5_CurrentActivity_Get(AdapterId, &activity); ADL_Success(adl_err))
		{
			if (activity.iEngineClock > 0)
				return Utils::to_string_with_precision(activity.iEngineClock / 100.0) + " Mhz";
		}
		else
		{
			spdlog::error("ADL_Overdrive5_CurrentActivity_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Overdrive5_CurrentActivity_Get from dll");
	}
	throw std::exception("Not support Overdrive5 to get CoreClock");
}

std::string Hardware::GPU::ADLSDK::OverDrive5::GetGPUVoltage(const uint32_t& AdapterId)
{
	if (ADL_Overdrive5_CurrentActivity_Get)
	{
		ADLPMActivity activity = { 0 };
		activity.iSize = sizeof(ADLPMActivity);
		if (auto adl_err = ADL_Overdrive5_CurrentActivity_Get(AdapterId, &activity); ADL_Success(adl_err))
		{
			return Utils::to_string_with_precision(activity.iVddc / 1000.0) + " V";
		}
		else
		{
			spdlog::error("ADL_Overdrive5_CurrentActivity_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Overdrive5_CurrentActivity_Get from dll");
	}
	throw std::exception("Not support Overdrive5 to get Voltage");
}

std::string Hardware::GPU::ADLSDK::OverDrive5::GetFanSpeedRPM(const uint32_t& AdapterId)
{
	if (ADL_Overdrive5_FanSpeed_Get)
	{
		ADLFanSpeedValue fanSpeedValue = { 0 };
		fanSpeedValue.iSpeedType = ADL_DL_FANCTRL_SPEED_TYPE_RPM;
		fanSpeedValue.iSize = sizeof(ADLFanSpeedValue);
		if (auto adl_err = ADL_Overdrive5_FanSpeed_Get(AdapterId, 0, &fanSpeedValue); ADL_Success(adl_err))
		{
			if (fanSpeedValue.iFanSpeed > 0)
				return std::to_string(fanSpeedValue.iFanSpeed) + " RPM";
		}
		else
		{
			spdlog::error("ADL_Overdrive5_FanSpeed_Get RPM Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Overdrive5_FanSpeed_Get from dll");
	}
	throw std::exception("Not support Overdrive5 to get fan speed");
}

std::string Hardware::GPU::ADLSDK::OverDrive5::GetFanSpeedPercent(const uint32_t& AdapterId)
{
	if (ADL_Overdrive5_FanSpeed_Get)
	{
		ADLFanSpeedValue fanSpeedValue = { 0 };
		fanSpeedValue.iSpeedType = ADL_DL_FANCTRL_SPEED_TYPE_PERCENT;
		fanSpeedValue.iSize = sizeof(ADLFanSpeedValue);
		if (auto adl_err = ADL_Overdrive5_FanSpeed_Get(AdapterId, 0, &fanSpeedValue); ADL_Success(adl_err))
		{
			if (fanSpeedValue.iFanSpeed > 0)
				return std::to_string(fanSpeedValue.iFanSpeed) + " %";
		}
		else
		{
			spdlog::error("ADL_Overdrive5_FanSpeed_Get Percent Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Overdrive5_FanSpeed_Get from dll");
	}
	throw std::exception("Not support Overdrive5 to get fan speed");
}