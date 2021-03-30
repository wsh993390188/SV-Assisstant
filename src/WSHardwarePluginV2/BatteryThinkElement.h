#pragma once
#include "BatteryElementCommon.h"
namespace Hardware
{
	namespace Battery
	{
		class BatteryThinkElement final : public BatteryInforamtion
		{
		public:
			explicit BatteryThinkElement(const std::wstring& DevicePath);
			~BatteryThinkElement();

			bool Update(const uint32_t& BatterySlotId, BatteryCommonStruct& BatteryInfo)override final;
		private:
			using QUERYBATTERYINFO = bool (WINAPI*)(int slotNumber, int& cycleCount, LPTSTR ManufactureDate, int dataSize);
			QUERYBATTERYINFO ThinkQueryBatteryInfo;
			HMODULE hModule;
		};
	}
}
