#include "stdafx.h"
#include <initguid.h>
#include <winioctl.h>
#include "BatteryElementCommon.h"

Hardware::Battery::BatteryInforamtion::BatteryInforamtion(const std::wstring& DevicePath)
{
	HANDLE handle = ::CreateFile(DevicePath.c_str(), FILE_ALL_ACCESS, 0, NULL
		, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	BatteryHandle = Utils::make_HANDLE_unique(handle);
	if (!BatteryHandle)
	{
		throw std::exception("Not Open Battery Device");
	}

	if (GetNewTag() == BATTERY_TAG_INVALID)
	{
		throw std::exception("Cant Get Battery tag");
	}
}

bool Hardware::Battery::BatteryInforamtion::Update(const uint32_t&, BatteryCommonStruct& BatteryInfo)
{
	if (GetNewTag() == BATTERY_TAG_INVALID)
		return false;

	BATTERY_QUERY_INFORMATION QueryBatInfo{};
	QueryBatInfo.AtRate = 0;
	QueryBatInfo.BatteryTag = CurrentBatTag;
	{
		QueryBatInfo.InformationLevel = BatteryManufactureName;
		QueryString(QueryBatInfo, BatteryInfo.Manufacture);
	}

	{
		QueryBatInfo.InformationLevel = BatterySerialNumber;
		QueryString(QueryBatInfo, BatteryInfo.SerialNumber);
	}

	{
		QueryBatInfo.InformationLevel = BatteryUniqueID;
		QueryString(QueryBatInfo, BatteryInfo.UniqueID);
	}

	{
		BATTERY_MANUFACTURE_DATE ManufactureDate{};
		if (QueryBatteryManufactureDate(ManufactureDate))
		{
			BatteryInfo.ManufactureDate = std::to_string(ManufactureDate.Day + 0) + R"(\)" + std::to_string(ManufactureDate.Month + 0) + R"(\)" + std::to_string(ManufactureDate.Year + 0);
		}
	}

	QueryBatteryTemperature(BatteryInfo.Temperature);
	QuertBatteryCurrentPercent(BatteryInfo.Percentage);

	{
		BATTERY_INFORMATION BatteryInformation{};
		if (QueryBatteryInfo(BatteryInformation))
		{
			BatteryInfo.Chemistry = BatteryChemistryToString(std::string(reinterpret_cast<const char*>(BatteryInformation.Chemistry), 4));
			BatteryInfo.Technology = BatteryTechnologyToString(BatteryInformation.Technology);
			BatteryInfo.FullChargedCapacity = BatteryCapacityToObject(BatteryInformation.FullChargedCapacity, BatteryInformation.Capabilities);
			BatteryInfo.DesignCapacity = BatteryCapacityToObject(BatteryInformation.DesignedCapacity, BatteryInformation.Capabilities);
			if (BatteryInformation.CycleCount != 0 && BatteryInformation.CycleCount != BATTERY_UNKNOWN_CURRENT)
				BatteryInfo.CycleCount = std::to_string(BatteryInformation.CycleCount);
			BatteryInfo.BatteryDepletion = BatteryCalcDepletion(BatteryInformation.FullChargedCapacity, BatteryInformation.DesignedCapacity);
			// 			BatteryInfo.DefaultAlert1 = BatteryCapacityToObject(BatteryInformation.DefaultAlert1, 0);
			// 			BatteryInfo.DefaultAlert2 = BatteryCapacityToObject(BatteryInformation.DefaultAlert2, 0);
		}

		BATTERY_STATUS BatteryStatus{};
		if (QueryBatteryStatus(BatteryStatus))
		{
			BatteryInfo.PowerState = BatteryPowerStatusToString(BatteryStatus);
			BatteryInfo.Voltage = BatteryVoltageToString(BatteryStatus);
			BatteryInfo.Capacity = BatteryCapacityToObject(BatteryStatus.Capacity, BatteryInformation.Capabilities);
			BatteryInfo.Rate = BatteryRateToObject(BatteryStatus.Rate, BatteryInformation.Capabilities);
		}
	}

	return true;
}

ULONG Hardware::Battery::BatteryInforamtion::GetNewTag()
{
	ULONG ulWait = 200;
	ULONG ulTag;
	DWORD dwBytesReturned;

	BOOL bResult = DeviceIoControl(BatteryHandle.get(), IOCTL_BATTERY_QUERY_TAG, &ulWait, sizeof(ULONG),
		&ulTag, sizeof(ULONG), &dwBytesReturned, NULL);

	if (bResult == FALSE)
	{
		CurrentBatTag = 0;
		return 0;
	}

	CurrentBatTag = ulTag;
	return ulTag;
}

bool Hardware::Battery::BatteryInforamtion::QueryString(BATTERY_QUERY_INFORMATION& batQueryInfo, std::string& csOutStr)
{
	wchar_t wszStrTemp[200]{};
	BOOL bSucceeded = FALSE;
	DWORD dwBytesReturned = 0;
	bSucceeded = DeviceIoControl(BatteryHandle.get(), IOCTL_BATTERY_QUERY_INFORMATION,
		&batQueryInfo, sizeof(batQueryInfo), wszStrTemp, _countof(wszStrTemp),
		&dwBytesReturned, NULL);
	if (bSucceeded)
	{
		csOutStr = Utils::wstringToUtf8(wszStrTemp);
	}
	else
	{
		Utils::ErrorCode(__FUNCTION__);
	}
	return bSucceeded == TRUE;
}

bool Hardware::Battery::BatteryInforamtion::QueryBatteryInfo(BATTERY_INFORMATION& BatteryInfo)
{
	BATTERY_QUERY_INFORMATION batteryQueryInfo;
	batteryQueryInfo.AtRate = 0;
	batteryQueryInfo.BatteryTag = CurrentBatTag;
	batteryQueryInfo.InformationLevel = BatteryInformation;
	DWORD BytesReturned;
	//取得信息
	auto result = ::DeviceIoControl(BatteryHandle.get(), IOCTL_BATTERY_QUERY_INFORMATION, &batteryQueryInfo
		, sizeof(batteryQueryInfo), &BatteryInfo, sizeof(BatteryInfo), &BytesReturned, NULL);
	if (!result)
	{
		Utils::ErrorCode(__FUNCTION__);
	}
	return result == TRUE;
}

bool Hardware::Battery::BatteryInforamtion::QueryBatteryManufactureDate(BATTERY_MANUFACTURE_DATE& BatteryDate)
{
	BATTERY_QUERY_INFORMATION batteryQueryInfo;
	batteryQueryInfo.AtRate = 0;
	batteryQueryInfo.BatteryTag = CurrentBatTag;
	batteryQueryInfo.InformationLevel = BatteryManufactureDate;
	DWORD BytesReturned;
	//取得信息
	auto result = ::DeviceIoControl(BatteryHandle.get(), IOCTL_BATTERY_QUERY_INFORMATION, &batteryQueryInfo
		, sizeof(batteryQueryInfo), &BatteryDate, sizeof(BatteryDate), &BytesReturned, NULL);
	if (result == FALSE)
	{
		Utils::ErrorCode(__FUNCTION__);
	}
	return result == TRUE;
}

bool Hardware::Battery::BatteryInforamtion::QueryBatteryStatus(BATTERY_STATUS& BatteryStatus)
{
	BATTERY_WAIT_STATUS bwt;
	bwt.BatteryTag = CurrentBatTag;
	bwt.HighCapacity = 0;
	bwt.LowCapacity = 0;
	bwt.Timeout = 500;
	bwt.PowerState = 0;
	DWORD BytesReturned;
	auto result = ::DeviceIoControl(BatteryHandle.get(), IOCTL_BATTERY_QUERY_STATUS, &bwt, sizeof(bwt)
		, &BatteryStatus, sizeof(BatteryStatus), &BytesReturned, NULL);
	if (result == FALSE)
	{
		Utils::ErrorCode(__FUNCTION__);
	}
	return result == TRUE;
}

bool Hardware::Battery::BatteryInforamtion::QueryBatteryTemperature(std::string& Temperature)
{
	BATTERY_QUERY_INFORMATION batteryQueryInfo;
	batteryQueryInfo.AtRate = 0;
	batteryQueryInfo.BatteryTag = CurrentBatTag;
	batteryQueryInfo.InformationLevel = BatteryTemperature;
	DWORD BytesReturned;
	ULONG TempTemperature = 0;
	//取得信息
	auto result = ::DeviceIoControl(BatteryHandle.get(), IOCTL_BATTERY_QUERY_INFORMATION, &batteryQueryInfo
		, sizeof(batteryQueryInfo), &TempTemperature, sizeof(TempTemperature), &BytesReturned, NULL);
	if (!result)
	{
		Utils::ErrorCode(__FUNCTION__);
	}
	else
	{
		if (TempTemperature > 0)
		{
			Temperature = std::to_string((TempTemperature / 10) - 273);
		}
		else
		{
			result = FALSE;
		}
	}
	return result == TRUE;
}

bool Hardware::Battery::BatteryInforamtion::QuertBatteryCurrentPercent(std::string& CurrentPercent)
{
	SYSTEM_POWER_STATUS lpPwrStatus;
	auto ret = ::GetSystemPowerStatus(&lpPwrStatus);
	if (ret == TRUE)
	{
		CurrentPercent = std::to_string(lpPwrStatus.BatteryLifePercent + 0) + "%";
	}
	return ret == TRUE;
}

std::string Hardware::Battery::BatteryInforamtion::BatteryPowerStatusToString(const BATTERY_STATUS& BatteryStatus)
{
	if (BATTERY_POWER_ON_LINE == BatteryStatus.PowerState)
		return "Power Online";
	else if (BATTERY_DISCHARGING == BatteryStatus.PowerState)
		return "Discharging";
	else if (BATTERY_CHARGING == BatteryStatus.PowerState)
		return "Charging";
	else if (BATTERY_CRITICAL == BatteryStatus.PowerState)
		return "Critical";
	else
		return "Unknown";
}

std::string Hardware::Battery::BatteryInforamtion::BatteryVoltageToString(const BATTERY_STATUS& BatteryStatus)
{
	if (BatteryStatus.Voltage == BATTERY_UNKNOWN_VOLTAGE)
		return {};
	return Utils::to_string_with_precision(BatteryStatus.Voltage / 1000.0) + "V";
}

Hardware::Battery::BatteryCapacity Hardware::Battery::BatteryInforamtion::BatteryCapacityToObject(const ULONG& CapacityValue, const ULONG& Unit)
{
	if (CapacityValue == BATTERY_UNKNOWN_CAPACITY || CapacityValue == 0)
		return {};
	Hardware::Battery::BatteryCapacity TempInst;
	if (Unit & BATTERY_CAPACITY_RELATIVE)
	{
		TempInst.Value = std::to_string(CapacityValue);
		TempInst.Unit = BatteryUnit::Percent;
	}
	else
	{
		if (CapacityValue / 1000)
		{
			TempInst.Value = Utils::to_string_with_precision(CapacityValue / 1000.0);
			TempInst.Unit = BatteryUnit::W;
		}
		else
		{
			TempInst.Value = std::to_string(CapacityValue);
			TempInst.Unit = BatteryUnit::mW;
		}
	}
	return TempInst;
}

Hardware::Battery::BatteryCapacity Hardware::Battery::BatteryInforamtion::BatteryRateToObject(const ULONG& RateValue, const ULONG& Unit)
{
	if (RateValue == BATTERY_UNKNOWN_RATE || RateValue == 0)
		return {};
	BatteryUnit UnitType = BatteryUnit::mW;
	if (Unit & BATTERY_CAPACITY_RELATIVE)
		UnitType = BatteryUnit::PerHour;
	Hardware::Battery::BatteryCapacity TempInst;
	TempInst.Value = std::to_string(RateValue);
	TempInst.Unit = UnitType;
	return TempInst;
}

std::string Hardware::Battery::BatteryInforamtion::BatteryChemistryToString(const std::string& Chemistry)
{
	if (Utils::StringIsEqualsNoCase(Chemistry, "PbAc"))
	{
		return "Lead Acid";
	}
	else if (Utils::StringIsEqualsNoCase(Chemistry, "LION") || Utils::StringIsEqualsNoCase(Chemistry, "Li-I"))
	{
		return "Lithium Ion";
	}
	else if (Utils::StringIsEqualsNoCase(Chemistry, "LI P") == 0 || Utils::StringIsEqualsNoCase(Chemistry, "LIP") == 0)
	{
		return "Lithium Polymer";
	}
	else if (Utils::StringIsEqualsNoCase(Chemistry, "NiCd"))
	{
		return "Nickel Cadmium";
	}
	else if (Utils::StringIsEqualsNoCase(Chemistry, "NiMH"))
	{
		return "Nickel Metal Hydride";
	}
	else if (Utils::StringIsEqualsNoCase(Chemistry, "NiZn"))
	{
		return "Nickel Zinc";
	}
	else if (Utils::StringIsEqualsNoCase(Chemistry, "RAM"))
	{
		return "Rechargeable Alkaline-Manganese";
	}

	return{};
}

std::string Hardware::Battery::BatteryInforamtion::BatteryTechnologyToString(const UCHAR& Technology)
{
	if (Technology == 0)
	{
		return "Primary Battery "; // MSDN Nonrechargeable battery
	}
	else if (Technology == 1)
	{
		return "Secondary Battery "; // MSDN Rechargeable battery
	}
	return {};
}

Hardware::Battery::BatteryCapacity Hardware::Battery::BatteryInforamtion::BatteryCalcDepletion(const ULONG& FullCapacity, const ULONG& DesignCapacity)
{
	if (FullCapacity == BATTERY_UNKNOWN_CAPACITY || BATTERY_UNKNOWN_CAPACITY == DesignCapacity)
		return {};
	if (FullCapacity == 0 || 0 == DesignCapacity)
		return {};

	if (FullCapacity >= DesignCapacity)
		return {};

	Hardware::Battery::BatteryCapacity TempInst;
	TempInst.Value = Utils::to_string_with_precision((DesignCapacity - FullCapacity) * 100.0 / DesignCapacity);
	TempInst.Unit = BatteryUnit::Percent;
	return TempInst;
}
