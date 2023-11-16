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
			auto Type44ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type44 Type(Address);

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Referenced Handle");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.ReferencedHandle();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					for (const auto& Handle : Type.ProcessorSpecificBlock())
					{
						std::wostringstream oss;
						oss << Handle << std::endl;
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"Processor-Specific Block");
						decltype(auto) SubItems = Item->GetSubItems();
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