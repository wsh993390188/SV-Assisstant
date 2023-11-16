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
			auto Type12ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type12 Type(Address);

				try
				{
					for (const auto& str : Type.Count())
					{
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"Configuration Information Strings");
						decltype(auto) SubItems = Item->GetSubItems();
						SubItems.Add(utf8ToWstring(str).c_str());
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