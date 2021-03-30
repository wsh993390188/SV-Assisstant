#include "stdafx.h"
#include "Type22.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Location = 0x04;
	constexpr auto Manufacturer = 0x05;
	constexpr auto ManufacturerDate = 0x06;
	constexpr auto SerialNumber = 0x07;
	constexpr auto DeviceName = 0x08;
	constexpr auto DeviceChemistry = 0x09;
	constexpr auto DesignCapacity = 0x0A;
	constexpr auto DesignVoltage = 0x0C;
	constexpr auto SBDSVersionNumber = 0x0E;
	constexpr auto MaximumErrorInBatteryData = 0x0F;
	constexpr auto SDBSSerialNumber = 0x10;
	constexpr auto SDBSManufacturerDate = 0x12;
	constexpr auto SDBSDeviceChemistry = 0x14;
	constexpr auto DesignCapacityMultiplier = 0x15;
	constexpr auto OEMspecific = 0x16;

	const std::string DeviceChemistryStrs[] =
	{
		{},
		"Other",
		"Unknown",
		"Lead Acid",
		"Nickel",
		"Nickel metal hydride",
		"Lithium - ion",
		"Zinc air",
		"Lithium Polymer"
	};
}

namespace
{
	class Type22Impl : public Smbios::TypeImpl
	{
	public:
		Type22Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Location),std::make_pair(2,1)},
				{GetSmbiosType(Manufacturer),std::make_pair(2,1)},
				{GetSmbiosType(ManufacturerDate),std::make_pair(2,1)},
				{GetSmbiosType(SerialNumber),std::make_pair(2,1)},
				{GetSmbiosType(DeviceName),std::make_pair(2,1)},
				{GetSmbiosType(DeviceChemistry),std::make_pair(2,1)},
				{GetSmbiosType(DesignCapacity),std::make_pair(2,1)},
				{GetSmbiosType(DesignVoltage),std::make_pair(2,1)},
				{GetSmbiosType(SBDSVersionNumber),std::make_pair(2,1)},
				{GetSmbiosType(MaximumErrorInBatteryData),std::make_pair(2,1)},
				{GetSmbiosType(SDBSSerialNumber),std::make_pair(2,2)},
				{GetSmbiosType(SDBSManufacturerDate),std::make_pair(2,2)},
				{GetSmbiosType(SDBSDeviceChemistry),std::make_pair(2,2)},
				{GetSmbiosType(DesignCapacityMultiplier),std::make_pair(2,2)},
				{GetSmbiosType(OEMspecific),std::make_pair(2,2)}
			};
		}
		~Type22Impl() = default;
		Type22Impl(const Type22Impl&) = delete;
		Type22Impl& operator=(const Type22Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}

		std::string GetType()
		{
			return "Type22";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(22, TableNum);
		}
	};
}

Smbios::Type22::Type22(const int32_t& TableNum) : pImpl(std::make_unique<Type22Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type22::Type22(void* PhysicalAddress) : pImpl(std::make_unique<Type22Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type22::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type22::~Type22() = default;

Smbios::Type22::Type22(Type22&&) = default;

Smbios::Type22& Smbios::Type22::operator=(Type22&&) = default;

std::string Smbios::Type22::Location() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Location, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::Location(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Location, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type22::Manufacturer() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Manufacturer, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::Manufacturer(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Manufacturer, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type22::ManufacturerDate() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::ManufacturerDate, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::ManufacturerDate(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ManufacturerDate, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type22::SerialNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::SerialNumber, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::SerialNumber(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SerialNumber, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type22::DeviceName() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::DeviceName, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::DeviceName(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DeviceName, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type22::DeviceChemistry() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::DeviceChemistry, Type22Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(DeviceChemistryStrs))
		{
			return DeviceChemistryStrs[num];
		}
		return std::string();
	});
}

bool Smbios::Type22::DeviceChemistry(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::DeviceChemistry, Type22Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
	{
		if (num < _countof(DeviceChemistryStrs))
		{
			return DeviceChemistryStrs[num];
		}
		return std::string();
	}, Str);
}

std::uint16_t Smbios::Type22::DesignCapacity() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::DesignCapacity, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::DesignCapacity(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DesignCapacity, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type22::DesignVoltage() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::DesignVoltage, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::DesignVoltage(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DesignVoltage, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type22::SBDSVersionNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::SBDSVersionNumber, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::SBDSVersionNumber(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SBDSVersionNumber, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type22::MaximumErrorInBatteryData() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::MaximumErrorInBatteryData, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::MaximumErrorInBatteryData(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MaximumErrorInBatteryData, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type22::SDBSSerialNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::SDBSSerialNumber, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::SDBSSerialNumber(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SDBSSerialNumber, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::ManufacturerDateField Smbios::Type22::SDBSManufacturerDate() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t, Smbios::ManufacturerDateField>
		(*pImpl, ::SDBSManufacturerDate, Type22Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& num)
	{
		return static_cast<Smbios::ManufacturerDateField>(num);
	});
}

bool Smbios::Type22::SDBSManufacturerDate(Smbios::ManufacturerDateField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>
		(*pImpl, ::SDBSManufacturerDate, Type22Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& num)
	{
		return static_cast<Smbios::ManufacturerDateField>(num);
	}, Str);
}

std::string Smbios::Type22::SDBSDeviceChemistry() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::SDBSDeviceChemistry, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::SDBSDeviceChemistry(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SDBSDeviceChemistry, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type22::DesignCapacityMultiplier() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::DesignCapacityMultiplier, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::DesignCapacityMultiplier(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DesignCapacityMultiplier, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type22::OEMspecific() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::OEMspecific, Type22Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type22::OEMspecific(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMspecific, Type22Impl::GetFuncRealName(__FUNCTION__), Str);
}