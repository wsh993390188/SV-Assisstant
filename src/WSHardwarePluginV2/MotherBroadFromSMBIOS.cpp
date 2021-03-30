#include "stdafx.h"
#include "MotherBroadFromSMBIOS.h"

using namespace Hardware;

Data::ErrorType Hardware::MotherBroad::MotherBroadFromSMBIOS::Initialize(std::string& response)
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

	auto SmbiosBroadInfo = Smbios::GetType(Smbios::SmbiosHeaderType::Type2());
	try
	{
		BroadInfo.Manufacturer = SmbiosBroadInfo.Manufacturer();
		Utils::trim(BroadInfo.Manufacturer);
	}
	catch (const std::exception& e)
	{
		spdlog::error("Bios Type2 cant get Manufacturer, error: {}", e.what());
	}

	try
	{
		BroadInfo.Model = SmbiosBroadInfo.Product();
		Utils::trim(BroadInfo.Model);
	}
	catch (const std::exception& e)
	{
		spdlog::error("Bios Type2 cant get Product Name, error: {}", e.what());
	}

	try
	{
		BroadInfo.SerialNumber = SmbiosBroadInfo.SerialNumber();
		Utils::trim(BroadInfo.SerialNumber);
	}
	catch (const std::exception& e)
	{
		spdlog::error("Bios Type2 cant get Serial Number, error: {}", e.what());
	}

	response = BuildInitJson();
	if (response.empty())
		return Data::ErrorType::DATAEMPTY;
	return Data::ErrorType::SUCCESS;
}

Data::ErrorType Hardware::MotherBroad::MotherBroadFromSMBIOS::Update(const std::string&, std::string&)
{
	return Data::ErrorType::NOTSUPPORT;
}