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
			auto Type9ToTextList = [](void* Address, auto textList)
			{
				decltype(auto) Items = textList->GetItems();
				Smbios::Type9 Type(Address);
				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Slot Designation");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.SlotDesignation()).c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Slot Type");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.SlotType()).c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Slot Data Bus Width");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.SlotDataBusWidth()).c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Current Usage");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.CurrentUsage()).c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Slot Length");
					decltype(auto) SubItems = Item->GetSubItems();
					SubItems.Add(utf8ToWstring(Type.SlotLength()).c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Slot ID");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.SlotID();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					// Todo
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Slot Characteristics1");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.SlotCharacteristics1().Number;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					// Todo
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Slot Characteristics2");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.SlotCharacteristics2().Number;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Segment Group Number(Base)");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.SegmentGroupNum();
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Bus Number (Base)");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.BusNumber() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					// Todo
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Device/Function Number(Base)");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.DevFuncNum().Number + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Data Bus Width (Base)");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.DataBusWidth() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				try
				{
					auto Item = new ::vl::presentation::controls::list::ListViewItem();
					Item->SetText(L"Peer (S/B/D/F/Width) grouping count(n)");
					decltype(auto) SubItems = Item->GetSubItems();
					std::wostringstream oss;
					oss << std::dec << Type.PeerGroupingCount() + 0;
					SubItems.Add(oss.str().c_str());
					Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
				}
				catch (const std::exception&)
				{
				}

				//Todo PeerGroups
				try
				{
					if (Type.PeerGroupingCount())
					{
						for (const auto& group : Type.PeerGroups())
						{
							auto Item = new ::vl::presentation::controls::list::ListViewItem();
							Item->SetText(L"Peer (S/B/D/F/Width) Group");
							decltype(auto) SubItems = Item->GetSubItems();
							std::wostringstream oss;
							oss << L"0x" << std::hex << std::setw(16) << std::setfill(L'0') << group;
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