#include "stdafx.h"
#include "GenericBios.h"
#include "WinReg/WinReg.hpp"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

Hardware::Data::ErrorType Hardware::BIOS::GenericBios::Initialize(std::string& response)
{
	auto BiosInfo = Smbios::GetType(Smbios::SmbiosHeaderType::Type0());
	try
	{
		BiosInfos.Brand = BiosInfo.VendorName();
		Utils::trim(BiosInfos.Brand);
	}
	catch (const std::exception& e)
	{
		spdlog::error("Bios cant get vender name, error: {}", e.what());
	}

	try
	{
		BiosInfos.Version = BiosInfo.BiosVersion();
		Utils::trim(BiosInfos.Version);
	}
	catch (const std::exception& e)
	{
		spdlog::error("Bios cant get version name, error: {}", e.what());
	}

	try
	{
		BiosInfos.DataTime = BiosInfo.BiosRelease();
		Utils::trim(BiosInfos.DataTime);
	}
	catch (const std::exception& e)
	{
		spdlog::error("Bios cant get bios release date, error {}", e.what());
	}

	BiosInfos.UEFI = IsUEFI();
	BiosInfos.SecureBoot = SecureBootEnabled();

	response = BuildInitJson();
	if (response.empty())
		return Data::ErrorType::DATAEMPTY;
	return Data::ErrorType::SUCCESS;
}

Hardware::Data::ErrorType Hardware::BIOS::GenericBios::Update(const std::string& Args, std::string& response)
{
	return Data::ErrorType::NOTSUPPORT;
}

bool Hardware::BIOS::GenericBios::IsUEFI() const
{
	GetFirmwareEnvironmentVariable(L"", L"{00000000-0000-0000-0000-000000000000}", NULL, 0);
	if (GetLastError() == ERROR_INVALID_FUNCTION) {
		return false;
	}
	else {
		return true;
	}
}

bool Hardware::BIOS::GenericBios::SecureBootEnabled()
{
	bool ret = false;
	try
	{
		winreg::RegKey Key(HKEY_LOCAL_MACHINE, LR"(SYSTEM\CurrentControlSet\Control\SecureBoot\State)");
		ret = Key.GetDwordValue(L"UEFISecureBootEnabled") == 1;
	}
	catch (const std::exception&)
	{
	}

	return ret;
}

Hardware::Data::ErrorType Hardware::BIOS::GenericBios::GetElements(LPCSTR paramter, std::string& response)
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
		if (ret.empty())
			return Data::ErrorType::DATAEMPTY;
		response = Json::FastWriter().write(ret);
		return Data::ErrorType::SUCCESS;
	}
	spdlog::error("parser Json error from Bios");
	return Data::ErrorType::UNKNOWNJSON;
}

Json::Value Hardware::BIOS::GenericBios::BuildBiosToJson()
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

	{
		Json::Value TempValue;
		TempValue["BIOS Mode"] = BiosInfos.UEFI ? "UEFI" : "Legacy";
		root.append(TempValue);
	}

	{
		Json::Value TempValue;
		TempValue["Secure Boot"] = BiosInfos.SecureBoot ? "Enabled" : "Disabled";
		root.append(TempValue);
	}
	return root;
}

std::string Hardware::BIOS::GenericBios::BuildInitJson()
{
	// 这个暂时这样处理，便于以后增加多BIOS支持
	Json::Value root;
	root["BiosId"].append(1);
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}