#include "stdafx.h"
#include "Type29.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Description = 0x04;
	constexpr auto LocationAndStatus = 0x05;
	constexpr auto MaximumValue = 0x06;
	constexpr auto MinimumValue = 0x08;
	constexpr auto Resolution = 0x0A;
	constexpr auto Tolerance = 0x0C;
	constexpr auto Accuracy = 0x0E;
	constexpr auto OEMDefined = 0x10;
	constexpr auto NominalValue = 0x14;
}

namespace
{
	class Type29Impl : public Smbios::TypeImpl
	{
	public:
		Type29Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Description),std::make_pair(0,0)},
				{GetSmbiosType(LocationAndStatus),std::make_pair(0,0)},
				{GetSmbiosType(MaximumValue),std::make_pair(0,0)},
				{GetSmbiosType(MinimumValue),std::make_pair(0,0)},
				{GetSmbiosType(Resolution),std::make_pair(0,0)},
				{GetSmbiosType(Tolerance),std::make_pair(0,0)},
				{GetSmbiosType(Accuracy),std::make_pair(0,0)},
				{GetSmbiosType(OEMDefined),std::make_pair(0,0)},
				{GetSmbiosType(NominalValue),std::make_pair(0,0)}
			};
		}
		~Type29Impl() = default;
		Type29Impl(const Type29Impl&) = delete;
		Type29Impl& operator=(const Type29Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}

		std::string GetType()
		{
			return "Type29";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(29, TableNum);
		}
	};
}

Smbios::Type29::Type29(const int32_t& TableNum) : pImpl(std::make_unique<Type29Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type29::Type29(void* PhysicalAddress) : pImpl(std::make_unique<Type29Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type29::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type29::~Type29() = default;

Smbios::Type29::Type29(Type29&&) = default;

Smbios::Type29& Smbios::Type29::operator=(Type29&&) = default;

std::string Smbios::Type29::Description() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Description, Type29Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type29::Description(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Description, Type29Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::VoltageProbeStatusField Smbios::Type29::LocationAndStatus() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::VoltageProbeStatusField>
		(*pImpl, ::LocationAndStatus, Type29Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::VoltageProbeStatusField>(ret);
	});
}

bool Smbios::Type29::LocationAndStatus(Smbios::VoltageProbeStatusField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::LocationAndStatus, Type29Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::VoltageProbeStatusField>(ret);
	}, Str);
}

std::uint16_t Smbios::Type29::MaximumValue() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::MaximumValue, Type29Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type29::MaximumValue(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MaximumValue, Type29Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type29::MinimumValue() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::MinimumValue, Type29Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type29::MinimumValue(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MinimumValue, Type29Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type29::Resolution() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Resolution, Type29Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type29::Resolution(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Resolution, Type29Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type29::Tolerance() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Tolerance, Type29Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type29::Tolerance(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Tolerance, Type29Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type29::Accuracy() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Accuracy, Type29Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type29::Accuracy(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Accuracy, Type29Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type29::OEMDefined() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::OEMDefined, Type29Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type29::OEMDefined(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMDefined, Type29Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type29::NominalValue() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::NominalValue, Type29Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type29::NominalValue(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NominalValue, Type29Impl::GetFuncRealName(__FUNCTION__), Str);
}