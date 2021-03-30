#include "stdafx.h"
#include "Type32.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Length = 0x01;
	constexpr auto BootStatus = 0x0A;

	const std::string SystemBootStatusStrs[] =
	{
		"No errors detected",
		"No bootable media",
		"\"normal\" operating system failed to load",
		"Firmware-detected hardware failure, including ¡°unknown¡± failure types",
		"Operating system-detected hardware failure",
		"User-requested boot, usually through a keystroke",
		"System security violation",
		"Previously-requested image",
		"System watchdog timer expired, causing the system to reboot"
	};
}

namespace
{
	class Type32Impl : public Smbios::TypeImpl
	{
	public:
		Type32Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(BootStatus),std::make_pair(0,0)}
			};
		}
		~Type32Impl() = default;
		Type32Impl(const Type32Impl&) = delete;
		Type32Impl& operator=(const Type32Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}

		std::string GetType()
		{
			return "Type32";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(32, TableNum);
		}
	};
}

Smbios::Type32::Type32(const int32_t& TableNum) : pImpl(std::make_unique<Type32Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type32::Type32(void* PhysicalAddress) : pImpl(std::make_unique<Type32Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type32::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type32::~Type32() = default;

Smbios::Type32::Type32(Type32&&) = default;

Smbios::Type32& Smbios::Type32::operator=(Type32&&) = default;

std::vector<std::string> Smbios::Type32::BootStatus() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::vector<std::string>>
		(*pImpl, ::Length, Type32Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector<std::string> res;
		auto end = length - ::BootStatus;
		for (auto index = 0; index < end; ++index)
		{
			auto num = GetSmbiosInformation<uint8_t>(*pImpl, ::BootStatus + index);
			if (num < _countof(SystemBootStatusStrs))
			{
				res.emplace_back(SystemBootStatusStrs[num]);
			}
			else if (num < 128)
			{
				res.emplace_back("Reserved for future assignment by this specification");
			}
			else if (num < 192)
			{
				res.emplace_back("Vendor/OEM-specific implementations");
			}
			else
			{
				res.emplace_back("Product-specific implementations");
			}
		}
		return res;
	});
}

bool Smbios::Type32::BootStatus(std::vector<std::string> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::Length, Type32Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector<std::string> res;
		auto end = length - ::BootStatus;
		for (auto index = 0; index < end; ++index)
		{
			auto num = GetSmbiosInformation<uint8_t>(*pImpl, ::BootStatus + index);
			if (num < _countof(SystemBootStatusStrs))
			{
				res.emplace_back(SystemBootStatusStrs[num]);
			}
			else if (num < 128)
			{
				res.emplace_back("Reserved for future assignment by this specification");
			}
			else if (num < 192)
			{
				res.emplace_back("Vendor/OEM-specific implementations");
			}
			else
			{
				res.emplace_back("Product-specific implementations");
			}
		}
		return res;
	}, Str);
}