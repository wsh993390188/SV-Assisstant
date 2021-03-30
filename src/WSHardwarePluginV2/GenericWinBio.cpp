#include "stdafx.h"
#include "GenericWinBio.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

using namespace Hardware;

Data::ErrorType Hardware::WinBio::GenericWinBio::GetElements(LPCSTR paramter, std::string& response)
{
	uint32_t WinBioId;
	if (ParserJson(paramter, WinBioId))
	{
		response = BuildElementJson(WinBioId);
		return Data::ErrorType::SUCCESS;
	}

	return Data::ErrorType::UNKNOWNJSON;
}

std::string Hardware::WinBio::GenericWinBio::BuildInitializeJson()
{
	Json::Value root;
	for (const auto& Info : WinBioInfos)
	{
		root["WinBioIds"].append(Info.first);
	}
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

std::string Hardware::WinBio::GenericWinBio::BuildElementJson(const uint32_t& WinBioId)
{
	Json::Value root;
	if (auto itr = WinBioInfos.find(WinBioId); itr != WinBioInfos.end())
	{
		{
			Json::Value temp;
			if (!itr->second.Type.empty())
			{
				temp["Type"] = itr->second.Type;
				root.append(std::move(temp));
			}
		}
		{
			Json::Value temp;
			if (!itr->second.Desc.empty())
			{
				temp["Description"] = itr->second.Desc;
				root.append(std::move(temp));
			}
		}

		{
			Json::Value temp;
			if (!itr->second.SerialNumber.empty())
			{
				temp["SerialNumber"] = itr->second.SerialNumber;
				root.append(std::move(temp));
			}
		}

		{
			Json::Value temp;
			if (!itr->second.Manufacturer.empty())
			{
				temp["Manufacturer"] = itr->second.Manufacturer;
				root.append(std::move(temp));
			}
		}

		{
			Json::Value temp;
			if (!itr->second.Model.empty())
			{
				temp["Model"] = itr->second.Model;
				root.append(std::move(temp));
			}
		}

		{
			Json::Value temp;
			if (!itr->second.Firmware.empty())
			{
				temp["Firmware"] = itr->second.Firmware;
				root.append(std::move(temp));
			}
		}
	}

	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

bool Hardware::WinBio::GenericWinBio::ParserJson(const std::string& JsonString, uint32_t& WinBioId)
{
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(JsonString, root))
		{
			if (root["WinBioId"].isIntegral())
			{
				WinBioId = root["WinBioId"].asUInt();
				return true;
			}
			else
			{
				spdlog::error("Json parser WinBioId is not Numberic");
			}
		}
		else
		{
			spdlog::error("parser json failed");
		}
	}
	catch (const std::exception& e)
	{
		spdlog::error("parser json failed {}", e.what());
	}
	return false;
}