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
			auto Type40ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type40 Type(Address);

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Number of Additional Information entries");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.NumofAdditionInfEntries() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					if (Type.NumofAdditionInfEntries())
					{
						for (const auto& Handle : Type.AdditionInfEntries())
						{
							auto Item = new ::vl::presentation::controls::list::ListViewItem();
							Item->SetText(L"Number of Associated Memory Slots");
							decltype(auto) SubItems = Item->GetSubItems();
							std::wostringstream oss;
							oss << Handle;
							SubItems.Add(oss.str().c_str());
							Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
						}
					}
				}
				catch (const std::exception&)
				{
				}
			};
		}
	}
}