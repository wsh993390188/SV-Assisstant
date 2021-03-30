#include "stdafx.h"
#include "GenericNIC.h"
#include "NetMonitor.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

Hardware::Data::ErrorType Hardware::NIC::GenericNIC::GetElements(LPCSTR paramter, std::string& response)
{
	uint32_t NICId;
	if (ParserJson(paramter, NICId))
	{
		response = BuildElementJson(NICId);
		return Data::ErrorType::SUCCESS;
	}

	return Data::ErrorType::UNKNOWNJSON;
}

std::string Hardware::NIC::GenericNIC::BuildInitializeJson()
{
	Json::Value root;
	for (const auto& NICInfo : NICInfos)
	{
		root["NICIds"].append(NICInfo.first);
	}
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

std::string Hardware::NIC::GenericNIC::BuildElementJson(const uint32_t& NICId)
{
	auto itr = NICInfos.find(NICId);
	if (itr == NICInfos.end())
		return {};
	Json::Value root;
	{
		Json::Value temp;
		if (!itr->second.Description.empty())
		{
			temp["Device Name"] = itr->second.Description;
			root.append(std::move(temp));
		}
	}

	{
		Json::Value temp;
		if (!itr->second.ConnectionName.empty())
		{
			temp["Connection Name"] = itr->second.ConnectionName;
			root.append(std::move(temp));
		}
	}

	{
		Json::Value temp;
		if (!itr->second.MacAddress.empty())
		{
			temp["MAC Addrss"] = itr->second.MacAddress;
			root.append(std::move(temp));
		}
	}

	{
		Json::Value temp;
		if (!itr->second.InterfaceType.empty())
		{
			temp["Interface Type"] = itr->second.InterfaceType;
			root.append(std::move(temp));
		}
	}

	if (itr->second.ConnectionSpeed != static_cast<decltype(itr->second.ConnectionSpeed)>(-1))
	{
		Json::Value temp;
		temp["Connection Speed"] = Utils::NetSpeedToStringWithUnit(itr->second.ConnectionSpeed);
		root.append(std::move(temp));
	}

	if (itr->second.MTU != static_cast<decltype(itr->second.MTU)>(-1))
	{
		Json::Value temp;
		temp["MTU"] = std::to_string(itr->second.MTU) + " bytes";
		root.append(std::move(temp));
	}

	for (const auto& IPAddress : itr->second.UniAddress)
	{
		if (IPAddress.Type == IPAddress::IPAddressType::IPV4)
		{
			Json::Value temp;
			temp["IPAddress"] = IPAddress.Address;
			root.append(std::move(temp));
		}
		else if (IPAddress.Type == IPAddress::IPAddressType::IPV6)
		{
			Json::Value temp;
			temp["IPV6Address"] = IPAddress.Address;
			root.append(std::move(temp));
		}
	}

	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

std::string Hardware::NIC::GenericNIC::BuildElementUpdateJson(const NetStruct& Info)
{
	Json::Value root;
	{
		Json::Value temp;
		temp["Bytes Received"] = Info.InOctets;
		root.append(std::move(temp));
	}

	{
		Json::Value temp;
		temp["Bytes Sent"] = Info.OutOctets;
		root.append(std::move(temp));
	}

	if (Info.IsWifi)
	{
		Json::Value temp;
		temp["Wifi Signal Quality"] = NetworkMonitor::Instance().QueryWiFiSignalQuality(Info.IfGuid);
		root.append(std::move(temp));
	}
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

bool Hardware::NIC::GenericNIC::ParserJson(const std::string& JsonString, uint32_t& NICId)
{
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(JsonString, root))
		{
			if (root["NICId"].isIntegral())
			{
				NICId = root["NICId"].asUInt();
				return true;
			}
			else
			{
				spdlog::error("Json NICIdis not numberic");
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