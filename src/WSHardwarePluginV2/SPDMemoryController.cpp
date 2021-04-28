#include "stdafx.h"
#include "SPDMemoryController.h"
#include "CommonSMBUS.h"
#include "IvyBridgeSMbus.h"

using namespace Hardware;

Data::ErrorType Hardware::Memory::SPDMemoryController::Initialize()
{
	bool Success = false;
	try
	{
		SMBUSController = std::make_unique<CommonSMBUS>();
		for (auto DIMMId = 0; DIMMId < 8; ++DIMMId)
		{
			if (SMBUSController->IsDeviceOnline(DIMMId))
			{
				AvailableMemoryIds.emplace(DIMMId);
				Success = true;
			}
		}
	}
	catch (std::exception&)
	{
		spdlog::error("Platform Not Support IO Smbus Controller");
	}
	if (!Success)
	{
		DWORD VendorID(0), DeviceID(0);
		SMBUSController.reset(new IvyBridgeSMbus());
		Utils::Ring0::SafePCIHandle PciPtr;
		//Must Be Success;
		PciPtr.ReadWORD(0, 0, 0, 0, VendorID);
		PciPtr.ReadWORD(0, 0, 0, 2, DeviceID);
		if ((VendorID == 0x8086 && DeviceID == 0x3C00) || (VendorID == 0x8086 && DeviceID == 0x0E00))
		{
			for (auto DIMMId = 0; DIMMId < 16; ++DIMMId)
			{
				if (SMBUSController->IsDeviceOnline(DIMMId))
				{
					AvailableMemoryIds.emplace(DIMMId);
					Success = true;
				}
			}
			if (!Success)
			{
				spdlog::error("Platform Not Support IvyBridge Smbus Controller");
			}
		}
	}
	if (Success)
	{
		GetSPDInformation();
	}
	else
	{
		SMBUSController = nullptr;
		spdlog::error("Platform Not Support Smbus Controller");
	}
	return Data::ErrorType::SUCCESS;
}

Data::ErrorType Hardware::Memory::SPDMemoryController::Update()
{
	if (!SMBUSController)
	{
		return Data::ErrorType::NOTSUPPORT;
	}
	return Data::ErrorType::NOTIMPLEMENTED;
}

Data::ErrorType Hardware::Memory::SPDMemoryController::SerializationSPD()
{
	return Data::ErrorType();
}

Data::ErrorType Hardware::Memory::SPDMemoryController::GetSPDInformation()
{
	if (!SMBUSController)
	{
		return Data::ErrorType::NOTSUPPORT;
	}

	for (const auto& DIMMIds : AvailableMemoryIds)
	{
		auto Itr = this->MemoryInformation.find(DIMMIds);
		if (Itr != MemoryInformation.end())
		{
			SMBUSController->ParserSPD(DIMMIds, Itr->second);
		}
		else
		{
			MemoryCommonInformation Info;
			SMBUSController->ParserSPD(DIMMIds, Info);
			MemoryInformation.emplace(DIMMIds, std::move(Info));
		}
	}

	return Data::ErrorType::SUCCESS;
}