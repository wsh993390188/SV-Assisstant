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
			auto Type14ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type14 Type(Address);
				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Group Name");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.GroupName()).c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					for (const auto& str : Type.Items())
					{
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"Item");
						decltype(auto) SubItems = Item->GetSubItems();
						std::wostringstream oss;
						oss << str;
						SubItems.Add(oss.str().c_str());
						Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
					}
				}
				catch (const std::exception&)
				{
				}
			};
		}
	}
}