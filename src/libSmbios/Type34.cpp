#include "stdafx.h"
#include "Type34.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Description = 0x04;
	constexpr auto Types = 0x05;
	constexpr auto Address = 0x06;
	constexpr auto AddressType = 0x0A;

	const std::string ManagementDeviceTypeStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("National Semiconductor LM75"),
		("National Semiconductor LM78"),
		("National Semiconductor LM79"),
		("National Semiconductor LM80"),
		("National Semiconductor LM81"),
		("Analog Devices ADM9240"),
		("Dallas Semiconductor DS1780"),
		("Maxim 1617"),
		("Genesys GL518SM"),
		("Winbond W83781D"),
		("Holtek HT82H791")
	};

	const std::string ManagementDeviceAddressTypeStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("I/O Port"),
		("Memory"),
		("SM Bus")
	};
}

namespace
{
	class Type34Impl : public Smbios::TypeImpl
	{
	public:
		Type34Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Description),std::make_pair(0,0)},
				{GetSmbiosType(Types),std::make_pair(0,0)},
				{GetSmbiosType(Address),std::make_pair(0,0)},
				{GetSmbiosType(AddressType),std::make_pair(0,0)}
			};
		}
		~Type34Impl() = default;
		Type34Impl(const Type34Impl&) = delete;
		Type34Impl& operator=(const Type34Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type34";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(34, TableNum);
		}
	};
}

Smbios::Type34::Type34(const int32_t& TableNum) : pImpl(std::make_unique<Type34Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type34::Type34(void* PhysicalAddress) : pImpl(std::make_unique<Type34Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type34::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type34::~Type34() = default;

Smbios::Type34::Type34(Type34&&) = default;

Smbios::Type34& Smbios::Type34::operator=(Type34&&) = default;

std::string Smbios::Type34::Description() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Description, Type34Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type34::Description(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Description, Type34Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type34::Types() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::Types, Type34Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret < _countof(ManagementDeviceTypeStrs))
		{
			return ManagementDeviceTypeStrs[ret];
		}
		return std::string();
	});
}

bool Smbios::Type34::Types(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::Types, Type34Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret < _countof(ManagementDeviceTypeStrs))
		{
			return ManagementDeviceTypeStrs[ret];
		}
		return std::string();
	}, Str);
}

uint32_t Smbios::Type34::Address() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::Address, Type34Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type34::Address(uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Address, Type34Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type34::AddressType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::AddressType, Type34Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(ManagementDeviceAddressTypeStrs))
		{
			return ManagementDeviceAddressTypeStrs[ret];
		}
		return std::string();
	});
}

bool Smbios::Type34::AddressType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::AddressType, Type34Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(ManagementDeviceAddressTypeStrs))
		{
			return ManagementDeviceAddressTypeStrs[ret];
		}
		return std::string();
	}, Str);
}