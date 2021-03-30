#include "stdafx.h"
#include <regex>
#include "GenericMemory.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

namespace
{
	constexpr uint32_t InvalidNumberOfMemory = -1;
}

Hardware::Memory::GenericMemory::GenericMemory()
{
	auto Type16Info = Smbios::GetType(Smbios::SmbiosHeaderType::Type16());
	if (!Type16Info.empty())
	{
		try
		{
			MaxNumberOfMemory = Type16Info[0].NumOfMemoryDevices();
		}
		catch (const std::exception&)
		{
			MaxNumberOfMemory = InvalidNumberOfMemory;
		}
	}
	auto Type17Info = Smbios::GetType(Smbios::SmbiosHeaderType::Type17());
	try
	{
		for (const auto& Info : Type17Info)
		{
			if (Info.Size() != 0)
			{
				auto DeviceLocator = Info.DeviceLocator();
				if (std::regex_match(DeviceLocator, std::regex(R"(^Channel[A-Z]-DIMM[0-9]$)", std::regex_constants::icase)))
				{
					char ChannelNumber;
					int DIMMNumber;
					sscanf_s(DeviceLocator.c_str(),
						"Channel%c-DIMM%d",
						&ChannelNumber, sizeof(char), &DIMMNumber);

					if (ChannelNumber >= 'A' && ChannelNumber <= 'Z')
					{
						auto DIMMId = (ChannelNumber - 'A') * 2 + DIMMNumber;
						AvailableMemoryIds.emplace(DIMMId);
						GetSPDInformationFromSMBIOS(Info, DIMMId);
					}
					else
					{
						spdlog::error("SMBIOS unknown Channel number:{}", ChannelNumber);
					}
				}
				else
				{
					spdlog::error("SMBIOS memory slot number has special rules: {}", DeviceLocator);
				}
			}
		}
	}
	catch (const std::exception&)
	{
		spdlog::error("SMBIOS does not has any available memory");
	}
}

Hardware::Data::ErrorType Hardware::Memory::GenericMemory::BuildInitializeJson(std::string& response)
{
	Json::Value root;

	for (const auto& DIMMId : AvailableMemoryIds)
	{
		root["MemoryIds"].append(uint32_t(DIMMId));
	}

	if (root.isNull())
	{
		response = {};
		return Data::ErrorType::DATAEMPTY;
	}

	response = Json::FastWriter().write(root);
	return Data::ErrorType::SUCCESS;
}

Hardware::Data::ErrorType Hardware::Memory::GenericMemory::ParserDimmJson(const Json::Value& root, USHORT& DimmIds)
{
	if (root["MemoryId"].isUInt())
	{
		DimmIds = root["MemoryId"].asUInt();
		return Data::ErrorType::SUCCESS;
	}
	return Data::ErrorType::UNKNOWNJSON;
}

Hardware::Data::ErrorType Hardware::Memory::GenericMemory::BuildDIMMJson(const USHORT& DIMMIds, std::string& response)
{
	auto Itr = MemoryInformation.find(DIMMIds);
	if (Itr != MemoryInformation.end())
	{
		response = BuildMemoryCommonInformationToJson(Itr->second);
	}

	if (response.empty())
	{
		return Data::ErrorType::DATAEMPTY;
	}
	return Data::ErrorType::SUCCESS;
}

std::string Hardware::Memory::GenericMemory::BuildMemoryCommonInformationToJson(const MemoryCommonInformation& Info)
{
	Json::Value root;

	{
		if (!Info.PartNumber.empty())
		{
			Json::Value TempValue;
			TempValue["PartNumber"] = Info.PartNumber;
			root.append(TempValue);
		}

		if (!Info.ModuleType.empty())
		{
			Json::Value TempValue;
			TempValue["ModuleType"] = Info.ModuleType;
			root.append(TempValue);
		}

		if (!Info.MaxBandWidth.empty())
		{
			Json::Value TempValue;
			TempValue["MaxBandWidth"] = Info.MaxBandWidth;
			root.append(TempValue);
		}

		if (!Info.DataBits.empty())
		{
			Json::Value TempValue;
			TempValue["DataBits"] = Info.DataBits;
			root.append(TempValue);
		}

		if (!Info.ModuleManufacturer.empty())
		{
			Json::Value TempValue;
			TempValue["ModuleManufacturer"] = Info.ModuleManufacturer;
			root.append(TempValue);
		}

		if (!Info.DRAMManufacturer.empty())
		{
			Json::Value TempValue;
			TempValue["DRAMManufacturer"] = Info.DRAMManufacturer;
			root.append(TempValue);
		}

		if (!Info.SerialNumber.empty())
		{
			Json::Value TempValue;
			TempValue["SerialNumber"] = Info.SerialNumber;
			root.append(TempValue);
		}

		if (!Info.SPDExt.empty())
		{
			Json::Value TempValue;
			TempValue["SPDExt"] = Info.SPDExt;
			root.append(TempValue);
		}

		if (!Info.ProductDate.empty())
		{
			Json::Value TempValue;
			TempValue["ProductDate"] = Info.ProductDate;
			root.append(TempValue);
		}

		if (abs(Info.ModuleSize) >= 1e-6)
		{
			Json::Value TempValue;
			TempValue["ModuleSize"] = Utils::to_string_with_precision(Info.ModuleSize) + " GB";
			root.append(TempValue);
		}

		if (!Info.Ranks_Banks.empty())
		{
			Json::Value TempValue;
			TempValue["Ranks_Banks"] = Info.Ranks_Banks;
			root.append(TempValue);
		}
	}

	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

std::add_const_t<decltype(Hardware::Memory::GenericMemory::MemoryInformation)>& Hardware::Memory::GenericMemory::GetMemoryInformation() const
{
	return MemoryInformation;
}

Hardware::Data::ErrorType Hardware::Memory::GenericMemory::GetSPDInformationFromSMBIOS(const Smbios::Type17& SMBIOSMemoryInfo, const USHORT& DIMMId)
{
	auto itr = MemoryInformation.find(DIMMId);
	if (itr == MemoryInformation.end())
	{
		MemoryCommonInformation SPDInformation;
		auto ret = GetSPDInformationFromSMBIOS(SPDInformation, SMBIOSMemoryInfo);
		MemoryInformation.emplace(DIMMId, SPDInformation);
		return ret;
	}
	else
	{
		return GetSPDInformationFromSMBIOS(itr->second, SMBIOSMemoryInfo);
	}
}

Hardware::Data::ErrorType Hardware::Memory::GenericMemory::GetSPDInformationFromSMBIOS(MemoryCommonInformation& MemoryInfo, const Smbios::Type17& SMBIOSMemoryInfo)
{
	try
	{
		MemoryInfo.ModuleSize = SMBIOSMemoryInfo.Size() / 1024.0; // MB To GB
	}
	catch (const std::exception&)
	{
		spdlog::error("SMBIOS Type17 not support Size");
	}

	try
	{
		MemoryInfo.DataBits = std::to_string(SMBIOSMemoryInfo.Datawidth());
	}
	catch (const std::exception&)
	{
		spdlog::error("SMBIOS Type17 not support Datawidth");
	}

	try
	{
		MemoryInfo.SerialNumber = SMBIOSMemoryInfo.SerialNumber();
	}
	catch (const std::exception&)
	{
		spdlog::error("SMBIOS Type17 not support SerialNumber");
	}

	try
	{
		MemoryInfo.PartNumber = SMBIOSMemoryInfo.PartNumber();
	}
	catch (const std::exception&)
	{
		spdlog::error("SMBIOS Type17 not support PartNumber");
	}

	try
	{
		MemoryInfo.ModuleManufacturer = SMBIOSMemoryInfo.Manufacturer();
	}
	catch (const std::exception&)
	{
		spdlog::error("SMBIOS Type17 not support Manufacturer");
	}

	try
	{
		MemoryInfo.ModuleType = SMBIOSMemoryInfo.MemoryType();
	}
	catch (const std::exception&)
	{
		spdlog::error("SMBIOS Type17 not support MemoryType");
	}

	return Data::ErrorType::SUCCESS;
}