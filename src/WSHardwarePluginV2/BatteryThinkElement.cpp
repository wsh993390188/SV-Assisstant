#include "stdafx.h"
#include "BatteryThinkElement.h"

Hardware::Battery::BatteryThinkElement::BatteryThinkElement(const std::wstring& DevicePath) : BatteryInforamtion(DevicePath), ThinkQueryBatteryInfo(nullptr), hModule(NULL)
{
	//Todo Lenovo «©√˚—È÷§
	hModule = LoadLibrary(_T("ThinkPowerPlugin.dll"));
	if (hModule)
	{
		ThinkQueryBatteryInfo = (QUERYBATTERYINFO)GetProcAddress(hModule, "QueryBatteryInfo");
	}
}

Hardware::Battery::BatteryThinkElement::~BatteryThinkElement()
{
	if (hModule)
	{
		::FreeLibrary(hModule);
	}
}

bool Hardware::Battery::BatteryThinkElement::Update(const uint32_t& BatterySlotId, BatteryCommonStruct& BatteryInfo)
{
	auto ret = __super::Update(BatterySlotId, BatteryInfo);
	if (ret)
	{
		if (ThinkQueryBatteryInfo)
		{
			int CycleCount = -1;
			TCHAR ManufactureDate[20] = {};
			if (ThinkQueryBatteryInfo(BatterySlotId + 1, CycleCount, ManufactureDate, 20))
			{
				BatteryInfo.CycleCount = CycleCount;
				BatteryInfo.ManufactureDate = Utils::wstringToUtf8(std::wstring(ManufactureDate, 20));
			}
			else
			{
				spdlog::error("Machine is Think series but ThinkPowerPlugin not load");
			}
		}
	}
	return ret;
}