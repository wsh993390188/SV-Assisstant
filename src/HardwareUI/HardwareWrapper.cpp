#include "framework.hpp"
#include <filesystem>
#include <format>
#include "HardwareWrapper.h"

using namespace Hardware::HardwareWrapper;

namespace
{
#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif
	std::string BuildElement(const size_t& Id, const std::string& JsonName)
	{
		try
		{
			Json::Value UpdateJson;
			UpdateJson[JsonName] = Id;
			return Json::FastWriter().write(UpdateJson);
		}
		catch (Json::Exception&)
		{
		}
		return{};
	}

	std::string BuildElement(const std::string& Id, const std::string& JsonName)
	{
		try
		{
			Json::Value UpdateJson;
			UpdateJson[JsonName] = Id;
			return Json::FastWriter().write(UpdateJson);
		}
		catch (Json::Exception&)
		{
		}
		return{};
	}

	std::string BuildCPUUpdateElement(const size_t& SocketId)
	{
		try
		{
			Json::Value UpdateJson;
			Json::Value temp;
			temp["SocketId"] = SocketId;
			UpdateJson.append(temp);
			return Json::FastWriter().write(UpdateJson);
		}
		catch (Json::Exception&)
		{
		}
		return{};
	}

	std::string BuildCPUUpdateElement(const size_t& SocketId, const size_t& CoreId, const size_t& ThreadId)
	{
		Json::Value UpdateJson;
		Json::Value temp;
		temp["SocketId"] = SocketId;
		temp["CoreId"] = CoreId;
		temp["ThreadId"] = ThreadId;
		UpdateJson.append(temp);
		return Json::FastWriter().write(UpdateJson);
	}

	std::vector<std::pair<std::wstring, std::wstring>> ParserCache(const Json::Value& CacheJson)
	{
		std::vector<std::pair<std::wstring, std::wstring>>ret;
		if (CacheJson.isArray())
		{
			for (const auto& Cache : CacheJson)
			{
				std::wstring Name, Value;
				if (Cache["Level"].isIntegral())
				{
					Name += (L"Level " + std::to_wstring(Cache["Level"].asUInt()));
				}

				if (Cache["Type"].isString())
				{
					Name += utf8_decode(Cache["Type"].asString());
				}

				if (Cache["Number"].isIntegral())
				{
					auto Number = Cache["Number"].asUInt();
					if (Number > 1)
					{
						Value += std::to_wstring(Number) + L" x ";
					}
				}

				if (Cache["Size"].isString())
				{
					Value += utf8_decode(Cache["Size"].asString());
				}
				ret.emplace_back(std::move(Name), std::move(Value));
			}
		}
		return ret;
	}

	std::vector<std::pair<std::wstring, std::wstring>> ParserResponse(const std::string& JsonStr)
	{
		Json::Reader reader;
		Json::Value root;
		std::vector<std::pair<std::wstring, std::wstring>> ret;
		if (reader.parse(JsonStr, root))
		{
			for (const auto& node : root)
			{
				Json::Value::Members arrayMember = node.getMemberNames();
				for (auto iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //遍历json成员
				{
					std::string member_name = *iter;
					if (node[member_name].isString())
					{
						std::string value_str = node[member_name].asString();
						ret.emplace_back(utf8_decode(member_name), utf8_decode(value_str));
					}
					else if (node[member_name].isUInt64())
					{
						auto value_str = node[member_name].asUInt64();
						ret.emplace_back(utf8_decode(member_name), std::to_wstring(value_str));
					}
					else if (node[member_name].isArray())
					{
						if (member_name == "Caches")
						{
							auto Caches = ParserCache(node[member_name]);
							ret.insert(ret.cend(), Caches.cbegin(), Caches.cend());
						}
						else
						{
							auto i = 1;
							for (const auto& value_str : node[member_name])
							{
								try
								{
									ret.emplace_back(utf8_decode(std::format("{} # {}", member_name, i)), utf8_decode(value_str.asString()));
									++i;
								}
								catch (const std::exception&)
								{
									continue;
								}
							}
						}
					}
					else if (node[member_name].isBool())
					{
						auto value_str = node[member_name].asBool();
						ret.emplace_back(utf8_decode(member_name), value_str ? L"true" : L"false");
					}
				}
			}
		}
		return ret;
	}

	std::vector<std::pair<std::wstring, std::wstring>> ParserResponse(const Json::Value& root)
	{
		std::vector<std::pair<std::wstring, std::wstring>> ret;
		for (const auto& node : root)
		{
			Json::Value::Members arrayMember = node.getMemberNames();
			for (auto iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //遍历json成员
			{
				std::string member_name = *iter;
				if (node[member_name].isString())
				{
					std::string value_str = node[member_name].asString();
					ret.emplace_back(utf8_decode(member_name), utf8_decode(value_str));
				}
				else if (node[member_name].isUInt64())
				{
					auto value_str = node[member_name].asUInt64();
					ret.emplace_back(utf8_decode(member_name), std::to_wstring(value_str));
				}
				else if (node[member_name].isBool())
				{
					auto value_str = node[member_name].asBool();
					ret.emplace_back(utf8_decode(member_name), value_str ? L"true" : L"false");
				}
			}
		}
		return ret;
	}

	std::vector<std::pair<std::wstring, std::wstring>> ParserCPUUpdateResponse(const std::string& JsonStr, const std::string& JsonValue)
	{
		Json::Reader reader;
		Json::Value root;

		std::vector<std::pair<std::wstring, std::wstring>> ret;

		if (reader.parse(JsonStr, root))
		{
			auto ThreadRoot = root[JsonValue];

			for (auto& Thread : ThreadRoot)
			{
				for (auto& ele : Thread["Element"])
				{
					ret.emplace_back(utf8_decode(ele["Name"].asString()), utf8_decode(ele["Value"].asString()));
				}
			}
		}
		return ret;
	}

	std::vector<Disk::DiskQueryInfo> ParserDiskInitializeJson(const std::string& Json)
	{
		Json::Reader reader;
		Json::Value root;
		std::vector<Disk::DiskQueryInfo> DriverMap;
		if (reader.parse(Json, root))
		{
			if (root.isNull())
				return DriverMap;
			for (const auto& temp : root)
			{
				if (temp["Id"].isInt())
				{
					Disk::DiskQueryInfo TempInfo(temp["Id"].asInt());
					try
					{
						for (const auto& Str : temp["Elements"])
						{
							if (Str.isString())
								TempInfo.StaticElements.push_back(Str.asString());
						}
					}
					catch (...)
					{
					}

					try
					{
						for (const auto& Str : temp["UpdateElements"])
						{
							if (Str.isString())
								TempInfo.UpdateElements.push_back(Str.asString());
						}
					}
					catch (...)
					{
					}
					DriverMap.emplace_back(std::move(TempInfo));
				}
			}
		}
		return DriverMap;
	}

	std::string BuildDiskJson(const int& CurrentId, const std::vector<std::string>& DiskDisplayName)
	{
		Json::Value temp;
		temp["Id"] = CurrentId;
		Json::Value Ele;
		for (const auto& name : DiskDisplayName)
		{
			Ele.append(name);
		}
		temp["Elements"] = Ele;
		return Json::FastWriter().write(temp);
	}
#if defined(_MSC_VER)
#pragma warning(default : 4996)
#endif
}

std::vector<CPU::CPUSocket> CPU::InitializeCPU()
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_CPU_INIT, nullptr, &data);
	if (data)
	{
		auto InitializeJson = utf8_encode(data);
		SysFreeString(data);
		try
		{
			Json::Reader reader;
			Json::Value root;
			if (reader.parse(InitializeJson, root))
			{
				Json::Value temp;
				CPUSocket Socket;
				Socket.SocketID = root["SocketId"].asUInt();
				for (const auto& CoreEle : root["CoreIds"])
				{
					CPUCore Core;
					Core.CoreId = CoreEle["CoreId"].asUInt();
					for (const auto& ThreadEle : CoreEle["ThreadIds"])
					{
						Core.ThreadIds.emplace_back(ThreadEle["ThreadId"].asUInt());
					}
					Socket.Cores.emplace_back(std::move(Core));
				}
				return { Socket };
			}
		}
		catch (Json::Exception&)
		{
		}
	}

	return{};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::CPU::GetElements(const size_t& SocketId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_CPU_GET, BuildElement(SocketId, "SocketId").c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::CPU::UpdateElements(const size_t& SocketId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_CPU_UPDATE, BuildCPUUpdateElement(SocketId).c_str(), &data);
	if (data)
	{
		auto ret = ParserCPUUpdateResponse(utf8_encode(data), "Socket");
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::CPU::UpdateElements(const size_t& SocketId, const size_t& CoreId, const size_t& ThreadId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_CPU_UPDATE, BuildCPUUpdateElement(SocketId, CoreId, ThreadId).c_str(), &data);
	if (data)
	{
		auto ret = ParserCPUUpdateResponse(utf8_encode(data), "Thread");
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<GPU::GPUCore> Hardware::HardwareWrapper::GPU::InitializeGPU()
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_GPU_INIT, nullptr, &data);
	if (data)
	{
		auto Str = utf8_encode(data);
		SysFreeString(data);
		try
		{
			Json::Reader reader;
			Json::Value root;
			std::vector<GPU::GPUCore> ret;
			if (reader.parse(Str, root))
			{
				for (const auto& MemoryEle : root["GPUIds"])
				{
					if (MemoryEle.isIntegral())
					{
						ret.emplace_back(MemoryEle.asUInt());
					}
				}
			}
			return ret;
		}
		catch (Json::Exception&)
		{
		}
	}
	return{};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::GPU::GetElements(const size_t& GpuId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_GPU_GET, BuildElement(GpuId, "GPUId").c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::GPU::UpdateElements(const size_t& GpuId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_GPU_UPDATE, BuildElement(GpuId, "GPUId").c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<Disk::DiskQueryInfo> Hardware::HardwareWrapper::Disk::InitializeDisk()
{
	std::vector<DiskQueryInfo> ret;
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_DISK_INIT, nullptr, &data);
	if (data)
	{
		ret = ParserDiskInitializeJson(utf8_encode(data));
		SysFreeString(data);
	}

	return ret;
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Disk::GetElements(const DiskQueryInfo& DiskInfo)
{
	BSTR data{};
	std::vector<std::pair<std::wstring, std::wstring>>ret;
	PcmHardwareAction(PCM_HARDWARE_ACTION_DISK_GET, BuildDiskJson(DiskInfo.DiskId, DiskInfo.StaticElements).c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return ret;
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Disk::UpdateElements(const DiskQueryInfo& DiskInfo)
{
	BSTR data{};
	std::vector<std::pair<std::wstring, std::wstring>> ret;
	PcmHardwareAction(PCM_HARDWARE_ACTION_DISK_UPDATE, BuildDiskJson(DiskInfo.DiskId, DiskInfo.UpdateElements).c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return ret;
}

std::vector<size_t> Hardware::HardwareWrapper::Memory::InitializeMemory()
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_MEMORY_INIT, nullptr, &data);
	if (data)
	{
		auto Str = utf8_encode(data);
		SysFreeString(data);
		try
		{
			Json::Reader reader;
			Json::Value root;
			std::vector<size_t> ret;
			if (reader.parse(Str, root))
			{
				for (const auto& MemoryEle : root["MemoryIds"])
				{
					if (MemoryEle.isIntegral())
					{
						ret.emplace_back(MemoryEle.asUInt());
					}
				}
			}
			return ret;
		}
		catch (Json::Exception&)
		{
		}
	}
	return{};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Memory::GetElements(const size_t& MemoryId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_MEMORY_GET, BuildElement(MemoryId, "MemoryId").c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Memory::UpdateElements(const size_t& MemoryId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_MEMORY_UPDATE, BuildElement(MemoryId, "MemoryId").c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<size_t> Hardware::HardwareWrapper::Battery::InitializeBattery()
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_BATTERY_INIT, nullptr, &data);
	if (data)
	{
		auto Str = utf8_encode(data);
		SysFreeString(data);
		try
		{
			Json::Reader reader;
			Json::Value root;
			std::vector<size_t> ret;
			if (reader.parse(Str, root))
			{
				for (const auto& MemoryEle : root["BatteryIds"])
				{
					if (MemoryEle.isIntegral())
					{
						ret.emplace_back(MemoryEle.asUInt());
					}
				}
			}
			return ret;
		}
		catch (Json::Exception&)
		{
		}
	}
	return{};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Battery::UpdateElements(const size_t& MemoryId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_BATTERY_UPDATE, BuildElement(MemoryId, "BatteryId").c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector <std::string> Hardware::HardwareWrapper::Monitor::InitializeMonitor()
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_MONITOR_INIT, nullptr, &data);
	if (data)
	{
		auto Str = utf8_encode(data);
		SysFreeString(data);
		try
		{
			Json::Reader reader;
			Json::Value root;
			std::vector<std::string> ret;
			if (reader.parse(Str, root))
			{
				for (const auto& MemoryEle : root["MonitorIds"])
				{
					if (MemoryEle.isString())
					{
						ret.emplace_back(MemoryEle.asString());
					}
				}
			}
			return ret;
		}
		catch (Json::Exception&)
		{
		}
	}
	return{};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Monitor::GetElements(const std::string& MemoryId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_MONITOR_GET, BuildElement(MemoryId, "MonitorId").c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<size_t> Hardware::HardwareWrapper::Motherbroad::InitializeMotherbroad()
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_BROAD_INIT, nullptr, &data);
	if (data)
	{
		auto Str = utf8_encode(data);
		SysFreeString(data);
		try
		{
			Json::Reader reader;
			Json::Value root;
			std::vector<size_t> ret;
			if (reader.parse(Str, root))
			{
				for (const auto& MemoryEle : root["BroadId"])
				{
					if (MemoryEle.isIntegral())
					{
						ret.emplace_back(MemoryEle.asUInt());
					}
				}
			}
			return ret;
		}
		catch (Json::Exception&)
		{
		}
	}
	return{};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Motherbroad::GetElements(const size_t& MemoryId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_BROAD_GET, BuildElement(MemoryId, "BroadId").c_str(), &data);
	std::vector<std::pair<std::wstring, std::wstring>> ret{};
	if (data)
	{
		try
		{
			Json::Reader reader;
			Json::Value root;
			if (reader.parse(utf8_encode(data), root))
			{
				if (root["Broad"].isArray())
				{
					ret = ParserResponse(root["Broad"]);
				}
			}
		}
		catch (Json::Exception& e)
		{
		}
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<size_t> Hardware::HardwareWrapper::Net::InitializeNet()
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_NIC_INIT, nullptr, &data);
	if (data)
	{
		auto Str = utf8_encode(data);
		SysFreeString(data);
		try
		{
			Json::Reader reader;
			Json::Value root;
			std::vector<size_t> ret;
			if (reader.parse(Str, root))
			{
				for (const auto& MemoryEle : root["NICIds"])
				{
					if (MemoryEle.isIntegral())
					{
						ret.emplace_back(MemoryEle.asUInt());
					}
				}
			}
			return ret;
		}
		catch (Json::Exception&)
		{
		}
	}
	return{};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Net::UpdateElements(const size_t& MemoryId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_NIC_UPDATE, BuildElement(MemoryId, "NICId").c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Net::GetElements(const size_t& MemoryId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_NIC_GET, BuildElement(MemoryId, "NICId").c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<size_t> Hardware::HardwareWrapper::Audio::InitializeAudio()
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_AUDIO_INIT, nullptr, &data);
	if (data)
	{
		auto Str = utf8_encode(data);
		SysFreeString(data);
		try
		{
			Json::Reader reader;
			Json::Value root;
			std::vector<size_t> ret;
			if (reader.parse(Str, root))
			{
				for (const auto& MemoryEle : root["AudioIds"])
				{
					if (MemoryEle.isIntegral())
					{
						ret.emplace_back(MemoryEle.asUInt());
					}
				}
			}
			return ret;
		}
		catch (Json::Exception&)
		{
		}
	}
	return{};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Audio::GetElements(const size_t& MemoryId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_AUDIO_GET, BuildElement(MemoryId, "AudioId").c_str(), &data);
	std::vector<std::pair<std::wstring, std::wstring>> ret{};
	if (data)
	{
		try
		{
			Json::Reader reader;
			Json::Value root;
			if (reader.parse(utf8_encode(data), root))
			{
				if (root.isArray())
				{
					ret = ParserResponse(root);
				}
			}
		}
		catch (Json::Exception& e)
		{
		}
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<size_t> Hardware::HardwareWrapper::Bios::InitializeBios()
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_BIOS_INIT, nullptr, &data);
	if (data)
	{
		auto Str = utf8_encode(data);
		SysFreeString(data);
		try
		{
			Json::Reader reader;
			Json::Value root;
			std::vector<size_t> ret;
			if (reader.parse(Str, root))
			{
				for (const auto& MemoryEle : root["BiosId"])
				{
					if (MemoryEle.isIntegral())
					{
						ret.emplace_back(MemoryEle.asUInt());
					}
				}
			}
			return ret;
		}
		catch (Json::Exception&)
		{
		}
	}
	return{};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::Bios::GetElements(const size_t& MemoryId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_BIOS_GET, BuildElement(MemoryId, "BiosId").c_str(), &data);
	std::vector<std::pair<std::wstring, std::wstring>> ret{};
	if (data)
	{
		try
		{
			Json::Reader reader;
			Json::Value root;
			if (reader.parse(utf8_encode(data), root))
			{
				if (root["Bios"].isArray())
				{
					ret = ParserResponse(root["Bios"]);
				}
			}
		}
		catch (Json::Exception& e)
		{
		}
		SysFreeString(data);
		return ret;
	}
	return {};
}

std::vector<size_t> Hardware::HardwareWrapper::WinBio::InitializeWinBio()
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_WINBIO_INIT, nullptr, &data);
	if (data)
	{
		auto Str = utf8_encode(data);
		SysFreeString(data);
		try
		{
			Json::Reader reader;
			Json::Value root;
			std::vector<size_t> ret;
			if (reader.parse(Str, root))
			{
				for (const auto& MemoryEle : root["WinBioIds"])
				{
					if (MemoryEle.isIntegral())
					{
						ret.emplace_back(MemoryEle.asUInt());
					}
				}
			}
			return ret;
		}
		catch (Json::Exception&)
		{
		}
	}
	return{};
}

std::vector<std::pair<std::wstring, std::wstring>> Hardware::HardwareWrapper::WinBio::GetElements(const size_t& WinBioId)
{
	BSTR data{};
	PcmHardwareAction(PCM_HARDWARE_ACTION_WINBIO_GET, BuildElement(WinBioId, "WinBioId").c_str(), &data);
	if (data)
	{
		auto ret = ParserResponse(utf8_encode(data));
		SysFreeString(data);
		return ret;
	}
	return {};
}