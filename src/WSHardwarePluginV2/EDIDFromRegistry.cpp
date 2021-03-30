#include "stdafx.h"
#include "winreg.hpp"
#include "EDIDFromRegistry.h"

namespace Hardware
{
	namespace Monitor
	{
		EDIDFromRegistry::EDIDFromRegistry()
		{
		}

		Data::ErrorType EDIDFromRegistry::Initialize(std::string& response)
		{
			//初始化变量信息
			Monitor.clear();
			MonitorInfos.clear();
			GetCurrentMonitor(Monitor);
			for (const auto& i : Monitor)
			{
				std::wstring Model, Driver;
				GetModelandDriver(i.DeviceID, Model, Driver);
				EDID EdidBuffer{};
				if (GetEDID(Model, Driver, EdidBuffer))
				{
					MonitorDataStruct MonitorInfo;
					MonitorInfo.emplace_back("Name", Utils::wstringToUtf8(i.DeviceString));
					if (ParserEDID(EdidBuffer, MonitorInfo))
					{
						MonitorInfos.emplace(Model, std::move(MonitorInfo));
					}
				}
			}
			if (MonitorInfos.empty())
				return Data::ErrorType::DATAEMPTY;
			response = BuildInitializeJson();
			return Data::ErrorType::SUCCESS;
		}

		Data::ErrorType EDIDFromRegistry::Update(const std::string& Args, std::string& response)
		{
			return Data::ErrorType::NOTSUPPORT;
		}

		bool EDIDFromRegistry::GetCurrentMonitor(std::vector<DISPLAY_DEVICE>& Monitor)
		{
			DWORD MointorIndex = 0, AdapterIndex = 0;
			DISPLAY_DEVICE Adapter;//当前显示设备信息
			DISPLAY_DEVICE Monitortmp;//当前显示设备临时存储信息
			ZeroMemory(&Adapter, sizeof(Adapter));
			Adapter.cb = sizeof(DISPLAY_DEVICE);
			while (
				EnumDisplayDevices(
					NULL,
					AdapterIndex,
					&Adapter,
					0
				))
			{
				ZeroMemory(&Monitortmp, sizeof(Monitortmp));
				Monitortmp.cb = sizeof(DISPLAY_DEVICE);
				DWORD MointorIndex = 0;
				while (
					EnumDisplayDevices(
						Adapter.DeviceName,
						MointorIndex,
						&Monitortmp,
						NULL
					))
				{
					if (
						((Monitortmp.StateFlags & DISPLAY_DEVICE_ACTIVE) == DISPLAY_DEVICE_ACTIVE) &&
						((Monitortmp.StateFlags & DISPLAY_DEVICE_ATTACHED) == DISPLAY_DEVICE_ATTACHED)
						)
					{
						Monitor.emplace_back(Monitortmp);
						// 						this->Adapter.emplace_back(Adapter);
					}
					++MointorIndex;
					ZeroMemory(&Monitortmp, sizeof(Monitortmp));
					Monitortmp.cb = sizeof(DISPLAY_DEVICE);
				}
				++AdapterIndex;
				ZeroMemory(&Adapter, sizeof(Adapter));
				Adapter.cb = sizeof(DISPLAY_DEVICE);
			}

			return !Monitor.empty();
		}

		bool EDIDFromRegistry::GetModelandDriver(const std::wstring& lpDeviceid, std::wstring& Model, std::wstring& Driver)
		{
			Model = _T("");
			Driver = _T("");
			if (lpDeviceid.empty())
			{
				return false;
			}
			size_t offset = 0;
			size_t lpbegin = lpDeviceid.find(_T("\\"), offset);
			if (lpbegin == std::string::npos)
			{
				return false;
			}
			offset = lpbegin + 1;
			size_t lpend = lpDeviceid.find(_T("\\"), offset);
			if (lpend == std::string::npos)
			{
				return false;
			}
			Model.assign(lpDeviceid, lpbegin + 1, lpend - lpbegin - 1);
			lpbegin = lpend;
			Driver.assign(lpDeviceid, lpbegin + 1, lpDeviceid.size());
			return true;
		}

		bool EDIDFromRegistry::GetEDID(const std::wstring& Model, const std::wstring& Driver, EDID& EDIDbuffer)
		{
			constexpr auto RegistryMainKey = _T("SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\");
			winreg::RegKey MonitorMainKey;
			auto ret = MonitorMainKey.TryOpen(HKEY_LOCAL_MACHINE, RegistryMainKey);
			if (ret.Failed())
				return false;

			auto SubKeys = MonitorMainKey.EnumSubKeys();

			for (const auto& SubKey : SubKeys)
			{
				if (Utils::StringIsEqualsNoCase(Model, SubKey))
				{
					winreg::RegKey TargetModelKey;
					ret = TargetModelKey.TryOpen(MonitorMainKey.Get(), SubKey);
					if (ret.Failed())
						break;
					auto ModelSubKeys = TargetModelKey.EnumSubKeys();
					for (const auto& ModelSubKey : ModelSubKeys)
					{
						winreg::RegKey TargetDeviceKey;
						ret = TargetDeviceKey.TryOpen(TargetModelKey.Get(), ModelSubKey);
						if (ret.Failed())
							continue;

						auto DriverStr = TargetDeviceKey.TryGetStringValue(L"Driver");
						if (DriverStr && Utils::StringIsEqualsNoCase(Driver, DriverStr.value()))
						{
							winreg::RegKey EDIDKey;
							ret = EDIDKey.TryOpen(TargetDeviceKey.Get(), L"Device Parameters");
							if (ret.IsOk())
							{
								auto EDIDValue = EDIDKey.TryGetBinaryValue(L"EDID");
								if (EDIDValue)
								{
									memcpy_s(&EDIDbuffer, sizeof(EDIDbuffer), EDIDValue->data(), EDIDValue->size());
									return true;
								}
							}
						}
					}
				}
			}

			return false;
		}
	}
}