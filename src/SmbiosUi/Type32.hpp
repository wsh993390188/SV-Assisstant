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
			auto Type32ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type32 Type(Address);

				try
				{
					for (const auto& Status : Type.BootStatus())
					{
						std::wostringstream oss;
						oss << std::dec << Status.c_str() << std::endl;
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"Boot Status");
						decltype(auto) SubItems = Item->GetSubItems();
						SubItems.Add(oss.str().c_str());
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