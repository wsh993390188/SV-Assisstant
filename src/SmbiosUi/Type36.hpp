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
			auto Type36ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type36 Type(Address);

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Lower Threshold - Non-critical");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.LowerThresholdNoncritical();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Upper Threshold - Non-critical");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.UpperThresholdNoncritical();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Lower Threshold - Critical");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.LowerThresholdCritical();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Upper Threshold - Critical");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.UpperThresholdCritical();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Lower Threshold - Non-recoverable");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.LowerThresholdNonrecoverable();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Upper Threshold - Non-recoverable");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.UpperThresholdNonrecoverable();
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