#pragma once
#include "UI\Source\SmbiosPartialClasses.h"
#include "..\libSmbios\LibSMBIOS.h"
#include "StringHelper.hpp"

namespace Smbios
{
	namespace Display
	{
		auto Type1ToTextList = [](void* Address, auto textList)
		{
			decltype(auto) Items = textList->GetItems();

			Smbios::Type1 Type(Address);
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
				Item->SetText(L"Product Name");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.ProductName()).c_str());
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
				Item->SetText(L"UUID");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.UUID()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"WakeUp Type");
				decltype(auto) SubItems = Item->GetSubItems();
				auto WakeType = Type.WakeUp();
				switch (WakeType)
				{
				case Smbios::WakeUpType::Reserved:
					SubItems.Add(L"Reserved");
					break;
				case Smbios::WakeUpType::Other:
					SubItems.Add(L"Other");
					break;
				case Smbios::WakeUpType::Unknown:
					SubItems.Add(L"Unknown");
					break;
				case Smbios::WakeUpType::APMTimer:
					SubItems.Add(L"APM Timer");
					break;
				case Smbios::WakeUpType::ModemRing:
					SubItems.Add(L"Modem Ring");
					break;
				case Smbios::WakeUpType::LANRemote:
					SubItems.Add(L"LAN Remote");
					break;
				case Smbios::WakeUpType::PowerSwitch:
					SubItems.Add(L"Power Switch");
					break;
				case Smbios::WakeUpType::PCIPME:
					SubItems.Add(L"PCI PME#");
					break;
				case Smbios::WakeUpType::ACPowerRestored:
					SubItems.Add(L"AC Power Restored");
					break;
				default:
					SubItems.Add((std::wstring(L"Unknown Wake Type ") + std::to_wstring(ToUType(WakeType))).c_str());
					break;
				}
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
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

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Family");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.Family()).c_str());
				Items.Add(::vl::Ptr<::vl::presentation::controls::list::ListViewItem>(Item));
			}
			catch (const std::exception&)
			{
			}
		};
	}
}