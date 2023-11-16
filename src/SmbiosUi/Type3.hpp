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
			std::wstring Type3EnumStatusTypeToString(const Type3StatusType& Type)
			{
				std::wstring ret;
				switch (Type)
				{
				case Smbios::Type3StatusType::Reserved:
					ret = L"Reserved";
					break;
				case Smbios::Type3StatusType::Other:
					ret = L"Other";
					break;
				case Smbios::Type3StatusType::Unknown:
					ret = L"Unknown";
					break;
				case Smbios::Type3StatusType::Safe:
					ret = L"Safe";
					break;
				case Smbios::Type3StatusType::Warning:
					ret = L"Warning";
					break;
				case Smbios::Type3StatusType::Critical:
					ret = L"Critical";
					break;
				case Smbios::Type3StatusType::NonRecoverable:
					ret = L"Non-Recoverable";
					break;
				default:
					ret = std::wstring(L"Unknown State ") + std::to_wstring(ToUType(Type)).c_str();
					break;
				}
				return ret;
			}

			std::wstring Type3EnumSecurityStatusTypeToString(const Type3SecurityStatus& Type)
			{
				std::wstring ret;
				switch (Type)
				{
				case Smbios::Type3SecurityStatus::None:
					ret = L"None";
					break;
				case Smbios::Type3SecurityStatus::Other:
					ret = L"Other";
					break;
				case Smbios::Type3SecurityStatus::Unknown:
					ret = L"Unknown";
					break;
				case Smbios::Type3SecurityStatus::ExternalInterfaceLockedOut:
					ret = L"External interface locked out";
					break;
				case Smbios::Type3SecurityStatus::ExternalInterfaceEnabled:
					ret = L"External interface enabled";
					break;
				default:
					ret = std::wstring(L"Unknown Security State ") + std::to_wstring(ToUType(Type)).c_str();
					break;
				}
				return ret;
			}
		}
		auto Type3ToTextList = [](void* Address, auto textList)
		{
			decltype(auto) Items = textList->GetItems();
			Smbios::Type3 Type(Address);
			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Manufacturer");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.Manufacturer()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}
			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Enclosure Type");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.Type()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Version");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.Version()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Serial Number");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.SerialNumber()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Asset Tag Number");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.AssetTag()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Boot-Up State");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(Type3EnumStatusTypeToString(Type.BootUpState()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Power Supply State");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(Type3EnumStatusTypeToString(Type.PowerSupplyState()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Thermal State");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(Type3EnumStatusTypeToString(Type.ThermalState()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Security State");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(Type3EnumSecurityStatusTypeToString(Type.SecurityState()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"OEM-defined");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << std::dec << Type.OEMDefined();
				SubItems.Add(oss.str().c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Height");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << std::dec << Type.Height() + 0;
				SubItems.Add(oss.str().c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Number of Power Cords");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << std::dec << Type.NumberofPowerCords() + 0;
				SubItems.Add(oss.str().c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Contained Element Count(n)");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << std::dec << Type.ContainedElementCount() + 0;
				SubItems.Add(oss.str().c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Contained Element Record Length(m)");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << std::dec << Type.ContainedElementRecordLength() + 0;
				SubItems.Add(oss.str().c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				if (Type.ContainedElementCount())
				{
					for (const auto& HandleObj : Type.ContainedElements())
					{
						std::wostringstream oss;
						oss << " " << std::hex << std::setw(2) << std::setfill(L'0') << HandleObj;
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"Contained Element");
						decltype(auto) SubItems = Item->GetSubItems();
						SubItems.Add(oss.str().c_str());
						Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
					}
				}
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"SKU Number");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.SKUNumber()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}
		};
	}
}