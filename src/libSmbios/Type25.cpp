#include "stdafx.h"
#include "Type25.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto NextScheduledPoweronMonth = 0x04;
	constexpr auto NextScheduledPoweronDay = 0x05;
	constexpr auto NextScheduledPoweronHour = 0x06;
	constexpr auto NextScheduledPoweronMinute = 0x07;
	constexpr auto NextScheduledPoweronSecond = 0x08;

#pragma pack(push,1)
	struct NumbericConvertHelp
	{
		union
		{
			struct
			{
				uint8_t LowNunber8_1;
				uint8_t LowNunber8_2;
				uint8_t LowNunber8_3;
				uint8_t LowNunber8_4;
				uint8_t HighNunber8_1;
				uint8_t HighNunber8_2;
				uint8_t HighNunber8_3;
				uint8_t HighNunber8_4;
			};

			struct
			{
				uint16_t LowNunber16_1;
				uint16_t LowNunber16_2;
				uint16_t HighNunber16_1;
				uint16_t HighNunber16_2;
			};

			struct
			{
				uint32_t LowNumber32;
				uint32_t HighNumber32;
			};
			uint64_t Number;
		};
	};
#pragma pack(pop)
}

namespace
{
	class Type25Impl : public Smbios::TypeImpl
	{
	public:
		Type25Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(NextScheduledPoweronSecond),std::make_pair(0,0)},
				{GetSmbiosType(NextScheduledPoweronMonth),std::make_pair(0,0)},
				{GetSmbiosType(NextScheduledPoweronMinute),std::make_pair(0,0)},
				{GetSmbiosType(NextScheduledPoweronHour),std::make_pair(0,0)},
				{GetSmbiosType(NextScheduledPoweronDay),std::make_pair(0,0)}
			};
		}
		~Type25Impl() = default;
		Type25Impl(const Type25Impl&) = delete;
		Type25Impl& operator=(const Type25Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type25";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(25, TableNum);
		}
	};
}

Smbios::Type25::Type25(const int32_t& TableNum) : pImpl(std::make_unique<Type25Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type25::Type25(void* PhysicalAddress) : pImpl(std::make_unique<Type25Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type25::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type25::~Type25() = default;

Smbios::Type25::Type25(Type25&&) = default;

Smbios::Type25& Smbios::Type25::operator=(Type25&&) = default;

std::uint8_t Smbios::Type25::NextScheduledPoweronMonth() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::NextScheduledPoweronMonth, Type25Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type25::NextScheduledPoweronMonth(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NextScheduledPoweronMonth, Type25Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type25::NextScheduledPoweronDay() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::NextScheduledPoweronDay, Type25Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type25::NextScheduledPoweronDay(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NextScheduledPoweronDay, Type25Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type25::NextScheduledPoweronHour() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::NextScheduledPoweronHour, Type25Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type25::NextScheduledPoweronHour(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NextScheduledPoweronHour, Type25Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type25::NextScheduledPoweronMinute() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::NextScheduledPoweronMinute, Type25Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type25::NextScheduledPoweronMinute(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NextScheduledPoweronMinute, Type25Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type25::NextScheduledPoweronSecond() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::NextScheduledPoweronSecond, Type25Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type25::NextScheduledPoweronSecond(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NextScheduledPoweronSecond, Type25Impl::GetFuncRealName(__FUNCTION__), Str);
}