#include "stdafx.h"
#include "IntelGPU.h"
#include "PCIUtils.h"

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

Hardware::GPU::IntelGPU::IntelGPU(const GPUDevice& GpuData, std::unique_ptr<GPUAdapter>&& Adapter) :
	GPUDeviceBase(GpuData, std::move(Adapter)), GpuDatas{}
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
		try
		{
			Json::Value temp;
			decorator->Update(MemoryPtr);
			temp[decorator->GetDecoratorName()] = decorator->GetDecoratorValue();
			root.append(temp);
		}
		catch (const std::exception&)
		{
			continue;
		}
	}
	return Json::FastWriter().write(root);
}

std::string Hardware::GPU::IntelGPU::GetGPUInfo()
{
	Json::Value root;

	if (!SubVendor.empty())
	{
		Json::Value temp;
		temp["Vendor"] = SubVendor;
		root.append(temp);
	}

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

	{
		bool HasMemory = false;
		if (this->m_Adapter)
		{
			if (this->m_Adapter->SharedMemory)
			{
				Json::Value temp;
				temp["Shared memory"] = Utils::MemoryToStringWithUnit(this->m_Adapter->SharedMemory);
				root.append(temp);
			}

			if (this->m_Adapter->DedicatedMemory)
			{
				Json::Value temp;
				temp["GPU memory"] = Utils::MemoryToStringWithUnit(this->m_Adapter->DedicatedMemory);
				root.append(temp);
			}

			if (!this->m_Adapter->Descriptor.empty())
			{
				Json::Value temp;
				temp["Description"] = Utils::wstringToUtf8(this->m_Adapter->Descriptor);
				root.append(temp);
				if (auto QueryInfo = ParserGPUName(this->m_Adapter->Descriptor); QueryInfo)
				{
					if (auto GPUDBInfo = GPUConfig::Instance().FindElements(*QueryInfo); GPUDBInfo)
					{
						if (GPUDBInfo->ChipName)
						{
							Json::Value temp;
							temp["Chip Name"] = GPUDBInfo->ChipName.Element;
							root.append(temp);
						}

						if (GPUDBInfo->Shaders)
						{
							Json::Value temp;
							temp["Shaders"] = GPUDBInfo->Shaders.Element;
							root.append(temp);
						}

						if (GPUDBInfo->TMUs)
						{
							Json::Value temp;
							temp["TMUs"] = GPUDBInfo->TMUs.Element;
							root.append(temp);
						}

						if (GPUDBInfo->ROPs)
						{
							Json::Value temp;
							temp["ROPs"] = GPUDBInfo->ROPs.Element;
							root.append(temp);
						}

						if (!HasMemory && GPUDBInfo->MemorySize)
						{
							Json::Value temp;
							temp["Memory Size"] = GPUDBInfo->MemorySize.Element;
							root.append(temp);
						}

						if (HasMemory && GPUDBInfo->MemoryType)
						{
							Json::Value temp;
							temp["Memory Type"] = GPUDBInfo->MemoryType.Element;
							root.append(temp);
						}

						if (HasMemory && GPUDBInfo->MemoryBits)
						{
							Json::Value temp;
							temp["Memory Bits"] = GPUDBInfo->MemoryBits.Element;
							root.append(temp);
						}
					}
				}
			}

			LARGE_INTEGER nFreq, nEndTime;
			QueryPerformanceFrequency(&nFreq);
			for (auto& node : this->m_Adapter->Nodes)
			{
				this->UpdateNode(node);
			}
			QueryPerformanceCounter(&nEndTime);
			TotalRunningTime.Delta = nEndTime.QuadPart - TotalRunningTime.Value;
			TotalRunningTime.Value = nEndTime.QuadPart;
			auto elapsedTime = (double)(TotalRunningTime.Delta * 100000ULL / nFreq.QuadPart);
			for (const auto& node : this->m_Adapter->Nodes)
			{
				auto usage = node.Delta / elapsedTime;
				if (usage > 100.0)
					usage = 100.0;
				if (usage > 1e-2)
				{
					Json::Value temp;
					temp[node.Name] = std::format("{:.2f} %", usage);
					root.append(temp);
				}
			}
		}
	}

	return Json::FastWriter().write(root);
}

std::unique_ptr<Hardware::XMLConfig::QueryInfo> Hardware::GPU::IntelGPU::ParserGPUName(std::wstring GPUName)
{
	auto ret = std::make_unique<Hardware::XMLConfig::QueryInfo>();
	Utils::replace(GPUName, std::wstring(L"(R)"), std::wstring(L""));
	Utils::replace(GPUName, std::wstring(L"Intel"), std::wstring(L""));
	Utils::replace(GPUName, std::wstring(L"TM"), std::wstring(L""));
	Utils::trim(GPUName);
	ret->Manufacture = "Intel";
	ret->Model = Utils::wstringToUtf8(GPUName);
	return ret;
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
	throw std::exception("Cannot get IntelGPUTemperature");
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
	throw std::exception("Cannot get IntelGPUEngineClock");
}

void Hardware::GPU::IntelGPUEngineClock::Update(std::weak_ptr<Utils::Ring0::SafeMemoryHandle> MemoryPtr)
{
	if (!MemoryPtr.expired())
	{
		auto MemoryPtrReal = MemoryPtr.lock();
		DWORD value{}, count = 0;
		while (count++ < 100000)
		{
			auto res = MemoryPtrReal->ReadDWORD(MemoryBase, value);
			if (res && value)
			{
				IsUpdate = true;
				m_EngineClock = Utils::extract_bits(value, 8, 15) * INTELGPU_ENGINE_FREQUENCY_MULTIPLIER;
				break;
			}
			else
			{
				//spdlog::error("IntelGPUEngineClock Read Memory {:x} Status:{} maybe read value:{}", MemoryBase, res ? "true" : "false", value);
			}
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
	throw std::exception("Cannot get IntelGPUMemoryClock");
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