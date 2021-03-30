#include "pch.h"

// SmbiosConsoleApp.cpp
//

#include <string>
#include <iostream>
#include <Windows.h>
#include <cstdio>

#include "..\..\src\libSmbios\LibSMBIOS.h"
inline namespace
{
	template<typename T>
	constexpr decltype(auto) ToUType(T enumerator)
	{
		return static_cast<std::underlying_type_t<T>>(enumerator);
	}

	template<typename T1, typename std::enable_if_t<std::is_integral_v<T1>, int> = 0>
	std::ostream& operator<<(std::ostream& stream, const std::vector<T1>& map)
	{
		if (map.empty())
		{
			stream << "null";
			return stream;
		}
		for (typename std::vector<T1>::const_iterator it = map.begin();
			it != map.end();
			++it)
		{
			stream << *it + 0 << ",";
		}
		return stream;
	}

	template<typename T1, typename std::enable_if_t<!std::is_integral_v<T1>, int> = 0>
	std::ostream& operator<<(std::ostream& stream, const std::vector<T1>& map)
	{
		for (typename std::vector<T1>::const_iterator it = map.begin();
			it != map.end();
			++it)
		{
			stream << *it << ",";
		}
		return stream;
	}
}

TEST(SMBIOS, Type5)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type5());

	for (auto& type0 : types)
	{
		{
			std::string Str1, Str2;
			auto res = type0.ErrorDetectingMethod(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorDetectingMethod());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " ErrorDetectingMethod: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorDetectingMethod()) << "Test ErrorDetectingMethod";
			}
		}

		{
			Smbios::ErrorCapability Str1, Str2;
			auto res = type0.ErrorCorrectingCapability(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorCorrectingCapability());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " ErrorCorrectingCapability: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorCorrectingCapability()) << "Test ErrorCorrectingCapability";
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.SupportedInterleave(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SupportedInterleave());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " SupportedInterleave: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SupportedInterleave()) << "Test ErrorDetectingMethod";
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.CurrentInterleave(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CurrentInterleave());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " CurrentInterleave: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CurrentInterleave()) << "Test CurrentInterleave";
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.MaximumMemoryMoudulesize(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaximumMemoryMoudulesize());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " MaximumMemoryMoudulesize: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaximumMemoryMoudulesize()) << "Test MaximumMemoryMoudulesize";
			}
		}

		{
			Smbios::MemorySpeeds Str1, Str2;
			auto res = type0.SupportedSpeeds(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SupportedSpeeds());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " SupportedSpeeds: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SupportedSpeeds()) << "Test SupportedSpeeds";
			}
		}

		{
			Smbios::MemoryTypes Str1, Str2;
			auto res = type0.SupportedMemoryTypes(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SupportedMemoryTypes());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " SupportedMemoryTypes: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SupportedMemoryTypes()) << "Test SupportedMemoryTypes";
			}
		}

		{
			Smbios::MemoryVoltages Str1, Str2;
			auto res = type0.MemoryModuleVoltage(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryModuleVoltage());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " MemoryModuleVoltage: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryModuleVoltage()) << "Test MemoryModuleVoltage";
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.NumofMemorySlots(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NumofMemorySlots());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " NumofMemorySlots: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NumofMemorySlots()) << "Test NumofMemorySlots";
			}
		}

		{
			std::vector<uint16_t> Str1, Str2;
			auto res = type0.MemoryModuleConfigurationHandles(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryModuleConfigurationHandles());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " MemoryModuleConfigurationHandles: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryModuleConfigurationHandles()) << "Test MemoryModuleConfigurationHandles";
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.EnabledErrorCorrectingCapabilities(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.EnabledErrorCorrectingCapabilities());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type5: " << " EnabledErrorCorrectingCapabilities: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.EnabledErrorCorrectingCapabilities()) << "Test EnabledErrorCorrectingCapabilities";
			}
		}
	}
}

TEST(SMBIOS, Type1)
{
	auto type0 = Smbios::GetType(Smbios::SmbiosHeaderType::Type1());
	{
		std::string Str1, Str2;
		auto res = type0.Manufacturer(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Manufacturer());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type1: " << " Manufacturer: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Manufacturer());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.ProductName(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.ProductName());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type1: " << " ProductName: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.ProductName());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.Version(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Version());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type1: " << " Version: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Version());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.SerialNumber(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.SerialNumber());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type1: " << " SerialNumber: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.SerialNumber());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.UUID(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.UUID());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type1: " << " UUID: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.UUID());
		}
	}

	{
		Smbios::WakeUpType Str1, Str2;
		auto res = type0.WakeUp(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.WakeUp());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type1: " << " WakeUp: " << ToUType(Str1) + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.WakeUp());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.SKUNumber(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.SKUNumber());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type1: " << " SKUNumber: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.SKUNumber());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.Family(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Family());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type1: " << " Family: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Family());
		}
	}
}

TEST(SMBIOS, Type0)
{
	auto type0 = Smbios::GetType(Smbios::SmbiosHeaderType::Type0());
	{
		std::string Str1, Str2;
		auto res = type0.VendorName(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.VendorName());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " VendorName: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.VendorName());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.BiosVersion(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.BiosVersion());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " BiosVersion: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.BiosVersion());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.BiosRelease(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.BiosRelease());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " BiosRelease: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.BiosRelease());
		}
	}

	{
		std::uint16_t Str1, Str2;
		auto res = type0.BiosROMSize(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.BiosROMSize());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " BiosROMSize: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.BiosROMSize());
		}
	}

	{
		std::uint64_t Str1, Str2;
		auto res = type0.BIOSCharacteristics(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.BIOSCharacteristics());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " BIOSCharacteristics: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.BIOSCharacteristics());
		}
	}

	{
		std::uint16_t Str1, Str2;
		auto res = type0.BIOSCharacteristicsExtension(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.BIOSCharacteristicsExtension());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " BIOSCharacteristicsExtension: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.BIOSCharacteristicsExtension());
		}
	}

	{
		std::uint8_t Str1, Str2;
		auto res = type0.BiosMajorRelease(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.BiosMajorRelease());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " BiosMajorRelease: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.BiosMajorRelease());
		}
	}

	{
		std::uint8_t Str1, Str2;
		auto res = type0.BiosMinorRelease(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.BiosMinorRelease());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " BiosMinorRelease: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.BiosMinorRelease());
		}
	}

	{
		std::uint8_t Str1, Str2;
		auto res = type0.EC_MajorRelease(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.EC_MajorRelease());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " EC_MajorRelease: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.EC_MajorRelease());
		}
	}

	{
		std::uint8_t Str1, Str2;
		auto res = type0.EC_MinorRelease(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.EC_MinorRelease());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " EC_MinorRelease: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.EC_MinorRelease());
		}
	}

	{
		std::uint16_t Str1, Str2;
		auto res = type0.BiosROMExtended(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.BiosROMExtended());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type0: " << " BiosROMExtended: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.BiosROMExtended());
		}
	}
}

TEST(SMBIOS, Type2)
{
	auto type0 = Smbios::GetType(Smbios::SmbiosHeaderType::Type2());
	{
		std::string Str1, Str2;
		auto res = type0.Manufacturer(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Manufacturer());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " Manufacturer: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Manufacturer());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.Product(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Product());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " Product: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Product());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.Version(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Version());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " Version: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Version());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.SerialNumber(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.SerialNumber());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " SerialNumber: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.SerialNumber());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.AssetTag(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.AssetTag());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " AssetTag: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.AssetTag());
		}
	}

	{
		std::uint8_t Str1, Str2;
		auto res = type0.FeatureFlags(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.FeatureFlags());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " FeatureFlags: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.FeatureFlags());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.LocationinChassis(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.LocationinChassis());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " LocationinChassis: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.LocationinChassis());
		}
	}

	{
		std::uint16_t Str1, Str2;
		auto res = type0.ChassisHandle(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.ChassisHandle());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " ChassisHandle: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.ChassisHandle());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.Board(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Board());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " Board: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Board());
		}
	}
	{
		std::uint8_t Str1, Str2;
		auto res = type0.NumberofContainedObjectHandles(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.NumberofContainedObjectHandles());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " NumberofContainedObjectHandles: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.NumberofContainedObjectHandles());
		}
	}
	{
		std::vector<uint16_t> Str1, Str2;
		auto res = type0.ContainedObjectHandles(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.ContainedObjectHandles());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type2: " << " ContainedObjectHandles: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.ContainedObjectHandles());
		}
	}
}

TEST(SMBIOS, Type3)
{
	auto type0 = Smbios::GetType(Smbios::SmbiosHeaderType::Type3());
	{
		std::string Str1, Str2;
		auto res = type0.Manufacturer(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Manufacturer());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " Manufacturer: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Manufacturer());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.Type(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Type());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " Type: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Type());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.Version(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Version());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " Version: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Version());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.SerialNumber(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.SerialNumber());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " SerialNumber: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.SerialNumber());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.AssetTag(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.AssetTag());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " AssetTag: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.AssetTag());
		}
	}

	{
		Smbios::Type3StatusType Str1, Str2;
		auto res = type0.BootUpState(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.BootUpState());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " BootUpState: " << ToUType(Str1) + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.BootUpState());
		}
	}

	{
		Smbios::Type3StatusType Str1, Str2;
		auto res = type0.PowerSupplyState(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.PowerSupplyState());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " PowerSupplyState: " << ToUType(Str1) + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.PowerSupplyState());
		}
	}
	{
		Smbios::Type3StatusType Str1, Str2;
		auto res = type0.ThermalState(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.ThermalState());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " ThermalState: " << ToUType(Str1) + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.ThermalState());
		}
	}
	{
		Smbios::Type3SecurityStatus Str1, Str2;
		auto res = type0.SecurityState(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.SecurityState());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " SecurityState: " << ToUType(Str1) + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.SecurityState());
		}
	}
	{
		std::uint32_t Str1, Str2;
		auto res = type0.OEMDefined(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.OEMDefined());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " OEMDefined: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.OEMDefined());
		}
	}
	{
		std::uint8_t Str1, Str2;
		auto res = type0.Height(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Height());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " Height: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Height());
		}
	}
	{
		std::uint8_t Str1, Str2;
		auto res = type0.NumberofPowerCords(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.NumberofPowerCords());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " NumberofPowerCords: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.NumberofPowerCords());
		}
	}
	{
		std::uint8_t Str1, Str2;
		auto res = type0.ContainedElementCount(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.ContainedElementCount());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " ContainedElementCount: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.ContainedElementCount());
		}
	}
	{
		std::uint8_t Str1, Str2;
		auto res = type0.ContainedElementRecordLength(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.ContainedElementRecordLength());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " ContainedElementRecordLength: " << Str1 + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.ContainedElementRecordLength());
		}
	}

	{
		std::vector<uint8_t> Str1, Str2;
		auto res = type0.ContainedElements(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.ContainedElements());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " ContainedElements: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.ContainedElements());
		}
	}

	{
		std::string Str1, Str2;
		auto res = type0.SKUNumber(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.SKUNumber());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type3: " << " SKUNumber: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.SKUNumber());
		}
	}
}

TEST(SMBIOS, Type4)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type4());
	for (auto& type0 : types)
	{
		{
			std::string Str1, Str2;
			auto res = type0.SocketDesignation(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SocketDesignation());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " SocketDesignation: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SocketDesignation());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ProcessorType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ProcessorType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " ProcessorType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ProcessorType());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ProcessorFamily(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ProcessorFamily());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " ProcessorFamily: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ProcessorFamily());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Manufacturer(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Manufacturer());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " Manufacturer: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Manufacturer());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ProcessorID(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ProcessorID());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " ProcessorID: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ProcessorID());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ProcessorVersion(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ProcessorVersion());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " ProcessorVersion: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ProcessorVersion());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Voltage(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Voltage());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " Voltage: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Voltage());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.ExtClock(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExtClock());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " ExtClock: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExtClock());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.MaxSpeed(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaxSpeed());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " MaxSpeed: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaxSpeed());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.CurrentSpeed(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CurrentSpeed());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " CurrentSpeed: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CurrentSpeed());
			}
		}
		{
			std::uint8_t Str1, Str2;
			auto res = type0.Status(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Status());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " Status: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Status());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.ProcessorUpgrade(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ProcessorUpgrade());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " ProcessorUpgrade: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ProcessorUpgrade());
			}
		}
		{
			std::uint16_t Str1, Str2;
			auto res = type0.L1CacheHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.L1CacheHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " L1CacheHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.L1CacheHandle());
			}
		}
		{
			std::uint16_t Str1, Str2;
			auto res = type0.L2CacheHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.L2CacheHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " L2CacheHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.L2CacheHandle());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.L3CacheHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.L3CacheHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " L3CacheHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.L3CacheHandle());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.SerialNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SerialNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " SerialNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SerialNumber());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.PartNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PartNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " PartNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PartNumber());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.AssetTag(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.AssetTag());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " AssetTag: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.AssetTag());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.CoreCount(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CoreCount());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " CoreCount: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CoreCount());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.CoreEnable(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CoreEnable());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " CoreEnable: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CoreEnable());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.ThreadCount(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ThreadCount());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " ThreadCount: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ThreadCount());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ProcessorCharacteristics(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ProcessorCharacteristics());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " ProcessorCharacteristics: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ProcessorCharacteristics());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ProcessorFamily2(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ProcessorFamily2());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " ProcessorFamily2: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ProcessorFamily2());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.CoreCount2(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CoreCount2());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " CoreCount2: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CoreCount2());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.CoreEnable2(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CoreEnable2());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " CoreEnable2: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CoreEnable2());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ThreadCount2(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ThreadCount2());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type4: " << " ThreadCount2: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ThreadCount2());
			}
		}
	}
}
TEST(SMBIOS, Type6)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type6());
	for (auto& type0 : types)
	{
		{
			std::string Str1, Str2;
			auto res = type0.SocketDesignation(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SocketDesignation());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type6: " << " SocketDesignation: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SocketDesignation());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.BankConnections(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.BankConnections());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type6: " << " BankConnections: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.BankConnections());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.CurrentSpeed(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CurrentSpeed());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type6: " << " CurrentSpeed: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CurrentSpeed());
			}
		}

		{
			Smbios::MemoryTypes Str1, Str2;
			auto res = type0.CurrentMemoryTypes(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CurrentMemoryTypes());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type6: " << " CurrentMemoryTypes: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CurrentMemoryTypes());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.InstalledSize(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InstalledSize());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type6: " << " InstalledSize: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InstalledSize());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.EnabledSize(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.EnabledSize());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type6: " << " EnabledSize: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.EnabledSize());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.ErrorStatus(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorStatus());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type6: " << " ErrorStatus: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorStatus());
			}
		}
	}
}
TEST(SMBIOS, Type7)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type7());

	auto count = 0;
	for (auto& type0 : type)
	{
		std::cout << "Type7: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.SocketDesignation(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SocketDesignation());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " SocketDesignation: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SocketDesignation());
			}
		}
		{
			std::uint16_t Str1, Str2;
			auto res = type0.CacheConfiguration(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CacheConfiguration());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " CacheConfiguration: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CacheConfiguration());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.MaximumCacheSize(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaximumCacheSize());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " MaximumCacheSize: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaximumCacheSize());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.InstalledSize(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InstalledSize());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " InstalledSize: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InstalledSize());
			}
		}
		{
			Smbios::SRAMType Str1, Str2;
			auto res = type0.SupportedSRAMType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SupportedSRAMType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " SupportedSRAMType: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SupportedSRAMType());
			}
		}
		{
			Smbios::SRAMType Str1, Str2;
			auto res = type0.CurrentSRAMType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CurrentSRAMType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " CurrentSRAMType: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CurrentSRAMType());
			}
		}
		{
			std::uint8_t Str1, Str2;
			auto res = type0.CacheSpeed(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CacheSpeed());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " CacheSpeed: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CacheSpeed());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.ErrorCorrectionType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorCorrectionType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " ErrorCorrectionType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorCorrectionType());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.SystemCacheType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SystemCacheType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " SystemCacheType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SystemCacheType());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.Associativity(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Associativity());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " Associativity: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Associativity());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.MaximumCacheSize2(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaximumCacheSize2());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " MaximumCacheSize2: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaximumCacheSize2());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.InstalledCacheSize2(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InstalledCacheSize2());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type7: " << " InstalledCacheSize2: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InstalledCacheSize2());
			}
		}
	}
}

TEST(SMBIOS, Type8)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type8());

	auto count = 0;
	for (auto& type0 : type)
	{
		std::cout << "Type8: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.InternalReferenceDesignator(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InternalReferenceDesignator());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type8: " << " InternalReferenceDesignator: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InternalReferenceDesignator());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.InternalConnectorType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InternalConnectorType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type8: " << " InternalConnectorType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InternalConnectorType());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.ExternalReferenceDesignator(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExternalReferenceDesignator());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type8: " << " ExternalReferenceDesignator: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExternalReferenceDesignator());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.ExternalConnectorType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExternalConnectorType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type8: " << " ExternalConnectorType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExternalConnectorType());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.PortType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PortType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type8: " << " PortType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PortType());
			}
		}
	}
}

TEST(SMBIOS, Type9)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type9());

	auto count = 0;
	for (auto& type0 : type)
	{
		std::cout << "Type9: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.SlotDesignation(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SlotDesignation());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " SlotDesignation: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SlotDesignation());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.SlotType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SlotType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type89 " << " SlotType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SlotType());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.SlotDataBusWidth(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SlotDataBusWidth());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " SlotDataBusWidth: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SlotDataBusWidth());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.CurrentUsage(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CurrentUsage());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " CurrentUsage: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CurrentUsage());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.SlotLength(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SlotLength());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " SlotLength: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SlotLength());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.SlotID(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SlotID());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " SlotID: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SlotID());
			}
		}

		{
			Smbios::SlotCharacteristics1Field Str1, Str2;
			auto res = type0.SlotCharacteristics1(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SlotCharacteristics1());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " SlotCharacteristics1: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SlotCharacteristics1());
			}
		}

		{
			Smbios::SlotCharacteristics2Field Str1, Str2;
			auto res = type0.SlotCharacteristics2(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SlotCharacteristics2());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " SlotCharacteristics2: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SlotCharacteristics2());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.SegmentGroupNum(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SegmentGroupNum());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " SegmentGroupNum: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SegmentGroupNum());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.BusNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.BusNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " BusNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.BusNumber());
			}
		}

		{
			Smbios::DeviceFuncField Str1, Str2;
			auto res = type0.DevFuncNum(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DevFuncNum());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " DevFuncNum: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DevFuncNum());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.DataBusWidth(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DataBusWidth());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " DataBusWidth: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DataBusWidth());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.PeerGroupingCount(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PeerGroupingCount());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " PeerGroupingCount: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PeerGroupingCount());
			}
		}

		{
			std::vector<uint64_t> Str1, Str2;
			auto res = type0.PeerGroups(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PeerGroups());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type9: " << " PeerGroups: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PeerGroups());
			}
		}
	}
}

TEST(SMBIOS, Type10)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type10());
	for (auto& type0 : types)
	{
		{
			std::vector<Smbios::OnBroadDeviceInformation> Str1, Str2;
			auto res = type0.DeviceInformation(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceInformation());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type10: " << " OnBroadDeviceInformation: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceInformation());
			}
		}
	}
}

TEST(SMBIOS, Type11)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type11());
	for (auto& type0 : types)
	{
		{
			std::vector<std::string> Str1, Str2;
			auto res = type0.Count(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Count());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type11: " << " Count: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Count());
			}
		}
	}
}

TEST(SMBIOS, Type12)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type12());
	for (auto& type0 : types)
	{
		std::vector<std::string> Str1, Str2;
		auto res = type0.Count(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.Count());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type12: " << " Count: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.Count());
		}
	}
}

TEST(SMBIOS, Type13)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type13());
	for (auto& type0 : types)
	{
		{
			std::vector<std::string> Str1, Str2;
			auto res = type0.InstallableLanguages(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InstallableLanguages());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type13: " << " InstallableLanguages: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InstallableLanguages());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.Flags(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Flags());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type13: " << " Flags: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Flags());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.CurrentLanguages(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CurrentLanguages());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type13: " << " CurrentLanguages: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CurrentLanguages());
			}
		}
	}
}

TEST(SMBIOS, Type14)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type14());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type14: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.GroupName(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.GroupName());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type14: " << " GroupName: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.GroupName());
			}
		}

		{
			std::vector<Smbios::GroupAssociationItems> Str1, Str2;
			auto res = type0.Items(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Items());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type14: " << " Items: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Items());
			}
		}
	}
}

TEST(SMBIOS, Type15)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type15());
	for (auto& type0 : types)
	{
		{
			uint16_t Str1, Str2;
			auto res = type0.LogAreaLength(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LogAreaLength());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " LogAreaLength: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LogAreaLength());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.LogHeaderStartOffset(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LogHeaderStartOffset());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " LogHeaderStartOffset: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LogHeaderStartOffset());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.LogDataStartOffset(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LogDataStartOffset());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " LogDataStartOffset: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LogDataStartOffset());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.AccessMethod(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.AccessMethod());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " AccessMethod: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.AccessMethod());
			}
		}

		{
			Smbios::LogStatusField Str1, Str2;
			auto res = type0.LogStatus(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LogStatus());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " LogStatus: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LogStatus());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.LogChangeToken(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LogChangeToken());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " LogChangeToken: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LogChangeToken());
			}
		}

		{
			Smbios::AccessMethodAddressField Str1, Str2;
			auto res = type0.AccessMethodAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.AccessMethodAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " AccessMethodAddress: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.AccessMethodAddress());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.LogHeaderFormat(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LogHeaderFormat());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " LogHeaderFormat: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LogHeaderFormat());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.NumofSupportedLogTypes(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NumofSupportedLogTypes());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " NumofSupportedLogTypes: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NumofSupportedLogTypes());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.LengthLogTypeDescriptor(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LengthLogTypeDescriptor());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " LengthLogTypeDescriptor: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LengthLogTypeDescriptor());
			}
		}

		{
			std::vector<Smbios::EventLogType> Str1, Str2;
			auto res = type0.ListofSupportedEventLogTypeDescriptors(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ListofSupportedEventLogTypeDescriptors());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type15: " << " ListofSupportedEventLogTypeDescriptors: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ListofSupportedEventLogTypeDescriptors());
			}
		}
	}
}

TEST(SMBIOS, Type16)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type16());
	for (auto& type0 : types)
	{
		{
			std::string Str1, Str2;
			auto res = type0.Location(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Location());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type16: " << " Location: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Location());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Use(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Use());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type16: " << " Use: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Use());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.MemoryErrorCorrection(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryErrorCorrection());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type16: " << " MemoryErrorCorrection: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryErrorCorrection());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.MaximumCapacity(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaximumCapacity());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type16: " << " MaximumCapacity: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaximumCapacity());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.MemoryErrorInformationHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryErrorInformationHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type16: " << " MemoryErrorInformationHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryErrorInformationHandle());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.NumOfMemoryDevices(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NumOfMemoryDevices());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type16: " << " NumOfMemoryDevices: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NumOfMemoryDevices());
			}
		}

		{
			std::uint64_t Str1, Str2;
			auto res = type0.ExtendedMaximumCapacity(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExtendedMaximumCapacity());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type16: " << " ExtendedMaximumCapacity: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExtendedMaximumCapacity());
			}
		}
	}
}

TEST(SMBIOS, Type17)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type17());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type17: " << count++ << std::endl;
		{
			std::uint16_t Str1, Str2;
			auto res = type0.PhysicalMemoryArrayHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PhysicalMemoryArrayHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " PhysicalMemoryArrayHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PhysicalMemoryArrayHandle());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.MemoryErrorInformationHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryErrorInformationHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " MemoryErrorInformationHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryErrorInformationHandle());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.MemoryTotalWidth(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryTotalWidth());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " MemoryTotalWidth: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryTotalWidth());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.Datawidth(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Datawidth());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " Datawidth: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Datawidth());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.Size(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Size());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " Size: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Size());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.FormFactor(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.FormFactor());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " FormFactor: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.FormFactor());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.DeviceSet(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceSet());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " DeviceSet: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceSet());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.DeviceLocator(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceLocator());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " DeviceLocator: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceLocator());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.BankLocator(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.BankLocator());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " BankLocator: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.BankLocator());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.MemoryType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " MemoryType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryType());
			}
		}

		{
			Smbios::MemoryTypeDetailField Str1, Str2;
			auto res = type0.TypeDetail(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.TypeDetail());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " TypeDetail: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.TypeDetail());
			}
		}
		{
			std::uint16_t Str1, Str2;
			auto res = type0.Speed(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Speed());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " Speed: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Speed());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Manufacturer(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Manufacturer());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " Manufacturer: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Manufacturer());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.SerialNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SerialNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " SerialNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SerialNumber());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.AssetTag(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.AssetTag());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " AssetTag: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.AssetTag());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.PartNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PartNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " PartNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PartNumber());
			}
		}

		{
			Smbios::MemoryAttributeField Str1, Str2;
			auto res = type0.Attributes(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Attributes());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " Attributes: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Attributes());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.ExtentedSize(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExtentedSize());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " ExtentedSize: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExtentedSize());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ConfiguredMemoryClockSpeed(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ConfiguredMemoryClockSpeed());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " ConfiguredMemoryClockSpeed: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ConfiguredMemoryClockSpeed());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.MinimumVoltage(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MinimumVoltage());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " MinimumVoltage: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MinimumVoltage());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.MaximumVoltage(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaximumVoltage());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " MaximumVoltage: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MinimumVoltage());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ConfiguredVoltage(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ConfiguredVoltage());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " ConfiguredVoltage: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ConfiguredVoltage());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.MemoryTechnology(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryTechnology());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " MemoryTechnology: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryTechnology());
			}
		}

		{
			Smbios::MemoryModeCapability Str1, Str2;
			auto res = type0.MemoryOperatingModeCapability(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryOperatingModeCapability());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " MemoryOperatingModeCapability: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryOperatingModeCapability());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.FirmwareVersion(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.FirmwareVersion());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " FirmwareVersion: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.FirmwareVersion());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ModuleManufacturerID(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ModuleManufacturerID());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " ModuleManufacturerID: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ModuleManufacturerID());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ModuleProductID(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ModuleProductID());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " ModuleProductID: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ModuleProductID());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.MemorySubsystemControllerManufacturerID(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemorySubsystemControllerManufacturerID());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " ModuleMemorySubsystemControllerManufacturerIDProductID: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemorySubsystemControllerManufacturerID());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.MemorySubsystemControllerProductID(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemorySubsystemControllerProductID());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " MemorySubsystemControllerProductID: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemorySubsystemControllerProductID());
			}
		}

		{
			std::uint64_t Str1, Str2;
			auto res = type0.NonVolatileSize(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NonVolatileSize());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " NonVolatileSize: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NonVolatileSize());
			}
		}

		{
			std::uint64_t Str1, Str2;
			auto res = type0.VolatileSize(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.VolatileSize());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " VolatileSize: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.VolatileSize());
			}
		}

		{
			std::uint64_t Str1, Str2;
			auto res = type0.CacheSize(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CacheSize());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " CacheSize: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CacheSize());
			}
		}

		{
			std::uint64_t Str1, Str2;
			auto res = type0.LogicalSize(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LogicalSize());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " LogicalSize: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LogicalSize());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.ExtendedSpeed(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExtendedSpeed());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " ExtendedSpeed: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExtendedSpeed());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.ExtendedConfiguredMemorySpeed(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExtendedConfiguredMemorySpeed());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type17: " << " ExtendedConfiguredMemorySpeed: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExtendedConfiguredMemorySpeed());
			}
		}
	}
}

TEST(SMBIOS, Type18)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type18());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type18: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.ErrorType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type18: " << " ErrorType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorType());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ErrorGranularity(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorGranularity());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type18: " << " ErrorGranularity: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorGranularity());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.ErrorOperation(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorOperation());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type18: " << " ErrorOperation: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorOperation());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.VendorSyndrome(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.VendorSyndrome());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type18: " << " VendorSyndrome: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.VendorSyndrome());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.MemoryArrayErrorAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryArrayErrorAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type18: " << " MemoryArrayErrorAddress: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryArrayErrorAddress());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.DeviceErrorAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceErrorAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type18: " << " DeviceErrorAddress: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceErrorAddress());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.ErrorResolution(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorResolution());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type18: " << " ErrorResolution: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorResolution());
			}
		}
	}
}

TEST(SMBIOS, Type19)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type19());
	for (auto& type0 : types)
	{
		{
			uint32_t Str1, Str2;
			auto res = type0.StartingAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.StartingAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type19: " << " StartingAddress: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.StartingAddress());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.EndingAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.EndingAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type19: " << " EndingAddress: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.EndingAddress());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.MemoryArrayHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryArrayHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type19: " << " MemoryArrayHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryArrayHandle());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.PartitionWidth(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PartitionWidth());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type19: " << " PartitionWidth: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PartitionWidth());
			}
		}

		{
			uint64_t Str1, Str2;
			auto res = type0.ExtentedStartingAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExtentedStartingAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type19: " << " ExtentedStartingAddress: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExtentedStartingAddress());
			}
		}

		{
			std::uint64_t Str1, Str2;
			auto res = type0.ExtentedEndingAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExtentedEndingAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type19: " << " ExtentedEndingAddress: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExtentedEndingAddress());
			}
		}
	}
}

TEST(SMBIOS, Type20)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type20());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type20: " << count++ << std::endl;
		{
			uint32_t Str1, Str2;
			auto res = type0.StartingAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.StartingAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type20: " << " StartingAddress: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.StartingAddress());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.EndingAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.EndingAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type20: " << " EndingAddress: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.EndingAddress());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.MemoryDeviceHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryDeviceHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type20: " << " MemoryDeviceHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryDeviceHandle());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.MemoryArrayMappedAddressHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryArrayMappedAddressHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type20: " << " MemoryArrayMappedAddressHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryArrayMappedAddressHandle());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.PartitionRowPosition(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PartitionRowPosition());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type20: " << " PartitionRowPosition: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PartitionRowPosition());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.InterleavePosition(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InterleavePosition());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type20: " << " InterleavePosition: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InterleavePosition());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.InterleaveDataDepth(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InterleaveDataDepth());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type20: " << " InterleaveDataDepth: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InterleaveDataDepth());
			}
		}

		{
			uint64_t Str1, Str2;
			auto res = type0.ExtentedStartingAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExtentedStartingAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type20: " << " ExtentedStartingAddress: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExtentedStartingAddress());
			}
		}

		{
			std::uint64_t Str1, Str2;
			auto res = type0.ExtentedEndingAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ExtentedEndingAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type20: " << " ExtentedEndingAddress: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ExtentedEndingAddress());
			}
		}
	}
}

TEST(SMBIOS, Type21)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type21());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type21: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.Types(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Types());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type21: " << " Types: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Types());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Interface(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Interface());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type21: " << " Interface: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Interface());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.NumOfButtons(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NumOfButtons());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type21: " << " NumOfButtons: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NumOfButtons());
			}
		}
	}
}

TEST(SMBIOS, Type22)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type22());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type22: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.Location(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Location());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " Types: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Location());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Manufacturer(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Manufacturer());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " Manufacturer: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Manufacturer());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ManufacturerDate(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ManufacturerDate());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " ManufacturerDate: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ManufacturerDate());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.SerialNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SerialNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " SerialNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SerialNumber());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.DeviceName(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceName());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " DeviceName: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceName());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.DeviceChemistry(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceChemistry());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " DeviceChemistry: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceChemistry());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.DesignCapacity(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DesignCapacity());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " DesignCapacity: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DesignCapacity());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.DesignVoltage(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DesignVoltage());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " DesignVoltage: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DesignVoltage());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.SBDSVersionNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SBDSVersionNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " SBDSVersionNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SBDSVersionNumber());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.MaximumErrorInBatteryData(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaximumErrorInBatteryData());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " MaximumErrorInBatteryData: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaximumErrorInBatteryData());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.SDBSSerialNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SDBSSerialNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " SDBSSerialNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SDBSSerialNumber());
			}
		}

		{
			Smbios::ManufacturerDateField Str1, Str2;
			auto res = type0.SDBSManufacturerDate(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SDBSManufacturerDate());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " SDBSManufacturerDate: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SDBSManufacturerDate());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.SDBSDeviceChemistry(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SDBSDeviceChemistry());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " SDBSDeviceChemistry: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SDBSDeviceChemistry());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.DesignCapacityMultiplier(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DesignCapacityMultiplier());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " DesignCapacityMultiplier: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DesignCapacityMultiplier());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.OEMspecific(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OEMspecific());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type22: " << " OEMspecific: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OEMspecific());
			}
		}
	}
}

TEST(SMBIOS, Type23)
{
	auto types = Smbios::GetType(Smbios::SmbiosHeaderType::Type23());
	for (auto& type0 : types)
	{
		{
			Smbios::SystemResetCapabilitiesField Str1, Str2;
			auto res = type0.Capabilities(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Capabilities());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type23: " << " Capabilities: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Capabilities());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ResetCount(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ResetCount());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type23: " << " ResetCount: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ResetCount());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ResetLimit(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ResetLimit());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type23: " << " ResetLimit: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ResetLimit());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.TimerInterval(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.TimerInterval());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type23: " << " TimerInterval: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.TimerInterval());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.Timeout(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Timeout());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type23: " << " Timeout: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Timeout());
			}
		}
	}
}

TEST(SMBIOS, Type24)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type24());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type24: " << count++ << std::endl;
		Smbios::HardwareSecurity Str1, Str2;
		auto res = type0.HardwareSecuritySettings(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.HardwareSecuritySettings());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type24: " << " HardwareSecuritySettings: " << Str1.Number + 0 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.HardwareSecuritySettings());
		}
	}
}

TEST(SMBIOS, Type25)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type25());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type25: " << count++ << std::endl;
		{
			uint8_t Str1, Str2;
			auto res = type0.NextScheduledPoweronMonth(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NextScheduledPoweronMonth());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type25: " << " NextScheduledPoweronMonth: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NextScheduledPoweronMonth());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.NextScheduledPoweronDay(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NextScheduledPoweronDay());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type25: " << " NextScheduledPoweronDay: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NextScheduledPoweronDay());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.NextScheduledPoweronHour(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NextScheduledPoweronHour());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type25: " << " NextScheduledPoweronHour: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NextScheduledPoweronHour());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.NextScheduledPoweronMinute(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NextScheduledPoweronMinute());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type25: " << " NextScheduledPoweronMinute: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NextScheduledPoweronMinute());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.NextScheduledPoweronSecond(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NextScheduledPoweronSecond());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type25: " << " NextScheduledPoweronSecond: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NextScheduledPoweronSecond());
			}
		}
	}
}

TEST(SMBIOS, Type26)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type26());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type26: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.Description(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Description());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type26: " << " Description: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Description());
			}
		}

		{
			Smbios::VoltageProbeStatusField Str1, Str2;
			auto res = type0.LocationAndStatus(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LocationAndStatus());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type26: " << " LocationAndStatus: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LocationAndStatus());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.MaximumValue(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaximumValue());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type26: " << " MaximumValue: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaximumValue());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.MinimumValue(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MinimumValue());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type26: " << " MinimumValue: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MinimumValue());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.Resolution(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Resolution());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type26: " << " Resolution: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Resolution());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.Tolerance(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Tolerance());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type26: " << " Tolerance: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Tolerance());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.Accuracy(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Accuracy());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type26: " << " Accuracy: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Accuracy());
			}
		}

		{
			uint32_t Str1, Str2;
			auto res = type0.OEMDefined(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OEMDefined());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type26: " << " OEMDefined: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OEMDefined());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.NominalValue(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NominalValue());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type26: " << " NominalValue: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NominalValue());
			}
		}
	}
}

TEST(SMBIOS, Type27)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type27());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type27: " << count++ << std::endl;

		{
			uint16_t Str1, Str2;
			auto res = type0.TemperatureProbeHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.TemperatureProbeHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type27: " << " TemperatureProbeHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.TemperatureProbeHandle());
			}
		}

		{
			Smbios::DeviceStatusField Str1, Str2;
			auto res = type0.DeviceTypeAndStatus(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceTypeAndStatus());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type27: " << " DeviceTypeAndStatus: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceTypeAndStatus());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.CoolingUnitGroup(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CoolingUnitGroup());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type27: " << " CoolingUnitGroup: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CoolingUnitGroup());
			}
		}

		{
			uint32_t Str1, Str2;
			auto res = type0.OEMDefined(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OEMDefined());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type27: " << " OEMDefined: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OEMDefined());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.NominalSpeed(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NominalSpeed());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type27: " << " NominalSpeed: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NominalSpeed());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Description(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Description());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type27: " << " Description: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Description());
			}
		}
	}
}

TEST(SMBIOS, Type28)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type28());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type28: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.Description(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Description());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type28: " << " Description: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Description());
			}
		}

		{
			Smbios::VoltageProbeStatusField Str1, Str2;
			auto res = type0.LocationAndStatus(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LocationAndStatus());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type28: " << " LocationAndStatus: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LocationAndStatus());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.MaximumValue(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaximumValue());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type28: " << " MaximumValue: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaximumValue());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.MinimumValue(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MinimumValue());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type28: " << " MinimumValue: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MinimumValue());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.Resolution(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Resolution());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type28: " << " Resolution: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Resolution());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.Tolerance(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Tolerance());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type28: " << " Tolerance: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Tolerance());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.Accuracy(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Accuracy());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type28: " << " Accuracy: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Accuracy());
			}
		}

		{
			uint32_t Str1, Str2;
			auto res = type0.OEMDefined(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OEMDefined());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type28: " << " OEMDefined: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OEMDefined());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.NominalValue(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NominalValue());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type28: " << " NominalValue: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NominalValue());
			}
		}
	}
}

TEST(SMBIOS, Type29)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type29());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type29: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.Description(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Description());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type29: " << " Description: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Description());
			}
		}

		{
			Smbios::VoltageProbeStatusField Str1, Str2;
			auto res = type0.LocationAndStatus(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LocationAndStatus());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type29: " << " LocationAndStatus: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LocationAndStatus());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.MaximumValue(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaximumValue());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type29: " << " MaximumValue: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaximumValue());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.MinimumValue(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MinimumValue());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type29: " << " MinimumValue: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MinimumValue());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.Resolution(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Resolution());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type29: " << " Resolution: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Resolution());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.Tolerance(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Tolerance());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type29: " << " Tolerance: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Tolerance());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.Accuracy(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Accuracy());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type29: " << " Accuracy: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Accuracy());
			}
		}

		{
			uint32_t Str1, Str2;
			auto res = type0.OEMDefined(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OEMDefined());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type29: " << " OEMDefined: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OEMDefined());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.NominalValue(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NominalValue());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type29: " << " NominalValue: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NominalValue());
			}
		}
	}
}

TEST(SMBIOS, Type30)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type30());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type30: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.ManufacturerName(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ManufacturerName());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type30: " << " ManufacturerName: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ManufacturerName());
			}
		}

		{
			Smbios::RemoteAccessConnectField Str1, Str2;
			auto res = type0.Connections(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Connections());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type30: " << " Connections: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Connections());
			}
		}
	}
}

TEST(SMBIOS, Type32)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type32());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type32: " << count++ << std::endl;
		std::vector<std::string> Str1, Str2;
		auto res = type0.BootStatus(Str1);
		if (res)
		{
			EXPECT_NO_THROW(Str2 = type0.BootStatus());
			EXPECT_EQ(Str1, Str2);
			std::cout << "Type32: " << " BootStatus: " << Str1 << std::endl;
		}
		else
		{
			EXPECT_ANY_THROW(Str2 = type0.BootStatus());
		}
	}
}

TEST(SMBIOS, Type33)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type33());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type33: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.ErrorType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type33: " << " ErrorType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorType());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ErrorGranularity(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorGranularity());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type33: " << " ErrorGranularity: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorGranularity());
			}
		}
		{
			std::string Str1, Str2;
			auto res = type0.ErrorOperation(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorOperation());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type33: " << " ErrorOperation: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorOperation());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.VendorSyndrome(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.VendorSyndrome());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type33: " << " VendorSyndrome: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.VendorSyndrome());
			}
		}

		{
			std::uint64_t Str1, Str2;
			auto res = type0.MemoryArrayErrorAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryArrayErrorAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type33: " << " MemoryArrayErrorAddress: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryArrayErrorAddress());
			}
		}

		{
			std::uint64_t Str1, Str2;
			auto res = type0.DeviceErrorAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceErrorAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type33: " << " DeviceErrorAddress: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceErrorAddress());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.ErrorResolution(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ErrorResolution());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type33: " << " ErrorResolution: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ErrorResolution());
			}
		}
	}
}

TEST(SMBIOS, Type34)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type34());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type34: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.Description(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Description());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type34: " << " Description: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Description());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Types(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Types());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type34: " << " Types: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Types());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.Address(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Address());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type34: " << " Address: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Address());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.AddressType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.AddressType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type34: " << " AddressType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.AddressType());
			}
		}
	}
}

TEST(SMBIOS, Type35)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type35());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type35: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.Description(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Description());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type35: " << " Description: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Description());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ManagementDeviceHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ManagementDeviceHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type35: " << " ManagementDeviceHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ManagementDeviceHandle());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ComponentHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ComponentHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type35: " << " ComponentHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ComponentHandle());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.ThresholdHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ThresholdHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type35: " << " ThresholdHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ThresholdHandle());
			}
		}
	}
}

TEST(SMBIOS, Type36)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type36());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type36: " << count++ << std::endl;
		{
			uint16_t Str1, Str2;
			auto res = type0.LowerThresholdNoncritical(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LowerThresholdNoncritical());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type36: " << " LowerThresholdNoncritical: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LowerThresholdNoncritical());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.UpperThresholdNoncritical(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.UpperThresholdNoncritical());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type36: " << " UpperThresholdNoncritical: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.UpperThresholdNoncritical());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.LowerThresholdCritical(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LowerThresholdCritical());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type36: " << " LowerThresholdCritical: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LowerThresholdCritical());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.UpperThresholdCritical(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.UpperThresholdCritical());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type36: " << " UpperThresholdCritical: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.UpperThresholdCritical());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.LowerThresholdNonrecoverable(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LowerThresholdNonrecoverable());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type36: " << " LowerThresholdNonrecoverable: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LowerThresholdNonrecoverable());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.UpperThresholdNonrecoverable(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.UpperThresholdNonrecoverable());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type36: " << " UpperThresholdNonrecoverable: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.UpperThresholdNonrecoverable());
			}
		}
	}
}

TEST(SMBIOS, Type37)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type37());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type37: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.ChannelType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ChannelType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type37: " << " ChannelType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ChannelType());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.MaximumChannelLoad(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaximumChannelLoad());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type37: " << " MaximumChannelLoad: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaximumChannelLoad());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.MemoryDeviceCount(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MemoryDeviceCount());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type37: " << " MemoryDeviceCount: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MemoryDeviceCount());
			}
		}

		{
			std::vector<Smbios::MemoryDeviceHandle> Str1, Str2;
			auto res = type0.Memory1Devices(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Memory1Devices());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type37: " << " Memory1Devices: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Memory1Devices());
			}
		}
	}
}

TEST(SMBIOS, Type38)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type38());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type38: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.InterfaceType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InterfaceType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type38: " << " InterfaceType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InterfaceType());
			}
		}

		{
			Smbios::IPMIRevisionField Str1, Str2;
			auto res = type0.IPMISpecRev(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.IPMISpecRev());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type38: " << " IPMISpecRev: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.IPMISpecRev());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.I2CSlaveAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.I2CSlaveAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type38: " << " I2CSlaveAddress: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.I2CSlaveAddress());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.NVStorageDeviceAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NVStorageDeviceAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type38: " << " NVStorageDeviceAddress: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NVStorageDeviceAddress());
			}
		}

		{
			std::uint64_t Str1, Str2;
			auto res = type0.BaseAddress(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.BaseAddress());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type38: " << " BaseAddress: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.BaseAddress());
			}
		}

		{
			Smbios::BaseAddrOrInterruptField Str1, Str2;
			auto res = type0.BaseAddressModInterrupt(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.BaseAddressModInterrupt());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type38: " << " BaseAddressModInterrupt: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.BaseAddressModInterrupt());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.InterruptNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InterruptNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type38: " << " InterruptNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InterruptNumber());
			}
		}
	}
}

TEST(SMBIOS, Type39)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type39());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type39: " << count++ << std::endl;
		{
			uint8_t Str1, Str2;
			auto res = type0.PowerUnitGroup(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PowerUnitGroup());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " PowerUnitGroup: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PowerUnitGroup());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Location(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Location());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " Location: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Location());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.DeviceName(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceName());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " DeviceName: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceName());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Manufacturer(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Manufacturer());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " Manufacturer: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Manufacturer());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.SerialNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SerialNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " SerialNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SerialNumber());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.AssetTagNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.AssetTagNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " AssetTagNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.AssetTagNumber());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ModelPartNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ModelPartNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " ModelPartNumber: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ModelPartNumber());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.RevisionLevel(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.RevisionLevel());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " RevisionLevel: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.RevisionLevel());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.MaxPowerCapacity(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MaxPowerCapacity());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " MaxPowerCapacity: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MaxPowerCapacity());
			}
		}

		{
			Smbios::PowerSupplyCharacteristicsField Str1, Str2;
			auto res = type0.PowerSupplyCharacteristics(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PowerSupplyCharacteristics());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " PowerSupplyCharacteristics: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PowerSupplyCharacteristics());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.InputVoltageProbeHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InputVoltageProbeHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " InputVoltageProbeHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InputVoltageProbeHandle());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.CoolingDeviceHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.CoolingDeviceHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " CoolingDeviceHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.CoolingDeviceHandle());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.InputCurrentProbeHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InputCurrentProbeHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type39: " << " InputCurrentProbeHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InputCurrentProbeHandle());
			}
		}
	}
}

TEST(SMBIOS, Type41)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type41());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type41: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.ReferenceDesignation(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ReferenceDesignation());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type41: " << " ReferenceDesignation: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ReferenceDesignation());
			}
		}

		{
			Smbios::ExtendedDeviceTypeField Str1, Str2;
			auto res = type0.DeviceType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type41: " << " DeviceType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceType());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.DeviceTypeInstance(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceTypeInstance());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type41: " << " DeviceTypeInstance: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceTypeInstance());
			}
		}

		{
			std::uint16_t Str1, Str2;
			auto res = type0.SegmentGroupNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SegmentGroupNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type41: " << " SegmentGroupNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SegmentGroupNumber());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.BusNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.BusNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type41: " << " BusNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.BusNumber());
			}
		}

		{
			Smbios::DeviceFuncField Str1, Str2;
			auto res = type0.DeviceFunctionNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DeviceFunctionNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type41: " << " DeviceFunctionNumber: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DeviceFunctionNumber());
			}
		}
	}
}

TEST(SMBIOS, Type40)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type40());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type40: " << count++ << std::endl;
		{
			std::uint8_t Str1, Str2;
			auto res = type0.NumofAdditionInfEntries(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NumofAdditionInfEntries());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type40: " << " NumofAdditionInfEntries: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NumofAdditionInfEntries());
			}
		}

		{
			std::vector<Smbios::AdditionalInformationEntry> Str1, Str2;
			auto res = type0.AdditionInfEntries(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.AdditionInfEntries());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type40: " << " AdditionInfEntries: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.AdditionInfEntries());
			}
		}
	}
}

TEST(SMBIOS, Type42)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type42());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type42: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.InterfaceType(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InterfaceType());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type42: " << " InterfaceType: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InterfaceType());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.InterfaceDataLength(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InterfaceDataLength());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type42: " << " InterfaceDataLength: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InterfaceDataLength());
			}
		}

		{
			std::vector<uint8_t>  Str1, Str2;
			auto res = type0.InterfaceData(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.InterfaceData());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type42: " << " InterfaceData: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.InterfaceData());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.NumberOfProtocol(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.NumberOfProtocol());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type42: " << " NumberOfProtocol: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.NumberOfProtocol());
			}
		}

		{
			std::vector<Smbios::ProtocolRecordDataField> Str1, Str2;
			auto res = type0.ProtocolRecords(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ProtocolRecords());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type42: " << " ProtocolRecords: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ProtocolRecords());
			}
		}
	}
}

TEST(SMBIOS, Type43)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type43());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type43: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.VendorID(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.VendorID());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type43: " << " VendorID: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.VendorID());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.MajorSpecVersion(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MajorSpecVersion());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type43: " << " MajorSpecVersion: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MajorSpecVersion());
			}
		}

		{
			std::uint8_t Str1, Str2;
			auto res = type0.MinorSpecVersion(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MinorSpecVersion());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type43: " << " MinorSpecVersion: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MinorSpecVersion());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.FirmwareVersion1(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.FirmwareVersion1());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type43: " << " FirmwareVersion1: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.FirmwareVersion1());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.FirmwareVersion2(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.FirmwareVersion2());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type43: " << " FirmwareVersion2: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.FirmwareVersion2());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Description(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Description());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type43: " << " Description: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Description());
			}
		}

		{
			Smbios::TPMDeviceCharacteristicsField Str1, Str2;
			auto res = type0.Characteristics(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Characteristics());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type43: " << " Characteristics: " << Str1.Number + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Characteristics());
			}
		}

		{
			std::uint32_t Str1, Str2;
			auto res = type0.OEMDefined(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OEMDefined());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type43: " << " OEMDefined: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OEMDefined());
			}
		}
	}
}

TEST(SMBIOS, Type44)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type44());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type44: " << count++ << std::endl;
		{
			uint16_t Str1, Str2;
			auto res = type0.ReferencedHandle(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ReferencedHandle());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type44: " << " ReferencedHandle: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ReferencedHandle());
			}
		}

		{
			std::vector<Smbios::ProcessorSpecificField> Str1, Str2;
			auto res = type0.ProcessorSpecificBlock(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ProcessorSpecificBlock());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type44: " << " ProcessorSpecificBlock: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ProcessorSpecificBlock());
			}
		}
	}
}

TEST(SMBIOS, Type131)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type131());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type131: " << count++ << std::endl;
		{
			uint8_t Str1, Str2;
			auto res = type0.Version(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Version());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type131: " << " Version: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Version());
			}
		}

		{
			std::vector<uint8_t> Str1, Str2;
			auto res = type0.TVTStructure(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.TVTStructure());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type131: " << " TVTStructure: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.TVTStructure());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Description(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Description());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type131: " << " Description: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Description());
			}
		}
	}
}

TEST(SMBIOS, Type133)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type133());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type133: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.OEMData(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OEMData());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type133: " << " OEMData: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OEMData());
			}
		}
	}
}

TEST(SMBIOS, Type134)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type134());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type134: " << count++ << std::endl;
		{
			uint8_t Str1, Str2;
			auto res = type0.Version(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Version());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type134: " << " Version: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Version());
			}
		}

		{
			uint32_t Str1, Str2;
			auto res = type0.TCPA_CAP_PROPERTY(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.TCPA_CAP_PROPERTY());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type134: " << " TCPA_CAP_PROPERTY: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.TCPA_CAP_PROPERTY());
			}
		}

		{
			uint32_t Str1, Str2;
			auto res = type0.TCPA_CAP_VERSION(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.TCPA_CAP_VERSION());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type134: " << " TCPA_CAP_VERSION: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.TCPA_CAP_VERSION());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.TPMState(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.TPMState());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type134: " << " TPMState: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.TPMState());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.Description(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Description());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type134: " << " Description: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Description());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ReservedString(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ReservedString());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type134: " << " ReservedString: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ReservedString());
			}
		}
	}
}

TEST(SMBIOS, Type140)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type140());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type140: " << count++ << std::endl;
		{
			std::string Str1, Str2;
			auto res = type0.Signature(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Signature());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type140: " << " Signature: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Signature());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.OEMStructOffset(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OEMStructOffset());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type140: " << " OEMStructOffset: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OEMStructOffset());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.OEMStructNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OEMStructNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type140: " << " OEMStructNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OEMStructNumber());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.OEMStructRevision(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OEMStructRevision());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type140: " << " OEMStructRevision: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OEMStructRevision());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ECPVersionID(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ECPVersionID());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type140: " << " ECPVersionID: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ECPVersionID());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.ECPReleaseDate(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ECPReleaseDate());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type140: " << " ECPReleaseDate: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ECPReleaseDate());
			}
		}
	}
}

TEST(SMBIOS, Type170)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type170());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type170: " << count++ << std::endl;
		{
			uint16_t Str1, Str2;
			auto res = type0.Capability(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.Capability());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type170: " << " Capability: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.Capability());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.ApplicationButton(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ApplicationButton());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type170: " << " ApplicationButton: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ApplicationButton());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.MediaButton(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MediaButton());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type170: " << " MediaButton: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MediaButton());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.DisplayButton(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.DisplayButton());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type170: " << " DisplayButton: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.DisplayButton());
			}
		}

		{
			uint16_t Str1, Str2;
			auto res = type0.OthersButton(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.OthersButton());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type170: " << " OthersButton: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.OthersButton());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.FunctionKeyNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.FunctionKeyNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type170: " << " FunctionKeyNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.FunctionKeyNumber());
			}
		}
	}
}

TEST(SMBIOS, Type200)
{
	auto type = Smbios::GetType(Smbios::SmbiosHeaderType::Type200());
	auto count = 0;
	for (const auto& type0 : type)
	{
		std::cout << "Type200: " << count++ << std::endl;

		{
			std::string Str1, Str2;
			auto res = type0.ID(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.ID());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " ID: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.ID());
			}
		}

		{
			std::string Str1, Str2;
			auto res = type0.MTM(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.MTM());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " MTM: " << Str1 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.MTM());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.BIOSSpecVersionMajorNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.BIOSSpecVersionMajorNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " BIOSSpecVersionMajorNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.BIOSSpecVersionMajorNumber());
			}
		}
		{
			uint8_t Str1, Str2;
			auto res = type0.BIOSSpecVersionMinorNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.BIOSSpecVersionMinorNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " BIOSSpecVersionMinorNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.BIOSSpecVersionMinorNumber());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.EEPROMSpecVersionMajorNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.EEPROMSpecVersionMajorNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " EEPROMSpecVersionMajorNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.EEPROMSpecVersionMajorNumber());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.EEPROMSpecVersionMinorNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.EEPROMSpecVersionMinorNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " EEPROMSpecVersionMinorNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.EEPROMSpecVersionMinorNumber());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.PMSpecVersionMajorNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PMSpecVersionMajorNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " PMSpecVersionMajorNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PMSpecVersionMajorNumber());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.PMSpecVersionMinorNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.PMSpecVersionMinorNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " PMSpecVersionMinorNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.PMSpecVersionMinorNumber());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.SMIIoPort(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SMIIoPort());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " SMIIoPort: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SMIIoPort());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.SoftwareSMITriggerValue(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.SoftwareSMITriggerValue());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " SoftwareSMITriggerValue: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.SoftwareSMITriggerValue());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.LENOVOSpecVersionMajorNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LENOVOSpecVersionMajorNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " LENOVOSpecVersionMajorNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LENOVOSpecVersionMajorNumber());
			}
		}

		{
			uint8_t Str1, Str2;
			auto res = type0.LENOVOSpecVersionMinorNumber(Str1);
			if (res)
			{
				EXPECT_NO_THROW(Str2 = type0.LENOVOSpecVersionMinorNumber());
				EXPECT_EQ(Str1, Str2);
				std::cout << "Type200: " << " LENOVOSpecVersionMinorNumber: " << Str1 + 0 << std::endl;
			}
			else
			{
				EXPECT_ANY_THROW(Str2 = type0.LENOVOSpecVersionMinorNumber());
			}
		}
	}
}

int _tmain(int argc, TCHAR* argv[])
{
	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}