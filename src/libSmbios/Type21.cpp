#include "stdafx.h"
#include "Type21.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Types = 0x04;
	constexpr auto Interface = 0x05;
	constexpr auto NumOfButtons = 0x06;

	//Bytes or Bit
	const std::string PointingDeviceTypeStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Mouse"),
		("Track Ball"),
		("Track Point"),
		("Glide Point"),
		("Touch Pad"),
		("Touch Screen"),
		("Optical Sensor")
	};
	const std::string PointingDeviceInterfaceStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Serial"),
		("PS/2"),
		("Infrared"),
		("HP-HIL"),
		("Bus mouse"),
		("ADB (Apple Desktop Bus)"),
	};
	const std::string PointingDeviceInterface2Strs[] =
	{
		("Bus mouse DB-9"),
		("Bus mouse micro-DIN"),
		("USB")
	};
}

namespace
{
	class Type21Impl : public Smbios::TypeImpl
	{
	public:
		Type21Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Types),std::make_pair(2,1)},
				{GetSmbiosType(Interface),std::make_pair(2,1)},
				{GetSmbiosType(NumOfButtons),std::make_pair(2,1)}
			};
		}
		~Type21Impl() = default;
		Type21Impl(const Type21Impl&) = delete;
		Type21Impl& operator=(const Type21Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type21";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(21, TableNum);
		}
	};
}

Smbios::Type21::Type21(const int32_t& TableNum) : pImpl(std::make_unique<Type21Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type21::Type21(void* PhysicalAddress) : pImpl(std::make_unique<Type21Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type21::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type21::~Type21() = default;

Smbios::Type21::Type21(Type21&&) = default;

Smbios::Type21& Smbios::Type21::operator=(Type21&&) = default;

std::string Smbios::Type21::Types() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::Types, Type21Impl::GetFuncRealName(__FUNCTION__),
			[](const auto& num)
	{
		if (num < _countof(PointingDeviceTypeStrs))
		{
			return PointingDeviceTypeStrs[num];
		}
		else
		{
			return std::string();
		}
	});
}

bool Smbios::Type21::Types(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::Types, Type21Impl::GetFuncRealName(__FUNCTION__),
			[](const auto& num)
	{
		if (num < _countof(PointingDeviceTypeStrs))
		{
			return PointingDeviceTypeStrs[num];
		}
		else
		{
			return std::string();
		}
	}, Str);
}

std::string Smbios::Type21::Interface() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::Interface, Type21Impl::GetFuncRealName(__FUNCTION__),
			[](const auto& num)
	{
		if (num < _countof(PointingDeviceInterfaceStrs))
		{
			return PointingDeviceInterfaceStrs[num];
		}
		else if (num >= 0xA0)
		{
			if (num - 0xA0 < _countof(PointingDeviceInterface2Strs))
			{
				return PointingDeviceInterface2Strs[num - 0xA0];
			}
		}
		return std::string();
	});
}

bool Smbios::Type21::Interface(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::Interface, Type21Impl::GetFuncRealName(__FUNCTION__),
			[](const auto& num)
	{
		if (num < _countof(PointingDeviceInterfaceStrs))
		{
			return PointingDeviceInterfaceStrs[num];
		}
		else if (num >= 0xA0)
		{
			if (num - 0xA0 < _countof(PointingDeviceInterface2Strs))
			{
				return PointingDeviceInterface2Strs[num - 0xA0];
			}
		}
		return std::string();
	}, Str);
}

uint8_t Smbios::Type21::NumOfButtons() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::NumOfButtons, Type21Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type21::NumOfButtons(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NumOfButtons, Type21Impl::GetFuncRealName(__FUNCTION__), Str);
}