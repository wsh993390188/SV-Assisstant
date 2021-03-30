#include "stdafx.h"
#include "Type37.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto ChannelType = 0x04;
	constexpr auto MaximumChannelLoad = 0x05;
	constexpr auto MemoryDeviceCount = 0x06;
	constexpr auto Memory1DeviceLoad = 0x07;
	constexpr auto Memory1DeviceHandle = 0x08;

	const std::string ChannelTypeStrs[] =
	{
		{},
		"Other",
		"Unknown",
		"RamBus",
		"SyncLink"
	};
}

namespace
{
	class Type37Impl : public Smbios::TypeImpl
	{
	public:
		Type37Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(ChannelType),std::make_pair(0,0)},
				{GetSmbiosType(MaximumChannelLoad),std::make_pair(0,0)},
				{GetSmbiosType(MemoryDeviceCount),std::make_pair(0,0)},
				{GetSmbiosType(Memory1Devices),std::make_pair(0,0)}
			};
		}
		~Type37Impl() = default;
		Type37Impl(const Type37Impl&) = delete;
		Type37Impl& operator=(const Type37Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type37";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(37, TableNum);
		}
	};
}

Smbios::Type37::Type37(const int32_t& TableNum) : pImpl(std::make_unique<Type37Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type37::Type37(void* PhysicalAddress) : pImpl(std::make_unique<Type37Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type37::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type37::~Type37() = default;

Smbios::Type37::Type37(Type37&&) = default;

Smbios::Type37& Smbios::Type37::operator=(Type37&&) = default;

std::string Smbios::Type37::ChannelType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::ChannelType, Type37Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret < _countof(ChannelTypeStrs))
		{
			return ChannelTypeStrs[ret];
		}

		return std::string();
	});
}

bool Smbios::Type37::ChannelType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::ChannelType, Type37Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& ret)
	{
		if (ret < _countof(ChannelTypeStrs))
		{
			return ChannelTypeStrs[ret];
		}

		return std::string();
	}, Str);
}

uint8_t Smbios::Type37::MaximumChannelLoad() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::MaximumChannelLoad, Type37Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type37::MaximumChannelLoad(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MaximumChannelLoad, Type37Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type37::MemoryDeviceCount() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::MemoryDeviceCount, Type37Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type37::MemoryDeviceCount(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemoryDeviceCount, Type37Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::vector<Smbios::MemoryDeviceHandle> Smbios::Type37::Memory1Devices() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::vector<Smbios::MemoryDeviceHandle>>
		(*pImpl, ::MemoryDeviceCount, Type37Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector<Smbios::MemoryDeviceHandle> ret;
		for (auto index = 1; index <= length; ++index)
		{
			Smbios::MemoryDeviceHandle temp;
			temp.DeviceLoad = GetSmbiosInformation<uint8_t>(*pImpl, ::Memory1DeviceLoad + 3 * (index - 1));
			temp.DeviceHandle = GetSmbiosInformation<uint16_t>(*pImpl, ::Memory1DeviceHandle + 3 * (index - 1));
			ret.emplace_back(std::move(temp));
		}
		return ret;
	});
}

bool Smbios::Type37::Memory1Devices(std::vector<Smbios::MemoryDeviceHandle> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::uint8_t>
		(*pImpl, ::MemoryDeviceCount, Type37Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector<Smbios::MemoryDeviceHandle> ret;
		for (auto index = 1; index <= length; ++index)
		{
			Smbios::MemoryDeviceHandle temp;
			temp.DeviceLoad = GetSmbiosInformation<uint8_t>(*pImpl, ::Memory1DeviceLoad + 3 * (index - 1));
			temp.DeviceHandle = GetSmbiosInformation<uint16_t>(*pImpl, ::Memory1DeviceHandle + 3 * (index - 1));
			ret.emplace_back(std::move(temp));
		}
		return ret;
	}, Str);
}