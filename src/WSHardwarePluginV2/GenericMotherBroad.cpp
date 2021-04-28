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

std::string Hardware::MotherBroad::GenericMotherBroad::BuildInitJson()
{
	Json::Value root;
	root["BroadId"].append(1);
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}