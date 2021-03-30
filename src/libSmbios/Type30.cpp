#include "stdafx.h"
#include "Type30.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto ManufacturerName = 0x04;
	constexpr auto Connections = 0x05;
}

namespace
{
	class Type30Impl : public Smbios::TypeImpl
	{
	public:
		Type30Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(ManufacturerName),std::make_pair(0,0)},
				{GetSmbiosType(Connections),std::make_pair(0,0)}
			};
		}
		~Type30Impl() = default;
		Type30Impl(const Type30Impl&) = delete;
		Type30Impl& operator=(const Type30Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}

		std::string GetType()
		{
			return "Type30";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(30, TableNum);
		}
	};
}

Smbios::Type30::Type30(const int32_t& TableNum) : pImpl(std::make_unique<Type30Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type30::Type30(void* PhysicalAddress) : pImpl(std::make_unique<Type30Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type30::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type30::~Type30() = default;

Smbios::Type30::Type30(Type30&&) = default;

Smbios::Type30& Smbios::Type30::operator=(Type30&&) = default;

std::string Smbios::Type30::ManufacturerName() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::ManufacturerName, Type30Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type30::ManufacturerName(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ManufacturerName, Type30Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::RemoteAccessConnectField Smbios::Type30::Connections() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::RemoteAccessConnectField>
		(*pImpl, ::Connections, Type30Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::RemoteAccessConnectField>(ret);
	});
}

bool Smbios::Type30::Connections(Smbios::RemoteAccessConnectField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::Connections, Type30Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::RemoteAccessConnectField>(ret);
	}, Str);
}