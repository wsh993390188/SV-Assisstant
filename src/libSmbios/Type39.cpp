#include "stdafx.h"
#include "Type39.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto PowerUnitGroup = 0x04;
	constexpr auto Location = 0x05;
	constexpr auto DeviceName = 0x06;
	constexpr auto Manufacturer = 0x07;
	constexpr auto SerialNumber = 0x08;
	constexpr auto AssetTagNumber = 0x09;
	constexpr auto ModelPartNumber = 0x0A;
	constexpr auto RevisionLevel = 0x0B;
	constexpr auto MaxPowerCapacity = 0x0C;
	constexpr auto PowerSupplyCharacteristics = 0x0E;
	constexpr auto InputVoltageProbeHandle = 0x10;
	constexpr auto CoolingDeviceHandle = 0x12;
	constexpr auto InputCurrentProbeHandle = 0x14;
}

namespace
{
	class Type39Impl : public Smbios::TypeImpl
	{
	public:
		Type39Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(PowerUnitGroup),std::make_pair(0,0)},
				{GetSmbiosType(Location),std::make_pair(0,0)},
				{GetSmbiosType(DeviceName),std::make_pair(0,0)},
				{GetSmbiosType(Manufacturer),std::make_pair(0,0)},
				{GetSmbiosType(SerialNumber),std::make_pair(0,0)},
				{GetSmbiosType(AssetTagNumber),std::make_pair(0,0)},
				{GetSmbiosType(ModelPartNumber),std::make_pair(0,0)},
				{GetSmbiosType(RevisionLevel),std::make_pair(0,0)},
				{GetSmbiosType(MaxPowerCapacity),std::make_pair(0,0)},
				{GetSmbiosType(PowerSupplyCharacteristics),std::make_pair(0,0)},
				{GetSmbiosType(InputVoltageProbeHandle),std::make_pair(0,0)},
				{GetSmbiosType(CoolingDeviceHandle),std::make_pair(0,0)},
				{GetSmbiosType(InputCurrentProbeHandle),std::make_pair(0,0)},
			};
		}
		~Type39Impl() = default;
		Type39Impl(const Type39Impl&) = delete;
		Type39Impl& operator=(const Type39Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type39";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(39, TableNum);
		}
	};
}

Smbios::Type39::Type39(const int32_t& TableNum) : pImpl(std::make_unique<Type39Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type39::Type39(void* PhysicalAddress) : pImpl(std::make_unique<Type39Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type39::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type39::~Type39() = default;

Smbios::Type39::Type39(Type39&&) = default;

Smbios::Type39& Smbios::Type39::operator=(Type39&&) = default;

std::uint8_t Smbios::Type39::PowerUnitGroup() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::PowerUnitGroup, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::PowerUnitGroup(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::PowerUnitGroup, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type39::Location() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Location, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::Location(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Location, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type39::DeviceName() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::DeviceName, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::DeviceName(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DeviceName, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type39::Manufacturer() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Manufacturer, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::Manufacturer(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Manufacturer, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type39::SerialNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::SerialNumber, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::SerialNumber(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SerialNumber, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type39::AssetTagNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::AssetTagNumber, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::AssetTagNumber(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::AssetTagNumber, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type39::ModelPartNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::ModelPartNumber, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::ModelPartNumber(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ModelPartNumber, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type39::RevisionLevel() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::RevisionLevel, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::RevisionLevel(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::RevisionLevel, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type39::MaxPowerCapacity() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::MaxPowerCapacity, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::MaxPowerCapacity(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MaxPowerCapacity, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::PowerSupplyCharacteristicsField Smbios::Type39::PowerSupplyCharacteristics() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t, Smbios::PowerSupplyCharacteristicsField>
		(*pImpl, ::PowerSupplyCharacteristics, Type39Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& num)
	{
		return static_cast<Smbios::PowerSupplyCharacteristicsField>(num);
	});
}

bool Smbios::Type39::PowerSupplyCharacteristics(Smbios::PowerSupplyCharacteristicsField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t>
		(*pImpl, ::PowerSupplyCharacteristics, Type39Impl::GetFuncRealName(__FUNCTION__),
			[](const uint16_t& num)
	{
		return static_cast<Smbios::PowerSupplyCharacteristicsField>(num);
	}, Str);
}

std::uint16_t Smbios::Type39::InputVoltageProbeHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::InputVoltageProbeHandle, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::InputVoltageProbeHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::InputVoltageProbeHandle, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type39::CoolingDeviceHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::CoolingDeviceHandle, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::CoolingDeviceHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::CoolingDeviceHandle, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type39::InputCurrentProbeHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::InputCurrentProbeHandle, Type39Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type39::InputCurrentProbeHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::InputCurrentProbeHandle, Type39Impl::GetFuncRealName(__FUNCTION__), Str);
}