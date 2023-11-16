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
			auto Type25ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type25 Type(Address);

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Next Scheduled Power on Month");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.NextScheduledPoweronMonth() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Next Scheduled Power on Day");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.NextScheduledPoweronDay() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Next Scheduled Power on Hour");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.NextScheduledPoweronHour() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Next Scheduled Power on Minute");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.NextScheduledPoweronMinute() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Next Scheduled Power on Second");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.NextScheduledPoweronSecond() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}
			};
		}
	}
}