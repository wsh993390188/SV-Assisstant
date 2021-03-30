#include "stdafx.h"
#include <WinBio.h>
#include "WinBioImpl.h"

bool Hardware::WinBio::WinBioImpl::EnumerateSensors(const uint32_t& Type)
{
	// Declare variables.
	HRESULT hr = S_OK;
	PWINBIO_UNIT_SCHEMA unitSchema = NULL;
	SIZE_T unitCount = 0;
	SIZE_T index = 0;
	std::string TypeString;

	// Enumerate the installed biometric units.
	hr = WinBioEnumBiometricUnits(
		Type,							// Type of biometric unit
		&unitSchema,                    // Array of unit schemas
		&unitCount);                   // Count of unit schemas

	if (FAILED(hr))
	{
		spdlog::error("WinBioEnumBiometricUnits failed. hr = 0x{:x}", hr);
		goto e_Exit;
	}

	if (Type == WINBIO_TYPE_FACIAL_FEATURES)
	{
		TypeString = "Face";
	}
	else if (Type == WINBIO_TYPE_FINGERPRINT)
	{
		TypeString = "Fingerprint";
	}

	// Display information for each installed biometric unit.
	for (index = 0; index < unitCount; ++index)
	{
		WinBioCommonInfo Temp{};
		Temp.Type = TypeString;
		Temp.Desc = Utils::wstringToUtf8(unitSchema[index].Description);
		Temp.Manufacturer = Utils::wstringToUtf8(unitSchema[index].Manufacturer);
		Temp.Model = Utils::wstringToUtf8(unitSchema[index].Model);
		Temp.SerialNumber = Utils::wstringToUtf8(unitSchema[index].SerialNumber);
		Temp.Firmware =
			std::to_string(unitSchema[index].FirmwareVersion.MajorVersion + 0) + "." +
			std::to_string(unitSchema[index].FirmwareVersion.MinorVersion);
		WinBioInfos.emplace(Number++, std::move(Temp));
	}

e_Exit:
	if (unitSchema != NULL)
	{
		WinBioFree(unitSchema);
		unitSchema = NULL;
	}
	return SUCCEEDED(hr);
}

Hardware::WinBio::WinBioImpl::WinBioImpl() : Number(0)
{
}

Hardware::Data::ErrorType Hardware::WinBio::WinBioImpl::Initialize(std::string& response)
{
	Number = 0;
	WinBioInfos.clear();

	EnumerateSensors(WINBIO_TYPE_FINGERPRINT);
	EnumerateSensors(WINBIO_TYPE_FACIAL_FEATURES);
	if (WinBioInfos.empty())
		return Data::ErrorType::DATAEMPTY;
	response = BuildInitializeJson();
	return Data::ErrorType::SUCCESS;
}

Hardware::Data::ErrorType Hardware::WinBio::WinBioImpl::Update(const std::string& Args, std::string& response)
{
	return Data::ErrorType::NOTSUPPORT;
}