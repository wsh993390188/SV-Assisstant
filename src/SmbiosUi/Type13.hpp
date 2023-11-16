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
			auto Type13ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type13 Type(Address);
				try
				{
					for (const auto& str : Type.InstallableLanguages())
					{
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"Installable Languages");
						decltype(auto) SubItems = Item->GetSubItems();
						SubItems.Add(utf8ToWstring(str).c_str());
						Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
					}
				}
				catch (const std::exception&)
				{
				}

				try
				{
					// Todo ½âÎöFlag
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Flags");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.Flags() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Current Language");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.CurrentLanguages()).c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}
			};
		}
	}
}