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
			auto Type42ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type42 Type(Address);
				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Interface Type");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.InterfaceType()).c_str());
					Items.Add(Item);
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Interface Data Length");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.InterfaceDataLength() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(Item);
				}
				catch (const std::exception&)
				{
				}

				try
				{
					// Todo
					for (const auto& Data : Type.InterfaceData())
					{
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"Interface Data");
						decltype(auto) SubItems = Item->GetSubItems();
						std::wostringstream oss;
						oss << std::hex << std::setw(2) << std::setfill(L'0') << Data + 0;
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
					Item->SetText(L"Number of Protocol Records");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.NumberOfProtocol() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(Item);
				}
				catch (const std::exception&)
				{
				}

				try
				{
					if (Type.NumberOfProtocol())
					{
						for (const auto& Handle : Type.ProtocolRecords())
						{
							auto Item = new ::vl::presentation::controls::list::ListViewItem();
							Item->SetText(L"Protocol Record");
							decltype(auto) SubItems = Item->GetSubItems();
							std::wostringstream oss;
							oss << Handle;
							SubItems.Add(oss.str().c_str());
							Items.Add(Item);
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