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
			auto Type131_Lenovo_ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Lenovo::Type131 Type(Address);

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Version");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.Version() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(Item);
				}
				catch (const std::exception&)
				{
				}

				try
				{
					std::wostringstream oss;
					for (const auto& Handle : Type.TVTStructure())
					{
						oss << std::hex << std::setw(2) << std::setfill(L'0') << Handle << std::endl;
					}
					if (!oss.str().empty())
					{
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"TVT Structure");
						decltype(auto) SubItems = Item->GetSubItems();
						SubItems.Add(oss.str().c_str());
						Items.Add(Item);
					}
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Description");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.Description()).c_str());
					Items.Add(Item);
				}
				catch (const std::exception&)
				{
				}
			};
		}
	}
}