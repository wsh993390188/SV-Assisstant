#include "stdafx.h"
#include "Type26.h"
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
	class Type26Impl : public Smbios::TypeImpl
	{
	public:
		Type26Impl()
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
		~Type26Impl() = default;
		Type26Impl(const Type26Impl&) = delete;
		Type26Impl& operator=(const Type26Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type26";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(26, TableNum);
		}
	};
}

Smbios::Type26::Type26(const int32_t& TableNum) : pImpl(std::make_unique<Type26Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type26::Type26(void* PhysicalAddress) : pImpl(std::make_unique<Type26Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type26::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type26::~Type26() = default;

Smbios::Type26::Type26(Type26&&) = default;

Smbios::Type26& Smbios::Type26::operator=(Type26&&) = default;

std::string Smbios::Type26::Description() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Description, Type26Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type26::Description(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Description, Type26Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::VoltageProbeStatusField Smbios::Type26::LocationAndStatus() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::VoltageProbeStatusField>
		(*pImpl, ::LocationAndStatus, Type26Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::VoltageProbeStatusField>(ret);
	});
}

bool Smbios::Type26::LocationAndStatus(Smbios::VoltageProbeStatusField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::LocationAndStatus, Type26Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::VoltageProbeStatusField>(ret);
	}, Str);
}

std::uint16_t Smbios::Type26::MaximumValue() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::MaximumValue, Type26Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type26::MaximumValue(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MaximumValue, Type26Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type26::MinimumValue() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::MinimumValue, Type26Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type26::MinimumValue(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MinimumValue, Type26Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type26::Resolution() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Resolution, Type26Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type26::Resolution(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Resolution, Type26Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type26::Tolerance() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Tolerance, Type26Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type26::Tolerance(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Tolerance, Type26Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type26::Accuracy() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Accuracy, Type26Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type26::Accuracy(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Accuracy, Type26Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type26::OEMDefined() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::OEMDefined, Type26Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type26::OEMDefined(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMDefined, Type26Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type26::NominalValue() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::NominalValue, Type26Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type26::NominalValue(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NominalValue, Type26Impl::GetFuncRealName(__FUNCTION__), Str);
}