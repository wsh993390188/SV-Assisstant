#include "stdafx.h"
#include "ADLSDKHelper.h"
#include "OverDrive6.h"

Hardware::GPU::ADLSDK::OverDrive6::OverDrive6(HMODULE hModule) : ADL_Overdrive6_Temperature_Get(nullptr), ADL_Overdrive6_FanSpeed_Get(nullptr),
ADL_Overdrive6_CurrentStatus_Get(nullptr)
{
	ADL_Overdrive6_Temperature_Get = Utils::GetADLFuncAddress<ADL_OVERDRIVE6_TEMPERATURE_GET>(hModule, "ADL_Overdrive6_Temperature_Get");
	ADL_Overdrive6_FanSpeed_Get = Utils::GetADLFuncAddress<ADL_OVERDRIVE6_FANSPEED_GET>(hModule, "ADL_Overdrive6_FanSpeed_Get");
	ADL_Overdrive6_CurrentStatus_Get = Utils::GetADLFuncAddress<ADL_OVERDRIVE6_CURRENTSTATUS_GET>(hModule, "ADL_Overdrive6_CurrentStatus_Get");
}

std::string Hardware::GPU::ADLSDK::OverDrive6::GetTemperature(const uint32_t& AdapterId)
{
	if (ADL_Overdrive6_Temperature_Get)
	{
		int temperature = { 0 };
		if (auto adl_err = ADL_Overdrive6_Temperature_Get(AdapterId, &temperature); ADL_Success(adl_err))
		{
			return std::to_string(temperature / 1000) + " Degree";
		}
		else
		{
			spdlog::error("ADL_Overdrive6_Temperature_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Overdrive6_Temperature_Get from dll");
	}
	throw std::exception("Not support Overdrive6 to get temperature");
}

std::string Hardware::GPU::ADLSDK::OverDrive6::GetFanSpeed(const uint32_t& AdapterId)
{
	if (ADL_Overdrive6_FanSpeed_Get)
	{
		ADLOD6FanSpeedInfo fanSpeedInfo = { 0 };
		if (auto adl_err = ADL_Overdrive6_FanSpeed_Get(AdapterId, &fanSpeedInfo); ADL_Success(adl_err))
		{
			if (ADL_OD6_FANSPEED_TYPE_RPM == (fanSpeedInfo.iSpeedType & ADL_OD6_FANSPEED_TYPE_RPM))
			{
				return std::to_string(fanSpeedInfo.iFanSpeedRPM) + " RPM";
			}
			else
			{
				return std::to_string(fanSpeedInfo.iFanSpeedPercent) + " %";
			}
		}
		else
		{
			spdlog::error("ADL_Overdrive6_FanSpeed_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Overdrive6_FanSpeed_Get from dll");
	}
	throw std::exception("Not support Overdrive6 to get fanspeed");
}

std::string Hardware::GPU::ADLSDK::OverDrive6::GetGPUMemoryClock(const uint32_t& AdapterId)
{
	if (ADL_Overdrive6_CurrentStatus_Get)
	{
		ADLOD6CurrentStatus currentStatus = { 0 };
		if (auto adl_err = ADL_Overdrive6_CurrentStatus_Get(AdapterId, &currentStatus); ADL_Success(adl_err))
		{
			if (currentStatus.iMemoryClock > 0)
				return Utils::to_string_with_precision(currentStatus.iMemoryClock / 100.0) + " MHz";
		}
		else
		{
			spdlog::error("ADL_Overdrive6_CurrentStatus_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Overdrive6_CurrentStatus_Get from dll");
	}
	throw std::exception("Not support Overdrive6 to get MemoryClock");
}

std::string Hardware::GPU::ADLSDK::OverDrive6::GetGPUCoreClock(const uint32_t& AdapterId)
{
	if (ADL_Overdrive6_CurrentStatus_Get)
	{
		ADLOD6CurrentStatus currentStatus = { 0 };
		if (auto adl_err = ADL_Overdrive6_CurrentStatus_Get(AdapterId, &currentStatus); ADL_Success(adl_err))
		{
			if (currentStatus.iEngineClock > 0)
				return Utils::to_string_with_precision(currentStatus.iEngineClock / 100.0) + " MHz";
		}
		else
		{
			spdlog::error("ADL_Overdrive6_CurrentStatus_Get Error: {}", GetADLStatus(adl_err).c_str());
		}
	}
	else
	{
		spdlog::error("cant load ADL_Overdrive6_CurrentStatus_Get from dll");
	}
	throw std::exception("Not support Overdrive6 to get CoreClock");
}

std::string Hardware::GPU::ADLSDK::OverDrive6::GetGPUVoltage(const uint32_t&)
{
	throw std::exception("Not support Overdrive6 to get Voltage");
}