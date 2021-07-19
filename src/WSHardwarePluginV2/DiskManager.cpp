#include "stdafx.h"
#include "DiskManager.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

Hardware::Data::ErrorType Hardware::DiskManager::Init(LPCSTR paramter, std::string& response)
{
	m_Ata.Init(TRUE, FALSE, nullptr, FALSE, FALSE, FALSE);
	return BuildInitJson(response);
}

Hardware::Data::ErrorType Hardware::DiskManager::Update(LPCSTR paramter, std::string& response)
{
	int CurrentId = -1;
	std::vector<std::string> Elements{};
	auto res = PaserUpdateJson(paramter, CurrentId, Elements);
	if (res != Data::ErrorType::SUCCESS)
	{
		return res;
	}
	m_Ata.UpdateSmartInfo(CurrentId);
	res = BuildJson(response, CurrentId, Elements);
	return res;
}

Hardware::Data::ErrorType Hardware::DiskManager::GetElements(LPCSTR paramter, std::string& response)
{
	std::vector<std::string> Elements{};
	int CurrentId = -1;
	auto res = PaserUpdateJson(paramter, CurrentId, Elements);
	if (res != Data::ErrorType::SUCCESS)
	{
		return res;
	}

	res = BuildJson(response, CurrentId, Elements);
	return res;
}

Hardware::Data::ErrorType Hardware::DiskManager::PaserUpdateJson(LPCSTR paramter, int& CurrentId, std::vector<std::string>& Elements)
{
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (!paramter || strlen(paramter) == 0)
			return Data::ErrorType::UNKNOWNJSON;
		if (reader.parse(paramter, root))
		{
			CurrentId = root["Id"].asInt();
			for (const auto& Element : root["Elements"])
			{
				Elements.push_back(Element.asString());
			}
			return Data::ErrorType::SUCCESS;
		}
	}
	catch (const Json::Exception& exc)
	{
		spdlog::debug(exc.what());
	}
	catch (const std::exception& e)
	{
		spdlog::debug(e.what());
	}
	catch (...)
	{
		spdlog::error("UnExpect Bug\n");
	}

	return Data::ErrorType::UNKNOWNJSON;
}

Hardware::Data::ErrorType Hardware::DiskManager::BuildJson(std::string& response, const int CurrentId, const std::vector<std::string>& Elements)
{
	try
	{
		Json::Value root;
		for (const auto& Element : Elements)
		{
			Json::Value TempValue;
			if (Element.compare("Temperature") == 0)
			{
				TempValue["Temperature"] = std::to_string(m_Ata.vars.at(CurrentId).Temperature) + " Degree";
			}
			else if (Element.compare("Interface") == 0)
			{
				TempValue["Interface"] = CT2A(m_Ata.vars.at(CurrentId).Interface, CP_UTF8).m_psz;
			}
			else if (Element.compare("Firmware") == 0)
			{
				TempValue["Firmware"] = CT2A(m_Ata.vars.at(CurrentId).FirmwareRev, CP_UTF8).m_psz;
			}
			else if (Element.compare("SerialNumber") == 0)
			{
				TempValue["SerialNumber"] = CT2A(m_Ata.vars.at(CurrentId).SerialNumber, CP_UTF8).m_psz;
			}
			else if (Element.compare("Model") == 0)
			{
				TempValue["Model"] = CT2A(m_Ata.vars.at(CurrentId).Model, CP_UTF8).m_psz;
			}
			else if (Element.compare("PowerOnHours") == 0)
			{
				if (m_Ata.vars.at(CurrentId).MeasuredPowerOnHours > 0)
					TempValue["PowerOnHours"] = m_Ata.vars.at(CurrentId).MeasuredPowerOnHours;
				else
					TempValue["PowerOnHours"] = m_Ata.vars.at(CurrentId).DetectedPowerOnHours;
			}
			else if (Element.compare("PowerOnCount") == 0)
			{
				TempValue["PowerOnCount"] = Json::UInt(m_Ata.vars.at(CurrentId).PowerOnCount);
			}
			else if (Element.compare("TotalDiskSize") == 0)
			{
				TempValue["TotalDiskSize"] = DiskSizeToString(m_Ata.vars.at(CurrentId).TotalDiskSize);
			}
			else if (Element.compare("DiskLetter") == 0)
			{
				TempValue["DiskLetter"] = CT2A(m_Ata.vars.at(CurrentId).DriveMap, CP_UTF8).m_psz;
			}
			else if (Element.compare("RotationRate") == 0)
			{
				TempValue["RotationRate"] = Json::UInt(m_Ata.vars.at(CurrentId).NominalMediaRotationRate);
			}
			else if (Element.compare("TotalHostWrite") == 0)
			{
				TempValue["TotalHostWrite"] = Json::UInt(m_Ata.vars.at(CurrentId).HostWrites);
			}
			else if (Element.compare("TotalHostRead") == 0)
			{
				TempValue["TotalHostRead"] = Json::UInt(m_Ata.vars.at(CurrentId).HostReads);
			}
			else if (Element.compare("DiskStatus") == 0)
			{
				TempValue["DiskStatus"] = m_Ata.GetDiskStatus(m_Ata.vars.at(CurrentId).DiskStatus);
			}
			else if (Element.compare("TransferMode") == 0)
			{
				TempValue["TransferMode"] = CT2A((m_Ata.vars.at(CurrentId).CurrentTransferMode + _T(" | ") + m_Ata.vars.at(CurrentId).MaxTransferMode), CP_UTF8).m_psz;
			}
			else if (Element.compare("Feature") == 0)
			{
				std::string Feature;
				if (m_Ata.vars.at(CurrentId).IsSmartSupported)
				{
					Feature += "S.M.A.R.T., ";
				}

				if (m_Ata.vars.at(CurrentId).IsApmSupported)
				{
					Feature += "APM, ";
				}

				if (m_Ata.vars.at(CurrentId).IsAamSupported)
				{
					Feature += "AAM, ";
				}

				if (m_Ata.vars.at(CurrentId).IsLba48Supported)
				{
					Feature += "48bit LBA, ";
				}

				if (m_Ata.vars.at(CurrentId).IsNcqSupported)
				{
					Feature += "NCQ, ";
				}

				if (m_Ata.vars.at(CurrentId).IsTrimSupported)
				{
					Feature += "TRIM, ";
				}
				if (m_Ata.vars.at(CurrentId).IsDeviceSleepSupported)
				{
					Feature += "DevSleep, ";
				}
				Utils::trim(Feature);
				TempValue["Feature"] = Feature;
			}
			else
			{
				continue;
			}
			root.append(TempValue);
		}

		response = Json::FastWriter().write(root);
		return Data::ErrorType::SUCCESS;
	}
	catch (const Json::Exception& e)
	{
		spdlog::debug(e.what());
	}
	catch (const std::exception& e)
	{
		spdlog::debug(e.what());
		return Data::ErrorType::OVERBOUND;
	}
	catch (...)
	{
		spdlog::error("UnExpect Bug\n");
	}
	return Data::ErrorType::UNKNOWNJSON;
}

Hardware::Data::ErrorType Hardware::DiskManager::BuildInitJson(std::string& response)
{
	if (m_Ata.vars.empty())
		return Data::ErrorType::SUCCESS;

	Json::Value root;
	int id = 0;
	for (const auto& var : m_Ata.vars)
	{
		Json::Value temp;
		temp["Id"] = id++;
		BuildStaticJson(temp["Elements"], var);
		BuildDynamicJson(temp["UpdateElements"], var);
		root.append(temp);
	}

	if (!root.isNull())
		response = Json::FastWriter().write(root);

	return Data::ErrorType::SUCCESS;
}

void Hardware::DiskManager::BuildStaticJson(Json::Value& Element, const CAtaSmart::ATA_SMART_INFO& var)
{
	if (!var.Interface.IsEmpty())
		Element.append("Interface");
	if (!var.FirmwareRev.IsEmpty())
		Element.append("Firmware");
	if (!var.SerialNumber.IsEmpty())
		Element.append("SerialNumber");
	if (!var.Model.IsEmpty())
		Element.append("Model");
	if (var.TotalDiskSize > 0)
		Element.append("TotalDiskSize");
	if (!var.DriveMap.IsEmpty())
		Element.append("DiskLetter");
	if ((var.NominalMediaRotationRate) > 0 && (var.NominalMediaRotationRate != 1))
		Element.append("RotationRate");
	if (!var.CurrentTransferMode.IsEmpty() || !var.MaxTransferMode.IsEmpty())
		Element.append("TransferMode");
	Element.append("Feature");
}

void Hardware::DiskManager::BuildDynamicJson(Json::Value& Element, const CAtaSmart::ATA_SMART_INFO& var)
{
	if (var.Temperature > -300)
	{
		Element.append("Temperature");
	}
	if (var.HostReads >= 0)
		Element.append("TotalHostRead");
	if (var.HostWrites >= 0)
		Element.append("TotalHostWrite");
	if (var.PowerOnCount > 0)
		Element.append("PowerOnCount");
	if (var.MeasuredPowerOnHours > 0)
		Element.append("PowerOnHours");
	else if (var.DetectedPowerOnHours >= 0)
		Element.append("PowerOnHours");
	Element.append("DiskStatus");
}

std::string Hardware::DiskManager::DiskSizeToString(const uint64_t& DiskSize)
{
	if (DiskSize < 1024)
		return std::to_string(DiskSize) + " MB";
	else
		return Utils::to_string_with_precision(DiskSize / 1024.0) + " GB";
}