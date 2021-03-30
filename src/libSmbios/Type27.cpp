#include "stdafx.h"
#include "Type27.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto TemperatureProbeHandle = 0x04;
	constexpr auto DeviceTypeAndStatus = 0x06;
	constexpr auto CoolingUnitGroup = 0x07;
	constexpr auto OEMDefined = 0x08;
	constexpr auto NominalSpeed = 0x0C;
	constexpr auto Description = 0x0E;
}

namespace
{
	class Type27Impl : public Smbios::TypeImpl
	{
	public:
		Type27Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(TemperatureProbeHandle),std::make_pair(2,2)},
				{GetSmbiosType(DeviceTypeAndStatus),std::make_pair(2,2)},
				{GetSmbiosType(CoolingUnitGroup),std::make_pair(2,2)},
				{GetSmbiosType(OEMDefined),std::make_pair(2,2)},
				{GetSmbiosType(NominalSpeed),std::make_pair(2,2)},
				{GetSmbiosType(Description),std::make_pair(2,7)}
			};
		}
		~Type27Impl() = default;
		Type27Impl(const Type27Impl&) = delete;
		Type27Impl& operator=(const Type27Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type27";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(27, TableNum);
		}
	};
}

Smbios::Type27::Type27(const int32_t& TableNum) : pImpl(std::make_unique<Type27Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type27::Type27(void* PhysicalAddress) : pImpl(std::make_unique<Type27Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type27::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type27::~Type27() = default;

Smbios::Type27::Type27(Type27&&) = default;

Smbios::Type27& Smbios::Type27::operator=(Type27&&) = default;

uint16_t Smbios::Type27::TemperatureProbeHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::TemperatureProbeHandle, Type27Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type27::TemperatureProbeHandle(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::TemperatureProbeHandle, Type27Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::DeviceStatusField Smbios::Type27::DeviceTypeAndStatus() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::DeviceStatusField>
		(*pImpl, ::DeviceTypeAndStatus, Type27Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::DeviceStatusField>(ret);
	});
}

bool Smbios::Type27::DeviceTypeAndStatus(Smbios::DeviceStatusField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::DeviceTypeAndStatus, Type27Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::DeviceStatusField>(ret);
	}, Str);
}

std::uint8_t Smbios::Type27::CoolingUnitGroup() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::CoolingUnitGroup, Type27Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type27::CoolingUnitGroup(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::CoolingUnitGroup, Type27Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint32_t Smbios::Type27::OEMDefined() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::OEMDefined, Type27Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type27::OEMDefined(uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMDefined, Type27Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type27::NominalSpeed() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::NominalSpeed, Type27Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type27::NominalSpeed(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NominalSpeed, Type27Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type27::Description() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Description, Type27Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type27::Description(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Description, Type27Impl::GetFuncRealName(__FUNCTION__), Str);
}