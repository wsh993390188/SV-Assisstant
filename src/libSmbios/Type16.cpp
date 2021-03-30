#include "stdafx.h"

#include "Type16.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Location = 0x04;
	constexpr auto Use = 0x05;
	constexpr auto MemoryErrorCorrection = 0x06;
	constexpr auto MaximumCapacity = 0x07;
	constexpr auto MemoryErrorInformationHandle = 0x0B;
	constexpr auto NumOfMemoryDevices = 0x0D;
	constexpr auto ExtendedMaximumCapacity = 0x0F;

	const std::string MemoryArrayLocationStrs[] =
	{
		{},
		{"Other"},
		("Unknown"),
		("System board or motherboard"),
		("ISA add-on card"),
		("EISA add-on card"),
		("PCI add-on card"),
		("MCA add-on card"),
		("PCMCIA add-on card"),
		("Proprietary add-on card"),
		("NuBus")
	};

	const std::string MemoryArrayLocation2Strs[] =
	{
		("PC-98/C20 add-on card"),
		("PC-98/C24 add-on card"),
		("PC-98/E add-on card"),
		("PC-98/Local bus add-on card"),
		("CXL Flexbus 1.0 add-on card")
	};

	const std::string MemoryArrayUse[] =
	{
		{},
		("Other"),
		("Unknown"),
		("System memory"),
		("Video memory"),
		("Flash memory"),
		("Non-volatile RAM"),
		("Cache memory")
	};

	const std::string MemoryArrayErrorCorrectionStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("None"),
		("Parity"),
		("Single-bit ECC"),
		("Multi-bit ECC"),
		("CRC")
	};
}

namespace
{
	class Type16Impl : public Smbios::TypeImpl
	{
	public:
		Type16Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Location),std::make_pair(2,1)},
				{GetSmbiosType(Use),std::make_pair(2,1)},
				{GetSmbiosType(MemoryErrorCorrection),std::make_pair(2,1)},
				{GetSmbiosType(MaximumCapacity),std::make_pair(2,1)},
				{GetSmbiosType(MemoryErrorInformationHandle),std::make_pair(2,1)},
				{GetSmbiosType(NumOfMemoryDevices),std::make_pair(2,1)},
				{GetSmbiosType(ExtendedMaximumCapacity),std::make_pair(2,7)}
			};
		}
		~Type16Impl() = default;
		Type16Impl(const Type16Impl&) = delete;
		Type16Impl& operator=(const Type16Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum)
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type16";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(16, TableNum);
		}
	};
}

Smbios::Type16::Type16(const int32_t& TableNum) : pImpl(std::make_unique<Type16Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type16::Type16(void* PhysicalAddress) : pImpl(std::make_unique<Type16Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type16::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type16::~Type16() = default;

Smbios::Type16::Type16(Type16&&) = default;

Smbios::Type16& Smbios::Type16::operator=(Type16&&) = default;

std::string Smbios::Type16::Location() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::Location, Type16Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(MemoryArrayLocationStrs))
		{
			return MemoryArrayLocationStrs[num];
		}
		else if (num >= 0xA0)
		{
			if (num - 0xA0 < _countof(MemoryArrayLocation2Strs))
			{
				return MemoryArrayLocation2Strs[num - 0xA0];
			}
		}

		return std::string();
	});
}

bool Smbios::Type16::Location(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::Location, Type16Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
	{
		if (num < _countof(MemoryArrayLocationStrs))
		{
			return MemoryArrayLocationStrs[num];
		}
		else if (num >= 0xA0)
		{
			if (num - 0xA0 < _countof(MemoryArrayLocation2Strs))
			{
				return MemoryArrayLocation2Strs[num - 0xA0];
			}
		}
		return std::string();
	}, Str);
}

std::string Smbios::Type16::Use() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::Use, Type16Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(MemoryArrayUse))
		{
			return MemoryArrayUse[num];
		}
		return std::string();
	});
}

bool Smbios::Type16::Use(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::Use, Type16Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
	{
		if (num < _countof(MemoryArrayUse))
		{
			return MemoryArrayUse[num];
		}
		return std::string();
	}, Str);
}

std::string Smbios::Type16::MemoryErrorCorrection() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t, std::string>
		(*pImpl, ::MemoryErrorCorrection, Type16Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(MemoryArrayErrorCorrectionStrs))
		{
			return MemoryArrayErrorCorrectionStrs[num];
		}
		return std::string();
	});
}

bool Smbios::Type16::MemoryErrorCorrection(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::MemoryErrorCorrection, Type16Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
	{
		if (num < _countof(MemoryArrayErrorCorrectionStrs))
		{
			return MemoryArrayErrorCorrectionStrs[num];
		}
		return std::string();
	}, Str);
}

std::uint32_t Smbios::Type16::MaximumCapacity() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t>
		(*pImpl, ::MaximumCapacity, Type16Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type16::MaximumCapacity(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MaximumCapacity, Type16Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type16::MemoryErrorInformationHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::MemoryErrorInformationHandle, Type16Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type16::MemoryErrorInformationHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemoryErrorInformationHandle, Type16Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type16::NumOfMemoryDevices() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::NumOfMemoryDevices, Type16Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type16::NumOfMemoryDevices(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::NumOfMemoryDevices, Type16Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint64_t Smbios::Type16::ExtendedMaximumCapacity() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint64_t>
		(*pImpl, ::ExtendedMaximumCapacity, Type16Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type16::ExtendedMaximumCapacity(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ExtendedMaximumCapacity, Type16Impl::GetFuncRealName(__FUNCTION__), Str);
}