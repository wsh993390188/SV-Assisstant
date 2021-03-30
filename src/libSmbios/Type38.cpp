#include "stdafx.h"
#include "Type38.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto InterfaceType = 0x04;
	constexpr auto IPMISpecRev = 0x05;
	constexpr auto I2CSlaveAddress = 0x06;
	constexpr auto NVStorageDeviceAddress = 0x07;
	constexpr auto BaseAddress = 0x08;
	constexpr auto BaseAddressModInterrupt = 0x10;
	constexpr auto InterruptNumber = 0x11;

	const std::string BMCInterfaceTypeStrs[] =
	{
		"Unknown",
		"KCS : Keyboard Controller Style",
		"SMIC : Server Management Interface Chip",
		"BT : Block Transfer",
		"SSIF : SMBus System Interface"
	};
}

namespace
{
	class Type38Impl : public Smbios::TypeImpl
	{
	public:
		Type38Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(InterfaceType),std::make_pair(0,0)},
				{GetSmbiosType(IPMISpecRev),std::make_pair(0,0)},
				{GetSmbiosType(I2CSlaveAddress),std::make_pair(0,0)},
				{GetSmbiosType(NVStorageDeviceAddress),std::make_pair(0,0)},
				{GetSmbiosType(BaseAddress),std::make_pair(0,0)},
				{GetSmbiosType(BaseAddressModInterrupt),std::make_pair(0,0)},
				{GetSmbiosType(InterruptNumber),std::make_pair(0,0)}
			};
		}
		~Type38Impl() = default;
		Type38Impl(const Type38Impl&) = delete;
		Type38Impl& operator=(const Type38Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type38";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(38, TableNum);
		}
	};
}

Smbios::Type38::Type38(const int32_t& TableNum) : pImpl(std::make_unique<Type38Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type38::Type38(void* PhysicalAddress) : pImpl(std::make_unique<Type38Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type38::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type38::~Type38() = default;

Smbios::Type38::Type38(Type38&&) = default;

Smbios::Type38& Smbios::Type38::operator=(Type38&&) = default;

std::string Smbios::Type38::InterfaceType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::InterfaceType, Type38Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(BMCInterfaceTypeStrs))
		{
			return BMCInterfaceTypeStrs[ret];
		}
		return std::string("Reversed");
	});
}

bool Smbios::Type38::InterfaceType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::InterfaceType, Type38Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		if (ret < _countof(BMCInterfaceTypeStrs))
		{
			return BMCInterfaceTypeStrs[ret];
		}
		return std::string("Reversed");
	}, Str);
}

Smbios::IPMIRevisionField Smbios::Type38::IPMISpecRev() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::IPMIRevisionField>
		(*pImpl, ::IPMISpecRev, Type38Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::IPMIRevisionField>(ret);
	});
}

bool Smbios::Type38::IPMISpecRev(Smbios::IPMIRevisionField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::IPMISpecRev, Type38Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		return static_cast<Smbios::IPMIRevisionField>(ret);
	}, Str);
}

std::uint8_t Smbios::Type38::I2CSlaveAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::I2CSlaveAddress, Type38Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type38::I2CSlaveAddress(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::I2CSlaveAddress, Type38Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type38::NVStorageDeviceAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::NVStorageDeviceAddress, Type38Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type38::NVStorageDeviceAddress(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NVStorageDeviceAddress, Type38Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint64_t Smbios::Type38::BaseAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>
		(*pImpl, ::BaseAddress, Type38Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type38::BaseAddress(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::BaseAddress, Type38Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::BaseAddrOrInterruptField Smbios::Type38::BaseAddressModInterrupt() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::BaseAddrOrInterruptField>
		(*pImpl, ::BaseAddressModInterrupt, Type38Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::BaseAddrOrInterruptField>(ret);
	});
}

bool Smbios::Type38::BaseAddressModInterrupt(Smbios::BaseAddrOrInterruptField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::BaseAddressModInterrupt, Type38Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::BaseAddrOrInterruptField>(ret);
	}, Str);
}

std::uint8_t Smbios::Type38::InterruptNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::InterruptNumber, Type38Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type38::InterruptNumber(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::InterruptNumber, Type38Impl::GetFuncRealName(__FUNCTION__), Str);
}