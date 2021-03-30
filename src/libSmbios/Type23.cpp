#include "stdafx.h"
#include "Type23.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Capabilities = 0x04;
	constexpr auto ResetCount = 0x05;
	constexpr auto ResetLimit = 0x07;
	constexpr auto TimerInterval = 0x09;
	constexpr auto Timeout = 0x0B;
}

namespace
{
	class Type23Impl : public Smbios::TypeImpl
	{
	public:
		Type23Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Capabilities),std::make_pair(0,0)},
				{GetSmbiosType(ResetCount),std::make_pair(0,0)},
				{GetSmbiosType(ResetLimit),std::make_pair(0,0)},
				{GetSmbiosType(TimerInterval),std::make_pair(0,0)},
				{GetSmbiosType(Timeout),std::make_pair(0,0)}
			};
		}
		~Type23Impl() = default;
		Type23Impl(const Type23Impl&) = delete;
		Type23Impl& operator=(const Type23Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum)
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type23";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(23, TableNum);
		}
	};
}

Smbios::Type23::Type23(const int32_t& TableNum) : pImpl(std::make_unique<Type23Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type23::Type23(void* PhysicalAddress) : pImpl(std::make_unique<Type23Impl>())
{
	pImpl->Create(PhysicalAddress);
}

Smbios::Type23::~Type23() = default;

bool Smbios::Type23::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type23::Type23(Type23&&) = default;

Smbios::Type23& Smbios::Type23::operator=(Type23&&) = default;

Smbios::SystemResetCapabilitiesField Smbios::Type23::Capabilities() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, Smbios::SystemResetCapabilitiesField>
		(*pImpl, ::Capabilities, Type23Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		return static_cast<Smbios::SystemResetCapabilitiesField>(num);
	});
}

bool Smbios::Type23::Capabilities(Smbios::SystemResetCapabilitiesField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::Capabilities, Type23Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		return static_cast<Smbios::SystemResetCapabilitiesField>(num);
	}, Str);
}

std::uint16_t Smbios::Type23::ResetCount() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::ResetCount, Type23Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type23::ResetCount(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ResetCount, Type23Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type23::ResetLimit() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::ResetLimit, Type23Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type23::ResetLimit(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ResetLimit, Type23Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type23::TimerInterval() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::TimerInterval, Type23Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type23::TimerInterval(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::TimerInterval, Type23Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type23::Timeout() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::Timeout, Type23Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type23::Timeout(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Timeout, Type23Impl::GetFuncRealName(__FUNCTION__), Str);
}