#include "stdafx.h"
#include "Type41.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto ReferenceDesignation = 0x04;
	constexpr auto DeviceType = 0x05;
	constexpr auto DeviceTypeInstance = 0x06;
	constexpr auto SegmentGroupNumber = 0x07;
	constexpr auto BusNumber = 0x09;
	constexpr auto DeviceFunctionNumber = 0x0A;

	const std::string OnboardDeviceTypesStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Video"),
		("SCSI Controller"),
		("Ethernet"),
		("Token Ring"),
		("Sound"),
		("PATA Controller"),
		("SATA Controller"),
		("SAS Controller")
	};
}

namespace
{
	class Type41Impl : public Smbios::TypeImpl
	{
	public:
		Type41Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(ReferenceDesignation),std::make_pair(0,0)},
				{GetSmbiosType(DeviceType),std::make_pair(0,0)},
				{GetSmbiosType(DeviceTypeInstance),std::make_pair(0,0)},
				{GetSmbiosType(SegmentGroupNumber),std::make_pair(0,0)},
				{GetSmbiosType(BusNumber),std::make_pair(0,0)},
				{GetSmbiosType(DeviceFunctionNumber),std::make_pair(0,0)}
			};
		}
		~Type41Impl() = default;
		Type41Impl(const Type41Impl&) = delete;
		Type41Impl& operator=(const Type41Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type41";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(41, TableNum);
		}
	};
}

Smbios::Type41::Type41(const int32_t& TableNum) : pImpl(std::make_unique<Type41Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type41::Type41(void* PhysicalAddress) : pImpl(std::make_unique<Type41Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type41::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type41::~Type41() = default;

Smbios::Type41::Type41(Type41&&) = default;

Smbios::Type41& Smbios::Type41::operator=(Type41&&) = default;

std::string Smbios::Type41::ReferenceDesignation() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::ReferenceDesignation, Type41Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type41::ReferenceDesignation(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ReferenceDesignation, Type41Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::ExtendedDeviceTypeField Smbios::Type41::DeviceType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::ExtendedDeviceTypeField>
		(*pImpl, ::DeviceType, Type41Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		Smbios::ExtendedDeviceTypeField temp;
		temp.Status = ret & 0x80 ? ExtendedDeviceTypeField::DeviceTypeStatus::Enable : ExtendedDeviceTypeField::DeviceTypeStatus::Disable;
		if ((ret & 0x7F) < _countof(OnboardDeviceTypesStrs))
		{
			temp.DeviceType = OnboardDeviceTypesStrs[ret & 0x7F];
		}
		else
		{
			temp.DeviceType = {};
		}
		return temp;
	});
}

bool Smbios::Type41::DeviceType(Smbios::ExtendedDeviceTypeField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::DeviceType, Type41Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		Smbios::ExtendedDeviceTypeField temp;
		temp.Status = ret & 0x80 ? ExtendedDeviceTypeField::DeviceTypeStatus::Enable : ExtendedDeviceTypeField::DeviceTypeStatus::Disable;
		if ((ret & 0x7F) < _countof(OnboardDeviceTypesStrs))
		{
			temp.DeviceType = OnboardDeviceTypesStrs[ret & 0x7F];
		}
		else
		{
			temp.DeviceType = {};
		}
		return temp;
	}, Str);
}

std::uint8_t Smbios::Type41::DeviceTypeInstance() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::DeviceTypeInstance, Type41Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type41::DeviceTypeInstance(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DeviceTypeInstance, Type41Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type41::SegmentGroupNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::SegmentGroupNumber, Type41Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type41::SegmentGroupNumber(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SegmentGroupNumber, Type41Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint8_t Smbios::Type41::BusNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::BusNumber, Type41Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type41::BusNumber(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::BusNumber, Type41Impl::GetFuncRealName(__FUNCTION__), Str);
}

Smbios::DeviceFuncField Smbios::Type41::DeviceFunctionNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, Smbios::DeviceFuncField>
		(*pImpl, ::DeviceFunctionNumber, Type41Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::DeviceFuncField>(ret);
	});
}

bool Smbios::Type41::DeviceFunctionNumber(Smbios::DeviceFuncField & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::DeviceFunctionNumber, Type41Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& ret)
	{
		return static_cast<Smbios::DeviceFuncField>(ret);
	}, Str);
}