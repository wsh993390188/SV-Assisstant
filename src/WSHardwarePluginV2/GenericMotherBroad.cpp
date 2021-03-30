#include "stdafx.h"
#include "StringHelper.hpp"
#include "GenericMotherBroad.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

Hardware::Data::ErrorType Hardware::MotherBroad::GenericMotherBroad::GetElements(LPCSTR paramter, std::string& response)
{
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(paramter, root))
	{
		Json::Value ret;
		if (root["BiosId"].isIntegral())
		{
			ret["Bios"] = BuildBiosToJson();
		}

		if (root["BroadId"].isIntegral())
		{
			ret["Broad"] = BuildBroadToJson();
		}
		if (ret.empty())
			return Data::ErrorType::DATAEMPTY;
		response = Json::FastWriter().write(ret);
		return Data::ErrorType::SUCCESS;
	}
	spdlog::error("parser Json error from motherbroad");
	return Data::ErrorType::UNKNOWNJSON;
}

Json::Value Hardware::MotherBroad::GenericMotherBroad::BuildBroadToJson()
{
	Json::Value root;
	if (!BroadInfo.Manufacturer.empty())
	{
		Json::Value TempValue;
		TempValue["Manufacturer"] = BroadInfo.Manufacturer;
		root.append(TempValue);
	}

	if (!BroadInfo.Model.empty())
	{
		Json::Value TempValue;
		TempValue["Model"] = BroadInfo.Model;
		root.append(TempValue);
	}

	if (!BroadInfo.SerialNumber.empty())
	{
		Json::Value TempValue;
		TempValue["SerialNumber"] = BroadInfo.SerialNumber;
		root.append(TempValue);
	}

	return root;
}

Json::Value Hardware::MotherBroad::GenericMotherBroad::BuildBiosToJson()
{
	Json::Value root;
	if (!BiosInfos.Brand.empty())
	{
		Json::Value TempValue;
		TempValue["Brand"] = BiosInfos.Brand;
		root.append(TempValue);
	}

	if (!BiosInfos.Version.empty())
	{
		Json::Value TempValue;
		TempValue["Version"] = BiosInfos.Version;
		root.append(TempValue);
	}

	if (!BiosInfos.DataTime.empty())
	{
		Json::Value TempValue;
		TempValue["DataTime"] = BiosInfos.DataTime;
		root.append(TempValue);
	}
	return root;
}

std::string Hardware::MotherBroad::GenericMotherBroad::BuildInitJson()
{
	// 这个暂时这样处理，便于以后增加多BIOS支持
	Json::Value root;
	root["BiosId"].append(1);
	root["BroadId"].append(1);
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}