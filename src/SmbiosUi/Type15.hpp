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
			auto Type15ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type15 Type(Address);

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Log Area Length");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.LogAreaLength();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Log Header Start Offset");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.LogHeaderStartOffset();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Log Data Start Offset");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.LogDataStartOffset();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Access Method");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.AccessMethod()).c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Log Status");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.LogStatus().Number + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Log Change Token");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.LogChangeToken();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Access Method Address");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.AccessMethodAddress().Number + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Log Header Format");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.LogHeaderFormat()).c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Number of Supported Log Type Descriptors");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.NumofSupportedLogTypes() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Length of each Log Type Descriptor");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.LengthLogTypeDescriptor() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					for (const auto& Item : Type.ListofSupportedEventLogTypeDescriptors())
					{
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"Length of each Log Type Descriptor");
						decltype(auto) SubItems = Item->GetSubItems();
						std::wostringstream oss;
						oss << Item;
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