#include "stdafx.h"
#include "Type134.h"
#include "TypeImpl.h"
#include "OEMControl.h"

using namespace Smbios::Lenovo;

namespace
{
	constexpr auto Length = 0x01;
	constexpr auto Version = 0x04;
	constexpr auto TCPA_CAP_PROPERTY = 0x05;
	constexpr auto TCPA_CAP_VERSION = 0x09;
	constexpr auto TPMState = 0x0D;
	constexpr auto Description = 0x0E;
	constexpr auto ReservedString = 0x0F;
	const std::string StateStrs[] =
	{
		"Hidden",
		"Reversed",
		"Disabled",
		"Enabled",
	};
}

namespace
{
	class Type134Impl : public Smbios::TypeImpl
	{
	public:
		Type134Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Version),std::make_pair(0,0)},
				{GetSmbiosType(TCPA_CAP_PROPERTY),std::make_pair(0,0)},
				{GetSmbiosType(TCPA_CAP_VERSION),std::make_pair(0,0)},
				{GetSmbiosType(TPMState),std::make_pair(0,0)},
				{GetSmbiosType(Description),std::make_pair(0,0)},
				{GetSmbiosType(ReservedString),std::make_pair(0,0)},
			};
		}
		~Type134Impl() = default;
		Type134Impl(const Type134Impl&) = delete;
		Type134Impl& operator=(const Type134Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type134(Lenovo)";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(134, TableNum);
		}
	};
}

Type134::Type134(const int32_t& TableNum) : pImpl(std::make_unique<Type134Impl>())
{
	pImpl->Create(TableNum);
}

Type134::Type134(void* PhysicalAddress) : pImpl(std::make_unique<Type134Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Type134::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr && Smbios::OEMControl::Instance().GetOemVendor() == Smbios::OEMControl::OemVendor::VENDOR_LENOVO;
}

Smbios::Lenovo::Type134::~Type134()
{
}

Smbios::Lenovo::Type134::Type134(Type134&&) = default;

Type134& Smbios::Lenovo::Type134::operator=(Type134&&) = default;

uint8_t Type134::Version() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::Version, Type134Impl::GetFuncRealName(__FUNCTION__));
}

bool Type134::Version(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Version, Type134Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint32_t Type134::TCPA_CAP_PROPERTY() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::TCPA_CAP_PROPERTY, Type134Impl::GetFuncRealName(__FUNCTION__));
}

bool Type134::TCPA_CAP_PROPERTY(uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::TCPA_CAP_PROPERTY, Type134Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint32_t Type134::TCPA_CAP_VERSION() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::TCPA_CAP_VERSION, Type134Impl::GetFuncRealName(__FUNCTION__));
}

bool Type134::TCPA_CAP_VERSION(uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::TCPA_CAP_VERSION, Type134Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Type134::TPMState() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::TPMState, Type134Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(StateStrs))
			return StateStrs[num];
		else
			return std::string();
	});
}

bool Type134::TPMState(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::TPMState, Type134Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(StateStrs))
			return StateStrs[num];
		else
			return std::string();
	}, Str);
}

std::string Type134::Description() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Description, Type134Impl::GetFuncRealName(__FUNCTION__));
}

bool Type134::Description(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Description, Type134Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Type134::ReservedString() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::ReservedString, Type134Impl::GetFuncRealName(__FUNCTION__));
}

bool Type134::ReservedString(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ReservedString, Type134Impl::GetFuncRealName(__FUNCTION__), Str);
}