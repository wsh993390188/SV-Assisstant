#include "stdafx.h"
#include "Type6.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto SocketDesignation = 0x04;
	constexpr auto BankConnections = 0x05;
	constexpr auto CurrentSpeed = 0x06;
	constexpr auto CurrentMemoryTypes = 0x07;
	constexpr auto InstalledSize = 0x09;
	constexpr auto EnabledSize = 0x0A;
	constexpr auto ErrorStatus = 0x0B;

	class Type6Impl : public Smbios::TypeImpl
	{
		Type6Impl(const Type6Impl&) = delete;
		Type6Impl& operator=(const Type6Impl&) = delete;
	public:
		Type6Impl()
		{
			TypeVersion =
			{
				{ GetSmbiosType(SocketDesignation),std::make_pair(0,0) },
				{ GetSmbiosType(BankConnections),std::make_pair(0,0) },
				{ GetSmbiosType(CurrentSpeed),std::make_pair(0,0) },
				{ GetSmbiosType(CurrentMemoryTypes),std::make_pair(0,0) },
				{ GetSmbiosType(InstalledSize),std::make_pair(0,0) },
				{ GetSmbiosType(EnabledSize),std::make_pair(0,0) },
				{ GetSmbiosType(ErrorStatus),std::make_pair(0,0) },
			};
		}
		~Type6Impl() = default;

		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}

		std::string GetType()
		{
			return "Type6";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(6, TableNum);
		}
	};
}

Smbios::Type6::Type6(const int32_t& TableNum) : pImpl(std::make_unique<Type6Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type6::Type6(void* PhysicalAddress) : pImpl(std::make_unique<Type6Impl>())
{
	pImpl->Create(PhysicalAddress);
}

Smbios::Type6::~Type6()
{
}

bool Smbios::Type6::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type6::Type6(Type6&&) = default;

Smbios::Type6& Smbios::Type6::operator=(Type6&&) = default;

std::string Smbios::Type6::SocketDesignation()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::SocketDesignation, Type6Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type6::SocketDesignation(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::SocketDesignation, Type6Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type6::BankConnections()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>(*pImpl, ::BankConnections, Type6Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type6::BankConnections(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::BankConnections, Type6Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type6::CurrentSpeed()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>(*pImpl, ::CurrentSpeed, Type6Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type6::CurrentSpeed(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::CurrentSpeed, Type6Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::MemoryTypes Smbios::Type6::CurrentMemoryTypes()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t, MemoryTypes>(*pImpl, ::CurrentMemoryTypes, Type6Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& ret)
	{
		return static_cast<Smbios::MemoryTypes>(ret);
	});
}

bool Smbios::Type6::CurrentMemoryTypes(Smbios::MemoryTypes & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint16_t, MemoryTypes>(*pImpl, ::CurrentMemoryTypes, Type6Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& ret)
	{
		return static_cast<Smbios::MemoryTypes>(ret);
	}, Str);
}

std::uint8_t Smbios::Type6::InstalledSize()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>(*pImpl, ::InstalledSize, Type6Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type6::InstalledSize(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::InstalledSize, Type6Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type6::EnabledSize()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>(*pImpl, ::EnabledSize, Type6Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type6::EnabledSize(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::EnabledSize, Type6Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type6::ErrorStatus()
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>(*pImpl, ::ErrorStatus, Type6Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type6::ErrorStatus(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::ErrorStatus, Type6Impl::GetFuncRealName(__FUNCTION__), Str);
}