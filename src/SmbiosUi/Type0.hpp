#pragma once

#include "UI\Source\SmbiosPartialClasses.h"
#include "..\libSmbios\LibSMBIOS.h"
#include "StringHelper.hpp"

namespace Smbios
{
	namespace Display
	{
		auto Type0ToTextList = [](void* Address, auto textList)-> bool
		{
			decltype(auto) Items = textList->GetItems();

			Smbios::Type0 Type(Address);
			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Vendor");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.VendorName()).c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Version");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.BiosVersion()).c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Start Address Segment");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << L"0x" << std::setw(4) << std::setfill(L'0') << std::hex << Type.BiosStartAddress();
				SubItems.Add(oss.str().c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Release Date");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.BiosRelease()).c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"ROM Size");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << std::dec << Type.BiosROMSize() << " KB";
				SubItems.Add(oss.str().c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"BIOS Characteristics");

				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << L"0x" << std::hex << std::setw(16) << std::setfill(L'0') << Type.BIOSCharacteristics();
				SubItems.Add(oss.str().c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Characteristics Extension");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << L"0x" << std::hex << std::setw(4) << std::setfill(L'0') << Type.BIOSCharacteristicsExtension();
				SubItems.Add(oss.str().c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"BIOS Release");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << std::dec << Type.BiosMajorRelease() + 0 << "." << Type.BiosMinorRelease() + 0;
				SubItems.Add(oss.str().c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"EC Release");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << std::dec << Type.EC_MajorRelease() + 0 << "." << Type.EC_MinorRelease() + 0;
				SubItems.Add(oss.str().c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"ROM Extension");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << L"0x" << std::hex << std::setw(4) << std::setfill(L'0') << Type.BiosROMExtended();
				SubItems.Add(oss.str().c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}
			return true;
		};
	}
}