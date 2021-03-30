#include "stdafx.h"
#include "Type10.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Length = 0x01;

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
	class Type10Impl : public Smbios::TypeImpl
	{
	public:
		Type10Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(DeviceInformation),std::make_pair(0,0)}
			};
		}
		~Type10Impl() = default;
		Type10Impl(const Type10Impl&) = delete;
		Type10Impl& operator=(const Type10Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum)
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type10";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(10, TableNum);
		}
	};
}

Smbios::Type10::Type10(const int32_t& TableNum) : pImpl(std::make_unique<Type10Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type10::Type10(void* PhysicalAddress) : pImpl(std::make_unique<Type10Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type10::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type10::~Type10() = default;

Smbios::Type10::Type10(Type10&&) = default;

Smbios::Type10& Smbios::Type10::operator=(Type10&&) = default;

std::vector<Smbios::OnBroadDeviceInformation> Smbios::Type10::DeviceInformation() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::vector<Smbios::OnBroadDeviceInformation> >(*pImpl, ::Length, Type10Impl::GetFuncRealName(__FUNCTION__),
		[this](const uint8_t& num)
	{
		std::vector<Smbios::OnBroadDeviceInformation> ret;
		for (WORD n = 1; n <= (num - 4) / 2; ++n)
		{
			Smbios::OnBroadDeviceInformation temp;
			auto DeviceType = GetSmbiosInformation<uint8_t>(*pImpl, 4 + 2 * (n - 1));
			temp.DeviceDesc = GetSmbiosInformation(*pImpl, 4 + 2 * (n - 1));
			if (DeviceType & 0x80)
			{
				temp.Status = OnBroadDeviceInformation::DeviceTypeStatus::Enable;
			}
			else
			{
				temp.Status = OnBroadDeviceInformation::DeviceTypeStatus::Disable;
			}

			if ((DeviceType & 0x7F) < _countof(OnboardDeviceTypesStrs))
			{
				temp.DeviceType = OnboardDeviceTypesStrs[DeviceType & 0x7F];
			}
			else
			{
				temp.DeviceType = {};
			}
		}
		return ret;
	});
}

bool Smbios::Type10::DeviceInformation(std::vector<Smbios::OnBroadDeviceInformation> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>(*pImpl, ::Length, Type10Impl::GetFuncRealName(__FUNCTION__),
		[this](const uint8_t& num)
	{
		std::vector<Smbios::OnBroadDeviceInformation> ret;
		for (WORD n = 1; n <= (num - 4) / 2; ++n)
		{
			Smbios::OnBroadDeviceInformation temp;
			auto DeviceType = GetSmbiosInformation<uint8_t>(*pImpl, 4 + 2 * (n - 1));
			temp.DeviceDesc = GetSmbiosInformation(*pImpl, 4 + 2 * (n - 1));
			if (DeviceType & 0x80)
			{
				temp.Status = OnBroadDeviceInformation::DeviceTypeStatus::Enable;
			}
			else
			{
				temp.Status = OnBroadDeviceInformation::DeviceTypeStatus::Disable;
			}

			if ((DeviceType & 0x7F) < _countof(OnboardDeviceTypesStrs))
			{
				temp.DeviceType = OnboardDeviceTypesStrs[DeviceType & 0x7F];
			}
			else
			{
				temp.DeviceType = {};
			}
		}
		return ret;
	}, Str);
}