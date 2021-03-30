#pragma once

#include "UI\Source\SmbiosPartialClasses.h"
#include "..\libSmbios\LibSMBIOS.h"
#include "StringHelper.hpp"

namespace Smbios
{
	namespace Display
	{
		namespace
		{
			std::wstring DeviceInformationToString(const Smbios::OnBroadDeviceInformation dev)
			{
				std::wostringstream oss;
				oss << "	Description: " << dev.DeviceDesc.c_str() << std::endl;
				oss << "	Type: " << dev.DeviceType.c_str() << std::endl;
				oss << "	Status: ";
				if (dev.Status == Smbios::OnBroadDeviceInformation::DeviceTypeStatus::Enable)
				{
					oss << L"Enable" << std::endl;
				}
				else
				{
					oss << L"Disable" << std::endl;
				}
				return oss.str();
			}
			auto Type10ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type10 Type(Address);
				try
				{
					for (const auto& Device : Type.DeviceInformation())
					{
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"Error Detecting Method");
						decltype(auto) SubItems = Item->GetSubItems();
						SubItems.Add(DeviceInformationToString(Device).c_str());
						Items.Add(Item);
					}
				}
				catch (const std::exception&)
				{
				}
			};
		}
	}
}