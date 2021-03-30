#include "stdafx.h"
#include <initguid.h>
#include <setupapi.h>
#include <winioctl.h>
#include <Batclass.h>
#include "BatteryFromSys.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

using namespace Hardware;

Hardware::Battery::BatteryFromSys::BatteryFromSys()
{
	GetDevicePath();
}

Data::ErrorType Hardware::Battery::BatteryFromSys::Initialize(std::string& response)
{
	if (DevicePath.empty())
	{
		return Data::ErrorType::NOTSUPPORT;
	}

	response = BuildInitializeJson();
	return Data::ErrorType::SUCCESS;
}

Data::ErrorType Hardware::Battery::BatteryFromSys::Update(const std::string& Args, std::string& response)
{
	uint32_t BatteryId;
	if (ParserJson(Args, BatteryId))
	{
		auto BatteryInst = DevicePath.find(BatteryId);
		if (BatteryInst != DevicePath.end())
		{
			BatteryCommonStruct BatteryInfo{};
			if (BatteryInst->second->Update(BatteryId, BatteryInfo))
			{
				response = BatteryInst->second->BuildBatteryElementToJsonString(BatteryInfo);
				return Data::ErrorType::SUCCESS;
			}
		}

		return Data::ErrorType::NOTSUPPORT;
	}
	return Data::ErrorType::UNKNOWNJSON;
}

Data::ErrorType Hardware::Battery::BatteryFromSys::GetElements(LPCSTR paramter, std::string& response)
{
	return this->Update(paramter, response);
}

bool Hardware::Battery::BatteryFromSys::GetDevicePath()
{
	HDEVINFO hBatDevInfo =
		SetupDiGetClassDevs(&GUID_DEVICE_BATTERY, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

	int nIndex = 0;
	for (;;) {
		BOOL bEnumDev;
		SP_DEVICE_INTERFACE_DATA devInterfaceData;
		devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		bEnumDev = SetupDiEnumDeviceInterfaces(hBatDevInfo, NULL, &GUID_DEVICE_BATTERY, nIndex, &devInterfaceData);
		// FIXED A BUG 06/07/2004
		// it was trying to get device information even when SetupDi... fails.
		if (bEnumDev)
		{
			// add battery when successful
			DWORD reqSize;
			SetupDiGetDeviceInterfaceDetail(hBatDevInfo, &devInterfaceData, NULL, 0, &reqSize, NULL);
			auto DevData = std::make_unique<uint8_t[]>(reqSize + 1);
			PSP_DEVICE_INTERFACE_DETAIL_DATA pDevDetailData;
			pDevDetailData = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(DevData.get());
			pDevDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			SetupDiGetDeviceInterfaceDetail(hBatDevInfo, &devInterfaceData, pDevDetailData, reqSize, NULL, NULL);
			try
			{
				DevicePath.emplace(nIndex, BatteryElementFactory::Instance().CreateBattery(pDevDetailData->DevicePath));
			}
			catch (const std::exception&)
			{
				continue;
			}
		}
		else
		{
			if (GetLastError() == ERROR_NO_MORE_ITEMS)
				break;
		}
		++nIndex;
	}
	SetupDiDestroyDeviceInfoList(hBatDevInfo);
	return !DevicePath.empty();
}

std::string Hardware::Battery::BatteryFromSys::BuildInitializeJson()
{
	Json::Value root;
	for (const auto& BatteryDev : DevicePath)
	{
		root["BatteryIds"].append(BatteryDev.first);
	}
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

bool Hardware::Battery::BatteryFromSys::ParserJson(const std::string& JsonString, uint32_t& BatteryId)
{
	try
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(JsonString, root))
		{
			if (root["BatteryId"].isIntegral())
			{
				BatteryId = root["BatteryId"].asUInt();
				return true;
			}
			else
			{
				spdlog::error("Json BatteryId is not numberic");
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