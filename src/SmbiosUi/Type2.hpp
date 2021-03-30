#pragma once

#include "UI\Source\SmbiosPartialClasses.h"
#include "..\libSmbios\LibSMBIOS.h"
#include "StringHelper.hpp"

namespace Smbios
{
	namespace Display
	{
		auto Type2ToTextList = [](void* Address, auto textList)
		{
			decltype(auto) Items = textList->GetItems();
			Smbios::Type2 Type(Address);
			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Manufacturer");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.Manufacturer()).c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Product");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.Product()).c_str());
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
				SubItems.Add(utf8ToWstring(Type.Version()).c_str());
				Items.Add(Item);
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
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Asset Tag");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.AssetTag()).c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Feature Flags");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << L"0x" << std::hex << std::setw(2) << std::setfill(L'0') << Type.FeatureFlags();
				SubItems.Add(oss.str().c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Location in Chassis");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.LocationinChassis()).c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Chassis Handle");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << std::dec << Type.ChassisHandle();
				SubItems.Add(oss.str().c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Board Type");
				decltype(auto) SubItems = Item->GetSubItems();
				SubItems.Add(utf8ToWstring(Type.Board()).c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				auto Item = new ::vl::presentation::controls::list::ListViewItem();
				Item->SetText(L"Number of Contained Object Handles");
				decltype(auto) SubItems = Item->GetSubItems();
				std::wostringstream oss;
				oss << std::dec << Type.NumberofContainedObjectHandles() + 0;
				SubItems.Add(oss.str().c_str());
				Items.Add(Item);
			}
			catch (const std::exception&)
			{
			}

			try
			{
				if (Type.NumberofContainedObjectHandles())
				{
					for (const auto& HandleObj : Type.ContainedObjectHandles())
					{
						std::wostringstream oss;
						oss << std::dec << HandleObj;
						auto Item = new ::vl::presentation::controls::list::ListViewItem();
						Item->SetText(L"Object Handle");
						decltype(auto) SubItems = Item->GetSubItems();
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