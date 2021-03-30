#include "stdafx.h"
#include "Type36.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto LowerThresholdNoncritical = 0x04;
	constexpr auto UpperThresholdNoncritical = 0x06;
	constexpr auto LowerThresholdCritical = 0x08;
	constexpr auto UpperThresholdCritical = 0x0A;
	constexpr auto LowerThresholdNonrecoverable = 0x0C;
	constexpr auto UpperThresholdNonrecoverable = 0x0E;
}

namespace
{
	class Type36Impl : public Smbios::TypeImpl
	{
	public:
		Type36Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(LowerThresholdNoncritical),std::make_pair(0,0)},
				{GetSmbiosType(UpperThresholdNoncritical),std::make_pair(0,0)},
				{GetSmbiosType(LowerThresholdCritical),std::make_pair(0,0)},
				{GetSmbiosType(UpperThresholdCritical),std::make_pair(0,0)},
				{GetSmbiosType(LowerThresholdNonrecoverable),std::make_pair(0,0)},
				{GetSmbiosType(UpperThresholdNonrecoverable),std::make_pair(0,0)}
			};
		}
		~Type36Impl() = default;
		Type36Impl(const Type36Impl&) = delete;
		Type36Impl& operator=(const Type36Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type36";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(36, TableNum);
		}
	};
}

Smbios::Type36::Type36(const int32_t& TableNum) : pImpl(std::make_unique<Type36Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type36::Type36(void* PhysicalAddress) : pImpl(std::make_unique<Type36Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type36::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type36::~Type36() = default;

uint16_t Smbios::Type36::LowerThresholdCritical() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::LowerThresholdCritical, Type36Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type36::LowerThresholdCritical(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LowerThresholdCritical, Type36Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type36::UpperThresholdNoncritical() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::UpperThresholdNoncritical, Type36Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type36::UpperThresholdNoncritical(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::UpperThresholdNoncritical, Type36Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::Type36::Type36(Type36&&) = default;

Smbios::Type36& Smbios::Type36::operator=(Type36&&) = default;

std::uint16_t Smbios::Type36::LowerThresholdNoncritical() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::LowerThresholdNoncritical, Type36Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type36::LowerThresholdNoncritical(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LowerThresholdNoncritical, Type36Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type36::UpperThresholdCritical() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::UpperThresholdCritical, Type36Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type36::UpperThresholdCritical(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::UpperThresholdCritical, Type36Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type36::LowerThresholdNonrecoverable() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::LowerThresholdNonrecoverable, Type36Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type36::LowerThresholdNonrecoverable(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LowerThresholdNonrecoverable, Type36Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type36::UpperThresholdNonrecoverable() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::UpperThresholdNonrecoverable, Type36Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type36::UpperThresholdNonrecoverable(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::UpperThresholdNonrecoverable, Type36Impl::GetFuncRealName(__FUNCTION__), Str);
}