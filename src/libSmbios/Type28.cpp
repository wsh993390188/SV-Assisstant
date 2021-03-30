#include "stdafx.h"
#include "Type28.h"
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
	class Type28Impl : public Smbios::TypeImpl
	{
	public:
		Type28Impl()
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
		~Type28Impl() = default;
		Type28Impl(const Type28Impl&) = delete;
		Type28Impl& operator=(const Type28Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type28";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(28, TableNum);
		}
	};
}

Smbios::Type28::Type28(const int32_t& TableNum) : pImpl(std::make_unique<Type28Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type28::Type28(void* PhysicalAddress) : pImpl(std::make_unique<Type28Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type28::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type28::~Type28() = default;

Smbios::Type28::Type28(Type28&&) = default;

Smbios::Type28& Smbios::Type28::operator=(Type28&&) = default;

std::string Smbios::Type28::Description() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Description, Type28Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type28::Description(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Description, Type28Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::VoltageProbeStatusField Smbios::Type28::LocationAndStatus() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::VoltageProbeStatusField>
		(*pImpl, ::LocationAndStatus, Type28Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::VoltageProbeStatusField>(ret);
	});
}

bool Smbios::Type28::LocationAndStatus(Smbios::VoltageProbeStatusField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::LocationAndStatus, Type28Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::VoltageProbeStatusField>(ret);
	}, Str);
}

std::uint16_t Smbios::Type28::MaximumValue() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::MaximumValue, Type28Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type28::MaximumValue(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MaximumValue, Type28Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type28::MinimumValue() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::MinimumValue, Type28Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type28::MinimumValue(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MinimumValue, Type28Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type28::Resolution() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Resolution, Type28Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type28::Resolution(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Resolution, Type28Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type28::Tolerance() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Tolerance, Type28Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type28::Tolerance(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Tolerance, Type28Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type28::Accuracy() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Accuracy, Type28Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type28::Accuracy(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Accuracy, Type28Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type28::OEMDefined() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::OEMDefined, Type28Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type28::OEMDefined(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMDefined, Type28Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type28::NominalValue() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::NominalValue, Type28Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type28::NominalValue(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NominalValue, Type28Impl::GetFuncRealName(__FUNCTION__), Str);
}