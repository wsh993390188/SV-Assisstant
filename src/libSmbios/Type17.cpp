#include "stdafx.h"

#include "Type17.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto PhysicalMemoryArrayHandle = 0x04;
	constexpr auto MemoryErrorInformationHandle = 0x06;
	constexpr auto MemoryTotalWidth = 0x08;
	constexpr auto Datawidth = 0x0A;
	constexpr auto Size = 0x0C;
	constexpr auto FormFactor = 0x0E;
	constexpr auto DeviceSet = 0x0F;
	constexpr auto DeviceLocator = 0x10;
	constexpr auto BankLocator = 0x11;
	constexpr auto MemoryType = 0x12;
	constexpr auto TypeDetail = 0x13;
	constexpr auto Speed = 0x15;
	constexpr auto Manufacturer = 0x17;
	constexpr auto SerialNumber = 0x18;
	constexpr auto AssetTag = 0x19;
	constexpr auto PartNumber = 0x1A;
	constexpr auto Attributes = 0x1B;
	constexpr auto ExtentedSize = 0x1C;
	constexpr auto ConfiguredMemoryClockSpeed = 0x20;
	constexpr auto MinimumVoltage = 0x22;
	constexpr auto MaximumVoltage = 0x24;
	constexpr auto ConfiguredVoltage = 0x26;
	constexpr auto MemoryTechnology = 0x28;
	constexpr auto MemoryOperatingModeCapability = 0x29;
	constexpr auto FirmwareVersion = 0x2B;
	constexpr auto ModuleManufacturerID = 0x2C;
	constexpr auto ModuleProductID = 0x2E;
	constexpr auto MemorySubsystemControllerManufacturerID = 0x30;
	constexpr auto MemorySubsystemControllerProductID = 0x32;
	constexpr auto NonVolatileSize = 0x34;
	constexpr auto VolatileSize = 0x3C;
	constexpr auto CacheSize = 0x44;
	constexpr auto LogicalSize = 0x4C;
	constexpr auto ExtendedSpeed = 0x54;
	constexpr auto ExtendedConfiguredMemorySpeed = 0x58;

	const std::string FormFactorStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("SIMM"),
		("SIP"),
		("Chip"),
		("DIP"),
		("ZIP"),
		("Proprietary Card"),
		("DIMM"),
		("TSOP"),
		("Row of chips"),
		("RIMM"),
		("SODIMM"),
		("SRIMM"),
		("FB-DIMM"),
		("Die"),
	};

	const std::string MemoryDeviceTypeStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("DRAM"),
		("EDRAM"),
		("VRAM"),
		("SRAM"),
		("RAM"),
		("ROM"),
		("FLASH"),
		("EEPROM"),
		("FEPROM"),
		("EPROM"),
		("CDRAM"),
		("3DRAM"),
		("SDRAM"),
		("SGRAM"),
		("RDRAM"),
		("DDR"),
		("DDR2"),
		("DDR2 FB-DIMM"),
		{},
		{},
		{},
		("DDR3"),
		("FBD2"),
		("DDR4"),
		("LPDDR"),
		("LPDDR2"),
		("LPDDR3"),
		("LPDDR4"),
		{"Logical non-volatile device"},
		{"HBM (High Bandwidth Memory)"},
		{"HBM2 (High Bandwidth Memory Generation 2)"}
	};

	const std::string MemoryTechnologyStrs[] =
	{
		{},
		u8"Other",
		u8"Unknown",
		u8"DRAM",
		u8"NVDIMM - N",
		u8"NVDIMM - F",
		u8"NVDIMM - P",
		u8"Intel® Optane™ DC Persistent Memory"
	};
}

namespace
{
	class Type17Impl : public Smbios::TypeImpl
	{
	public:
		Type17Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(PhysicalMemoryArrayHandle),std::make_pair(2,1)},
				{GetSmbiosType(MemoryErrorInformationHandle),std::make_pair(2,1)},
				{GetSmbiosType(MemoryTotalWidth),std::make_pair(2,1)},
				{GetSmbiosType(Datawidth),std::make_pair(2,1)},
				{GetSmbiosType(Size),std::make_pair(2,1)},
				{GetSmbiosType(FormFactor),std::make_pair(2,1)},
				{GetSmbiosType(DeviceSet),std::make_pair(2,1)},
				{GetSmbiosType(DeviceLocator),std::make_pair(2,1)},
				{GetSmbiosType(BankLocator),std::make_pair(2,1)},
				{GetSmbiosType(MemoryType),std::make_pair(2,1)},
				{GetSmbiosType(TypeDetail),std::make_pair(2,1)},
				{GetSmbiosType(Speed),std::make_pair(2,3)},
				{GetSmbiosType(Manufacturer),std::make_pair(2,3)},
				{GetSmbiosType(SerialNumber),std::make_pair(2,3)},
				{GetSmbiosType(AssetTag),std::make_pair(2,3)},
				{GetSmbiosType(PartNumber),std::make_pair(2,3)},
				{GetSmbiosType(Attributes),std::make_pair(2,6)},
				{GetSmbiosType(ExtentedSize),std::make_pair(2,7)},
				{GetSmbiosType(ConfiguredMemoryClockSpeed),std::make_pair(2,7)},
				{GetSmbiosType(MinimumVoltage),std::make_pair(2,8)},
				{GetSmbiosType(MaximumVoltage),std::make_pair(2,8)},
				{GetSmbiosType(ConfiguredVoltage),std::make_pair(2,8)},
				{GetSmbiosType(MemoryTechnology),std::make_pair(3,2)},
				{GetSmbiosType(MemoryOperatingModeCapability),std::make_pair(3,2)},
				{GetSmbiosType(FirmwareVersion),std::make_pair(3,2)},
				{GetSmbiosType(ModuleManufacturerID),std::make_pair(3,2)},
				{GetSmbiosType(ModuleProductID),std::make_pair(3,2)},
				{GetSmbiosType(MemorySubsystemControllerManufacturerID),std::make_pair(3,2)},
				{GetSmbiosType(MemorySubsystemControllerProductID),std::make_pair(3,2)},
				{GetSmbiosType(NonVolatileSize),std::make_pair(3,2)},
				{GetSmbiosType(VolatileSize),std::make_pair(3,2)},
				{GetSmbiosType(CacheSize),std::make_pair(3,2)},
				{GetSmbiosType(LogicalSize),std::make_pair(3,2)},
				{GetSmbiosType(ExtendedSpeed),std::make_pair(3,3)},
				{GetSmbiosType(ExtendedConfiguredMemorySpeed),std::make_pair(3,3)}
			};
		}
		~Type17Impl() = default;
		Type17Impl(const Type17Impl&) = delete;
		Type17Impl& operator=(const Type17Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}

		std::string GetType()
		{
			return "Type17";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(17, TableNum);
		}
	};
}

Smbios::Type17::Type17(const int32_t& TableNum) : pImpl(std::make_unique<Type17Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type17::Type17(void* PhysicalAddress) : pImpl(std::make_unique<Type17Impl>())
{
	pImpl->Create(PhysicalAddress);
}

Smbios::Type17::~Type17() = default;

Smbios::Type17::Type17(Type17&&) = default;

Smbios::Type17& Smbios::Type17::operator=(Type17&&) = default;

bool Smbios::Type17::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

std::uint16_t Smbios::Type17::PhysicalMemoryArrayHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::PhysicalMemoryArrayHandle, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::PhysicalMemoryArrayHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::PhysicalMemoryArrayHandle, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::MemoryErrorInformationHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::MemoryErrorInformationHandle, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::MemoryErrorInformationHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemoryErrorInformationHandle, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::MemoryTotalWidth() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::MemoryTotalWidth, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::MemoryTotalWidth(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemoryTotalWidth, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::Datawidth() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::Datawidth, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::Datawidth(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Datawidth, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::Size() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::Size, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::Size(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Size, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type17::FormFactor() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::FormFactor, Type17Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(FormFactorStrs))
		{
			return FormFactorStrs[num];
		}
		else
		{
			return std::string();
		}
	});
}

bool Smbios::Type17::FormFactor(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::FormFactor, Type17Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
	{
		if (num < _countof(FormFactorStrs))
		{
			return FormFactorStrs[num];
		}
		else
		{
			return std::string();
		}
	}, Str);
}

std::uint8_t Smbios::Type17::DeviceSet() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::DeviceSet, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::DeviceSet(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DeviceSet, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type17::DeviceLocator() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::DeviceLocator, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::DeviceLocator(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DeviceLocator, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type17::BankLocator() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::BankLocator, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::BankLocator(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::BankLocator, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type17::MemoryType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::MemoryType, Type17Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(MemoryDeviceTypeStrs))
		{
			return MemoryDeviceTypeStrs[num];
		}
		else
		{
			return std::string();
		}
	});
}

bool Smbios::Type17::MemoryType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::MemoryType, Type17Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
	{
		if (num < _countof(MemoryDeviceTypeStrs))
		{
			return MemoryDeviceTypeStrs[num];
		}
		else
		{
			return std::string();
		}
	}, Str);
}

Smbios::MemoryTypeDetailField Smbios::Type17::TypeDetail() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, Smbios::MemoryTypeDetailField>
		(*pImpl, ::TypeDetail, Type17Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& num)
	{
		return static_cast<Smbios::MemoryTypeDetailField>(num);
	});
}

bool Smbios::Type17::TypeDetail(Smbios::MemoryTypeDetailField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>
		(*pImpl, ::TypeDetail, Type17Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& num)
	{
		return static_cast<Smbios::MemoryTypeDetailField>(num);
	}, Str);
}

std::uint16_t Smbios::Type17::Speed() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Speed, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::Speed(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Speed, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type17::Manufacturer() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Manufacturer, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::Manufacturer(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Manufacturer, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type17::SerialNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::SerialNumber, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::SerialNumber(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SerialNumber, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type17::AssetTag() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::AssetTag, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::AssetTag(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::AssetTag, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type17::PartNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::PartNumber, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::PartNumber(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::PartNumber, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::MemoryAttributeField Smbios::Type17::Attributes() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, Smbios::MemoryAttributeField>
		(*pImpl, ::Attributes, Type17Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		return static_cast<Smbios::MemoryAttributeField>(num);
	});
}

bool Smbios::Type17::Attributes(Smbios::MemoryAttributeField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::Attributes, Type17Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		return static_cast<Smbios::MemoryAttributeField>(num);
	}, Str);
}

std::uint32_t Smbios::Type17::ExtentedSize() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t>
		(*pImpl, ::ExtentedSize, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::ExtentedSize(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ExtentedSize, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::ConfiguredMemoryClockSpeed() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::ConfiguredMemoryClockSpeed, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::ConfiguredMemoryClockSpeed(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ConfiguredMemoryClockSpeed, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::MinimumVoltage() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::MinimumVoltage, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::MinimumVoltage(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MinimumVoltage, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::MaximumVoltage() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::MaximumVoltage, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::MaximumVoltage(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MaximumVoltage, Type17Impl::GetFuncRealName(__FUNCTION__), Str);;
}

std::uint16_t Smbios::Type17::ConfiguredVoltage() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::ConfiguredVoltage, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::ConfiguredVoltage(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ConfiguredVoltage, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type17::MemoryTechnology() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::MemoryTechnology, Type17Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(MemoryTechnologyStrs))
			return MemoryTechnologyStrs[num];
		else
			return std::string();
	});
}

bool Smbios::Type17::MemoryTechnology(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::MemoryTechnology, Type17Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
	{
		if (num < _countof(MemoryTechnologyStrs))
			return MemoryTechnologyStrs[num];
		else
			return std::string();
	}, Str);
}

Smbios::MemoryModeCapability Smbios::Type17::MemoryOperatingModeCapability() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, Smbios::MemoryModeCapability>
		(*pImpl, ::MemoryOperatingModeCapability, Type17Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& num)
	{
		return static_cast<Smbios::MemoryModeCapability>(num);
	});
}

bool Smbios::Type17::MemoryOperatingModeCapability(Smbios::MemoryModeCapability & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>
		(*pImpl, ::MemoryOperatingModeCapability, Type17Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& num)
	{
		return static_cast<Smbios::MemoryModeCapability>(num);
	}, Str);
}

std::string Smbios::Type17::FirmwareVersion() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::FirmwareVersion, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::FirmwareVersion(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::FirmwareVersion, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::ModuleManufacturerID() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::ModuleManufacturerID, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::ModuleManufacturerID(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ModuleManufacturerID, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::ModuleProductID() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::ModuleProductID, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::ModuleProductID(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ModuleProductID, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::MemorySubsystemControllerManufacturerID() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::MemorySubsystemControllerManufacturerID, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::MemorySubsystemControllerManufacturerID(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemorySubsystemControllerManufacturerID, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type17::MemorySubsystemControllerProductID() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::MemorySubsystemControllerProductID, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::MemorySubsystemControllerProductID(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemorySubsystemControllerProductID, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint64_t Smbios::Type17::NonVolatileSize() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>
		(*pImpl, ::NonVolatileSize, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::NonVolatileSize(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NonVolatileSize, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint64_t Smbios::Type17::VolatileSize() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>
		(*pImpl, ::VolatileSize, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::VolatileSize(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::VolatileSize, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint64_t Smbios::Type17::CacheSize() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>
		(*pImpl, ::CacheSize, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::CacheSize(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::CacheSize, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint64_t Smbios::Type17::LogicalSize() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>
		(*pImpl, ::LogicalSize, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::LogicalSize(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LogicalSize, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type17::ExtendedSpeed() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::ExtendedSpeed, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::ExtendedSpeed(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ExtendedSpeed, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type17::ExtendedConfiguredMemorySpeed() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::ExtendedConfiguredMemorySpeed, Type17Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type17::ExtendedConfiguredMemorySpeed(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ExtendedConfiguredMemorySpeed, Type17Impl::GetFuncRealName(__FUNCTION__), Str);
}