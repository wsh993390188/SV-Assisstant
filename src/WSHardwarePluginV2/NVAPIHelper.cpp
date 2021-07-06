#include "stdafx.h"
#include "NVAPIHelper.h"

namespace
{
	constexpr inline bool NvAPI_Success(const NvAPI_Status& Status)
	{
		return Status == NVAPI_OK;
	}
}

Hardware::GPU::NVAPIHelper& Hardware::GPU::NVAPIHelper::Instance()
{
	static NVAPIHelper inst;
	return inst;
}

std::string Hardware::GPU::NVAPIHelper::GetVoltage(const uint32_t& DeviceID)
{
	if (!IsNvapiEnable && !NvAPI_GPU_GetVoltages)
	{
		throw NVAPINotExist();
	}

	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceID)
		{
			NV_VOLTAGES Voltage{};
			Voltage.Version = MAKE_NVAPI_VERSION(NV_VOLTAGES, 1);
			Voltage.Flags = 0;
			Voltage.voltages[0].domainId = NVAPI_GPU_PERF_VOLTAGE_INFO_DOMAIN_CORE;
			if (auto status = NvAPI_GPU_GetVoltages(gpu.hPhysicalGpu, &Voltage); NvAPI_Success(status))
			{
				for (auto index = 0; index < Voltage.max; ++index)
				{
					if (Voltage.voltages[index].max < _countof(Voltage.voltages[index].info))
					{
						// MAX Voltage
						return Utils::to_string_with_precision(Voltage.voltages[index].info[Voltage.voltages[index].max - 1].mvolt / 1000.0) + " V";
					}
				}
			}
			else
			{
				spdlog::warn("NvAPI_GPU_GetVoltages Error: {}", GetNvAPIStatusString(status).c_str());
			}
		}
	}

	throw NVAPIDeviceNotExist(DeviceID);
}

std::string Hardware::GPU::NVAPIHelper::GetGPUName(const uint32_t& DeviceID)
{
	if (!IsNvapiEnable)
	{
		throw NVAPINotExist();
	}

	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceID)
		{
			NvAPI_ShortString name = "";

			if (auto status = NvAPI_GPU_GetFullName(gpu.hPhysicalGpu, name); NvAPI_Success(status))
			{
				return std::string("Nvidia ") + name;
			}
			else
			{
				spdlog::warn("NvAPI_GPU_GetFullName Error: {}", GetNvAPIStatusString(status).c_str());
				return "Nvidia GPU";
			}
		}
	}

	throw NVAPIDeviceNotExist(DeviceID);
}

std::string Hardware::GPU::NVAPIHelper::GetGPUDriverVersion()
{
	if (!IsNvapiEnable)
	{
		throw NVAPINotExist();
	}

	NvAPI_ShortString ver{};
	NvU32 DriverVersion{ 0 };
	if (auto status = NvAPI_SYS_GetDriverAndBranchVersion(&DriverVersion, ver); NvAPI_Success(status))
	{
		return std::to_string(DriverVersion / 100) + "." + std::to_string(DriverVersion % 100);
	}
	else
	{
		spdlog::warn("NvAPI_SYS_GetDriverAndBranchVersion Error: {}", GetNvAPIStatusString(status).c_str());
	}

	throw std::exception("Not support get GPU driver version");
}

std::string Hardware::GPU::NVAPIHelper::GetGPUMemoryClock(const uint32_t& DeviceId)
{
	if (!IsNvapiEnable)
	{
		throw NVAPINotExist();
	}

	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceId)
		{
			NV_GPU_CLOCK_FREQUENCIES Fres = {};
			Fres.version = NV_GPU_CLOCK_FREQUENCIES_VER;
			Fres.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;

			if (auto status = NvAPI_GPU_GetAllClockFrequencies(gpu.hPhysicalGpu, &Fres);  NvAPI_Success(status))
			{
				if (Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency / 1000 > 0)
				{
					return Utils::to_string_with_precision(Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency / 1000.0) + " MHz";
				}
				else
				{
					return std::to_string(Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_MEMORY].frequency) + " kHz";
				}
			}
			else
			{
				spdlog::warn("NvAPI_GPU_GetAllClockFrequencies Error: {}", GetNvAPIStatusString(status).c_str());
			}
		}
	}

	throw NVAPIDeviceNotExist(DeviceId);
}

std::string Hardware::GPU::NVAPIHelper::GetGPUCoreClock(const uint32_t& DeviceId)
{
	if (!IsNvapiEnable)
	{
		throw NVAPINotExist();
	}

	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceId)
		{
			NV_GPU_CLOCK_FREQUENCIES Fres = {};
			Fres.version = NV_GPU_CLOCK_FREQUENCIES_VER;
			Fres.ClockType = NV_GPU_CLOCK_FREQUENCIES_CURRENT_FREQ;

			if (auto status = NvAPI_GPU_GetAllClockFrequencies(gpu.hPhysicalGpu, &Fres);  NvAPI_Success(status))
			{
				if (Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency / 1000 > 0)
				{
					return Utils::to_string_with_precision(Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency / 1000.0) + " MHz";
				}
				else
				{
					return std::to_string(Fres.domain[NVAPI_GPU_PUBLIC_CLOCK_GRAPHICS].frequency) + " kHz";
				}
			}
			else
			{
				spdlog::warn("NvAPI_GPU_GetAllClockFrequencies Error: {}", GetNvAPIStatusString(status).c_str());
			}
		}
	}

	throw NVAPIDeviceNotExist(DeviceId);
}

std::string Hardware::GPU::NVAPIHelper::GetGPUUsage(const uint32_t& DeviceID)
{
	if (!IsNvapiEnable)
	{
		throw NVAPINotExist();
	}

	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceID)
		{
			NV_GPU_DYNAMIC_PSTATES_INFO_EX Usage{};
			Usage.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
			if (auto status = NvAPI_GPU_GetDynamicPstatesInfoEx(gpu.hPhysicalGpu, &Usage);  NvAPI_Success(status))
			{
				if (Usage.utilization[0].bIsPresent)
				{
					return std::to_string(Usage.utilization[0].percentage) + " %";
				}
			}
			else
			{
				spdlog::warn("NvAPI_GPU_GetAllClockFrequencies Error: {}", GetNvAPIStatusString(status).c_str());
			}
		}
	}
	throw NVAPIDeviceNotExist(DeviceID);
}

Hardware::GPU::NVAPIHelper::NVAPIHelper() : IsNvapiEnable(false), NvAPI_GPU_GetVoltages(nullptr), NvAPI_GPU_ClientPowerTopologyGetStatus(nullptr)
{
	if (auto Status = NvAPI_Initialize(); NvAPI_Success(Status))
	{
		spdlog::info("NvAPI_Initialize Success");
		IsNvapiEnable = true;
#ifdef _WIN64
		auto nvapi = GetModuleHandle(L"nvapi64.dll");
#else
		auto nvapi = GetModuleHandle(L"nvapi.dll");
#endif // WIN64

		if (nvapi)
		{
			using TYPE_NvAPI_QueryInterface = void* (*)(UINT32 offset);
			auto NvAPI_QueryInterface = Utils::GetADLFuncAddress<TYPE_NvAPI_QueryInterface>(nvapi, "nvapi_QueryInterface");
			if (NvAPI_QueryInterface)
			{
				NvAPI_GPU_GetVoltages = reinterpret_cast<decltype(NvAPI_GPU_GetVoltages)>(NvAPI_QueryInterface(0x7D656244));
				NvAPI_GPU_ClientPowerTopologyGetStatus = reinterpret_cast<decltype(NvAPI_GPU_ClientPowerTopologyGetStatus)>(NvAPI_QueryInterface(0x0EDCF624E));
			}
		}

		EnumPhysicalGPUs();
	}
	else
	{
		spdlog::warn("NvAPI_Initialize Error: {}", GetNvAPIStatusString(Status).c_str());
	}
}

Hardware::GPU::NVAPIHelper::~NVAPIHelper()
{
	if (IsNvapiEnable)
	{
		NvAPI_Unload();
	}
}

std::string Hardware::GPU::NVAPIHelper::GetNvAPIStatusString(const NvAPI_Status& nvapiErrorStatus)
{
	NvAPI_ShortString errorDescStr;
	NvAPI_GetErrorMessage(nvapiErrorStatus, errorDescStr);
	return errorDescStr;
}

void Hardware::GPU::NVAPIHelper::EnumPhysicalGPUs()
{
	NvPhysicalGpuHandle nvGPUHandle[NVAPI_MAX_PHYSICAL_GPUS] = { 0 };
	NvU32 gpuCount = 0;
	if (auto Status = NvAPI_EnumPhysicalGPUs(nvGPUHandle, &gpuCount); NvAPI_Success(Status))
	{
		for (NvU32 Count = 0; Count < gpuCount; Count++) // iterating per Physical GPU Handle call
		{
			spdlog::info("NVAPI query GPU count:{}", gpuCount);
			NVGPUPhysicalInfo TempQueryInfo{};
			Status = NvAPI_GPU_GetPCIIdentifiers(nvGPUHandle[Count], &TempQueryInfo.MajorId,
				&TempQueryInfo.SubId, &TempQueryInfo.RevisionId, &TempQueryInfo.ExtDeviceId);
			if (NvAPI_Success(Status))
			{
				spdlog::info("Find DID:{:x} Success", TempQueryInfo.DeviceId);
				TempQueryInfo.hPhysicalGpu = nvGPUHandle[Count];
				GpuQueryDatas.emplace_back(std::move(TempQueryInfo));
			}
			else
			{
				spdlog::warn("NvAPI_GPU_GetPCIIdentifiers Error: {}", GetNvAPIStatusString(Status).c_str());
			}
		}
	}
	else
	{
		spdlog::warn("NvAPI_EnumPhysicalGPUs Error: {}", GetNvAPIStatusString(Status).c_str());
	}
}

std::string Hardware::GPU::NVAPIHelper::GetTemperature(const uint32_t& DeviceID)
{
	if (!IsNvapiEnable)
	{
		throw NVAPINotExist();
	}

	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceID)
		{
			return std::to_string(GetTemperatureImpl(gpu.hPhysicalGpu, NVAPI_THERMAL_TARGET_ALL));
		}
	}

	throw NVAPIDeviceNotExist(DeviceID);
}

std::string Hardware::GPU::NVAPIHelper::GetFanSpeed(const uint32_t& DeviceID)
{
	if (!IsNvapiEnable)
	{
		throw NVAPINotExist();
	}

	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceID)
		{
			NvU32 FanSpeed{ 0 };
			if (auto status = NvAPI_GPU_GetTachReading(gpu.hPhysicalGpu, &FanSpeed); NvAPI_Success(status))
			{
				return std::to_string(FanSpeed);
			}
			else
			{
				spdlog::warn("NvAPI_GPU_GetTachReading Error: {}", GetNvAPIStatusString(status).c_str());
			}
		}
	}

	throw NVAPIDeviceNotExist(DeviceID);
}

std::string Hardware::GPU::NVAPIHelper::GetGPUPower(const uint32_t& DeviceID)
{
	if (!IsNvapiEnable || !NvAPI_GPU_ClientPowerTopologyGetStatus)
	{
		throw NVAPINotExist();
	}

	for (const auto& gpu : GpuQueryDatas)
	{
		if (gpu.DeviceId == DeviceID)
		{
			NV_POWER_TOPOLOGY_STATUS GPUPower{};
			GPUPower.version = MAKE_NVAPI_VERSION(decltype(GPUPower), 1);
			if (auto status = NvAPI_GPU_ClientPowerTopologyGetStatus(gpu.hPhysicalGpu, &GPUPower); NvAPI_Success(status))
			{
			}
			else
			{
				spdlog::warn("NvAPI_GPU_ClientPowerTopologyGetStatus Error: {}", GetNvAPIStatusString(status).c_str());
			}
		}
	}

	throw NVAPIDeviceNotExist(DeviceID);
}

NvS32 Hardware::GPU::NVAPIHelper::GetTemperatureImpl(const NvPhysicalGpuHandle& GPUHandle, const NV_THERMAL_TARGET& target)
{
	NvAPI_Status status = NVAPI_OK;
	NV_GPU_THERMAL_SETTINGS Thermal = {};
	Thermal.version = NV_GPU_THERMAL_SETTINGS_VER;
	Thermal.count = 1;

	if (auto status = NvAPI_GPU_GetThermalSettings(GPUHandle, target, &Thermal); NvAPI_Success(status))
	{
		for (const auto& sensor : Thermal.sensor)
		{
			if (sensor.target == NVAPI_THERMAL_TARGET_GPU)
				return sensor.currentTemp;
		}
	}
	else
	{
		spdlog::warn("NvAPI_GPU_GetThermalSettings Error: {}", GetNvAPIStatusString(status).c_str());
	}
	throw NVAPITemperatureExp(target);
}

void Hardware::GPU::NVAPIHelper::GetDynamicInfo(const NvPhysicalGpuHandle& GPUHandle)
{
	NvAPI_Status status = NVAPI_OK;
	NV_GPU_DYNAMIC_PSTATES_INFO_EX PStatus = {};
	PStatus.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;

	if (auto status = NvAPI_GPU_GetDynamicPstatesInfoEx(GPUHandle, &PStatus); NvAPI_Success(status))
	{
	}
	else
	{
		spdlog::warn("NvAPI_GPU_GetThermalSettings Error: {}", GetNvAPIStatusString(status).c_str());
	}
	throw std::exception("Not Support");
}

char const* Hardware::GPU::NVAPINotExist::what() const
{
	return "NVAPI cant initialize";
}

char const* Hardware::GPU::NVAPIDeviceNotExist::what() const
{
	return Msg.c_str();
}

char const* Hardware::GPU::NVAPITemperatureExp::what() const
{
	return Msg.c_str();
}

std::string Hardware::GPU::NVAPITemperatureExp::TargetToString() const
{
	switch (target)
	{
	case NVAPI_THERMAL_TARGET_NONE:
		return"None";
	case NVAPI_THERMAL_TARGET_GPU:
		return"GPU";
	case NVAPI_THERMAL_TARGET_MEMORY:
		return"Memory";
	case NVAPI_THERMAL_TARGET_POWER_SUPPLY:
		return"Power";
	case NVAPI_THERMAL_TARGET_BOARD:
		return"Broad";
	case NVAPI_THERMAL_TARGET_ALL:
		return "All";
	}

	return{};
}