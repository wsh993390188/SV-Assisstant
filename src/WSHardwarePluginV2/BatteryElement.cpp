#include "stdafx.h"
#include "BatteryElementCommon.h"
#include "BatteryThinkElement.h"
#include "BatteryElement.h"

#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

Hardware::Battery::BatteryElementFactory& Hardware::Battery::BatteryElementFactory::Instance()
{
	static BatteryElementFactory inst;
	return inst;
}

std::shared_ptr<Hardware::Battery::BatteryElement> Hardware::Battery::BatteryElementFactory::CreateBattery(const std::wstring& DevicePath)
{
	if (Smbios::LenovoDisplay::IsThink())
	{
		return std::make_shared<Hardware::Battery::BatteryThinkElement>(DevicePath);
	}
	return std::make_shared<Hardware::Battery::BatteryInforamtion>(DevicePath);
}

std::string Hardware::Battery::BatteryElement::BuildBatteryElementToJsonString(const BatteryCommonStruct& BatteryInfo)
{
	Json::Value root;
	{
		if (BatteryInfo.BatteryDepletion)
		{
			Json::Value Temp;
			Temp["BatteryDepletion"] = BatteryInfo.BatteryDepletion.Tou8String();
			root.append(std::move(Temp));
		}
	}

	{
		if (BatteryInfo.Capacity)
		{
			Json::Value Temp;
			Temp["Capacity"] = BatteryInfo.Capacity.Tou8String();
			root.append(std::move(Temp));
		}
	}

	{
		if (!BatteryInfo.Chemistry.empty())
		{
			Json::Value Temp;
			Temp["Chemistry"] = BatteryInfo.Chemistry;
			root.append(std::move(Temp));
		}
	}

	{
		if (!BatteryInfo.CycleCount.empty())
		{
			Json::Value Temp;
			Temp["CycleCount"] = BatteryInfo.CycleCount;
			root.append(std::move(Temp));
		}
	}

	{
		if (BatteryInfo.DefaultAlert1)
		{
			Json::Value Temp;
			Temp["DefaultAlert1"] = BatteryInfo.DefaultAlert1.Tou8String();
			root.append(std::move(Temp));
		}
	}

	{
		if (BatteryInfo.DefaultAlert2)
		{
			Json::Value Temp;
			Temp["DefaultAlert1"] = BatteryInfo.DefaultAlert2.Tou8String();
			root.append(std::move(Temp));
		}
	}

	{
		if (BatteryInfo.DesignCapacity)
		{
			Json::Value Temp;
			Temp["DesignCapacity"] = BatteryInfo.DesignCapacity.Tou8String();
			root.append(std::move(Temp));
		}
	}

	{
		if (BatteryInfo.FullChargedCapacity)
		{
			Json::Value Temp;
			Temp["FullChargedCapacity"] = BatteryInfo.FullChargedCapacity.Tou8String();
			root.append(std::move(Temp));
		}
	}

	{
		if (!BatteryInfo.Manufacture.empty())
		{
			Json::Value Temp;
			Temp["Manufacture"] = BatteryInfo.Manufacture;
			root.append(std::move(Temp));
		}
	}
	{
		if (!BatteryInfo.ManufactureDate.empty())
		{
			Json::Value Temp;
			Temp["ManufactureDate"] = BatteryInfo.ManufactureDate;
			root.append(std::move(Temp));
		}
	}

	{
		if (BatteryInfo.Rate)
		{
			Json::Value Temp;
			Temp["Rate"] = BatteryInfo.Rate.Tou8String();
			root.append(std::move(Temp));
		}
	}

	{
		if (!BatteryInfo.Percentage.empty())
		{
			Json::Value Temp;
			Temp["Percentage"] = BatteryInfo.Percentage;
			root.append(std::move(Temp));
		}
	}

	{
		if (!BatteryInfo.PowerState.empty())
		{
			Json::Value Temp;
			Temp["PowerState"] = BatteryInfo.PowerState;
			root.append(std::move(Temp));
		}
	}

	{
		if (!BatteryInfo.SerialNumber.empty())
		{
			Json::Value Temp;
			Temp["SerialNumber"] = BatteryInfo.SerialNumber;
			root.append(std::move(Temp));
		}
	}

	{
		if (!BatteryInfo.Technology.empty())
		{
			Json::Value Temp;
			Temp["Technology"] = BatteryInfo.Technology;
			root.append(std::move(Temp));
		}
	}

	{
		if (!BatteryInfo.Temperature.empty())
		{
			Json::Value Temp;
			Temp["Temperature"] = BatteryInfo.Temperature;
			root.append(std::move(Temp));
		}
	}

	{
		if (!BatteryInfo.UniqueID.empty())
		{
			Json::Value Temp;
			Temp["UniqueID"] = BatteryInfo.UniqueID;
			root.append(std::move(Temp));
		}
	}

	{
		if (!BatteryInfo.Voltage.empty())
		{
			Json::Value Temp;
			Temp["Voltage"] = BatteryInfo.Voltage;
			root.append(std::move(Temp));
		}
	}
	if (root.isNull())
		return {};
	return Json::FastWriter().write(root);
}

std::string Hardware::Battery::BatteryCapacity::Tou8String() const
{
	switch (Unit)
	{
	case Hardware::Battery::BatteryUnit::mW:
		return Value + R"(mW)";
	case Hardware::Battery::BatteryUnit::PerHour:
		return Value + R"(Per hour)";
	case Hardware::Battery::BatteryUnit::Percent:
		return Value + R"(%)";
	}

	spdlog::error("Unknown Unit");
	return {};
}