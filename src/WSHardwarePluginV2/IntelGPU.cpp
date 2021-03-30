#include "stdafx.h"
#include "IntelGPU.h"
#include "DXGIGPUBase.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

namespace
{
	constexpr auto INTELGPU_TEMPERATURE = 0x5980;
	constexpr auto INTELGPU_ENGINE_FREQUENCY = 0x5948;
	constexpr auto INTELGPU_MEMORY_FREQUERCY = 0x5E00;
	constexpr auto INTELGPU_RP_FREQUENCY = 0x5998;
	constexpr auto INTELGPU_ENGINE_FREQUENCY_MULTIPLIER = 50ull;
	constexpr auto INTELGPU_MEMORY_BASE_FREQUENCY = 400 / 3.0;
}

Hardware::GPU::IntelGPU::IntelGPU(const GPUDevice& GpuData) :
	GPUDeviceBase(GpuData), GpuDatas{}
{
	if (GPUBaseData.BarAddress != GPU::InvaildMemoryBase)
	{
		m_Decorators.emplace_back(std::make_unique<IntelGPUTemperature>(this->GPUBaseData.BarAddress + INTELGPU_TEMPERATURE, "Temperature"));
		//m_Decorators.emplace_back(std::make_unique<IntelGPUEngineClock>(this->GPUBaseData.BarAddress + INTELGPU_ENGINE_FREQUENCY, "Engine Clock"));
		m_Decorators.emplace_back(std::make_unique<IntelGPUMemoryClock>(this->GPUBaseData.BarAddress + INTELGPU_MEMORY_FREQUERCY, "Memory Clock"));

		auto MemoryPtr = std::make_shared<Utils::Ring0::SafeMemoryHandle>();
		DWORD RPData{};
		if (MemoryPtr->ReadDWORD(this->GPUBaseData.BarAddress + INTELGPU_RP_FREQUENCY, RPData) && RPData)
		{
			auto RPN = Utils::extract_bits_ui(RPData, 16, 23);
			auto RP1 = Utils::extract_bits_ui(RPData, 8, 15);
			GpuDatas.GPUMinFreq = (RPN < RP1 ? RPN : RP1) * 50;
			GpuDatas.GPUMaxFreq = Utils::extract_bits_ui(RPData, 0, 7) * 50;
		}
	}
}

std::string Hardware::GPU::IntelGPU::UpdateGPUInfo()
{
	Json::Value root;
	auto MemoryPtr = std::make_shared<Utils::Ring0::SafeMemoryHandle>();
	for (auto& decorator : m_Decorators)
	{
		Json::Value temp;
		decorator->Update(MemoryPtr);
		temp[decorator->GetDecoratorName()] = decorator->GetDecoratorValue();
		root.append(temp);
	}
	return Json::FastWriter().write(root);
}

std::string Hardware::GPU::IntelGPU::GetGPUInfo()
{
	Json::Value root;
	{
		if (GpuDatas.GPUMinFreq)
		{
			Json::Value temp;
			temp["GPUMinFreq"] = std::to_string(GpuDatas.GPUMinFreq) + " Mhz";
			root.append(temp);
		}
	}

	{
		if (GpuDatas.GPUMinFreq)
		{
			Json::Value temp;
			temp["GPUMaxFreq"] = std::to_string(GpuDatas.GPUMaxFreq) + " Mhz";
			root.append(temp);
		}
	}

	GPUDXBaseData data;
	if (DXGIGPUBase::Instance().QueryGPUInfo({ GPUBaseData.VendorId, GPUBaseData.DeviceId }, data) == Data::ErrorType::SUCCESS)
	{
		{
			if (!data.Description.empty())
			{
				Json::Value temp;
				temp["Description"] = Utils::wstringToUtf8(data.Description);
				root.append(temp);
			}

			if (data.SharedSystemMemory)
			{
				Json::Value temp;
				temp["Shared memory"] = Utils::MemoryToStringWithUnit(data.SharedSystemMemory);
				root.append(temp);
			}

			if (data.DedicatedVideoMemory)
			{
				Json::Value temp;
				temp["GPU memory"] = Utils::MemoryToStringWithUnit(data.DedicatedVideoMemory);
				root.append(temp);
			}
		}
	}

	return Json::FastWriter().write(root);
}

Hardware::GPU::IntelGPUTemperature::IntelGPUTemperature(const uint64_t& MemoryBase, const std::string& Name) : GPUDecorator(MemoryBase, Name)
{
	m_TjCurrent = (std::numeric_limits<int64_t>::max)();
}

const std::string Hardware::GPU::IntelGPUTemperature::GetDecoratorValue() const
{
	if (m_TjCurrent != (std::numeric_limits<int64_t>::max)())
	{
		if (IsUpdate)
		{
			IsUpdate = false;
			return std::to_string(m_TjCurrent) + " Degree";
		}
	}
	return {};
}

void Hardware::GPU::IntelGPUTemperature::Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr)
{
	if (!MemoryPtr.expired())
	{
		auto MemoryPtrReal = MemoryPtr.lock();
		DWORD value{};
		auto res = MemoryPtrReal->ReadDWORD(MemoryBase, value);
		if (res && value)
		{
			IsUpdate = true;
			m_TjCurrent = value;
		}
		else
		{
			spdlog::error("IntelGPUTemperature Read Memory {:x} Status:{} maybe read value:{}", MemoryBase, res ? "true" : "false", value);
		}
	}
}

Hardware::GPU::IntelGPUEngineClock::IntelGPUEngineClock(const uint64_t& MemoryBase, const std::string& Name) :
	GPUDecorator(MemoryBase, Name)
{
	m_EngineClock = (std::numeric_limits<int64_t>::max)();
}

const std::string Hardware::GPU::IntelGPUEngineClock::GetDecoratorValue() const
{
	if (m_EngineClock != (std::numeric_limits<int64_t>::max)())
	{
		if (IsUpdate)
		{
			IsUpdate = false;
			return std::to_string(m_EngineClock) + " Mhz";
		}
	}
	return {};
}

void Hardware::GPU::IntelGPUEngineClock::Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr)
{
	if (!MemoryPtr.expired())
	{
		auto MemoryPtrReal = MemoryPtr.lock();
		DWORD value{};
		auto res = MemoryPtrReal->ReadDWORD(MemoryBase, value);
		if (res && value)
		{
			IsUpdate = true;
			m_EngineClock = Utils::extract_bits(value, 8, 15) * INTELGPU_ENGINE_FREQUENCY_MULTIPLIER;
		}
		else
		{
			spdlog::error("IntelGPUEngineClock Read Memory {:x} Status:{} maybe read value:{}", MemoryBase, res ? "true" : "false", value);
		}
	}
}

Hardware::GPU::IntelGPUMemoryClock::IntelGPUMemoryClock(const uint64_t& MemoryBase, const std::string& Name) :
	GPUDecorator(MemoryBase, Name)
{
	m_MemoryClock = (std::numeric_limits<double>::max)();
}

const std::string Hardware::GPU::IntelGPUMemoryClock::GetDecoratorValue() const
{
	if (m_MemoryClock != (std::numeric_limits<double>::max)())
	{
		if (IsUpdate)
		{
			IsUpdate = false;
			return Utils::to_string_with_precision(m_MemoryClock) + " Mhz";
		}
	}
	return {};
}

void Hardware::GPU::IntelGPUMemoryClock::Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr)
{
	if (!MemoryPtr.expired())
	{
		auto MemoryPtrReal = MemoryPtr.lock();
		DWORD value{};
		auto res = MemoryPtrReal->ReadDWORD(MemoryBase, value);
		if (res && value)
		{
			IsUpdate = true;
			m_MemoryClock = INTELGPU_MEMORY_BASE_FREQUENCY * Utils::extract_bits_ui(value, 0, 3);
		}
		else
		{
			spdlog::error("IntelGPUMemoryClock Read Memory {:x} Status:%s maybe read value:{}", MemoryBase, res ? "true" : "false", value);
		}
	}
}