#include "stdafx.h"
#include "Type18.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto ErrorType = 0x04;
	constexpr auto ErrorGranularity = 0x05;
	constexpr auto ErrorOperation = 0x06;
	constexpr auto VendorSyndrome = 0x07;
	constexpr auto MemoryArrayErrorAddress = 0x0B;
	constexpr auto DeviceErrorAddress = 0x0F;
	constexpr auto ErrorResolution = 0x13;

	const std::string MemoryErrorTypeStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("OK"),
		("Bad read"),
		("Parity error"),
		("Single-bit error"),
		("Double-bit error"),
		("Multi-bit error"),
		("Nibble error"),
		("Checksum error"),
		("CRC error"),
		("Corrected single-bit error"),
		("Corrected error"),
		("Uncorrectable error")
	};

	const std::string MemoryErrorGranularityStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Device level"),
		("Memory partition level")
	};

	const std::string MemoryErrorOperationStrs[] =
	{
		{},
		("Other"),
		("Unknown"),
		("Read"),
		("Write"),
		("Partial Write")
	};
}

namespace
{
	class Type18Impl : public Smbios::TypeImpl
	{
	public:
		Type18Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(ErrorType),std::make_pair(2,1)},
				{GetSmbiosType(ErrorGranularity),std::make_pair(2,1)},
				{GetSmbiosType(ErrorOperation),std::make_pair(2,1)},
				{GetSmbiosType(VendorSyndrome),std::make_pair(2,1)},
				{GetSmbiosType(MemoryArrayErrorAddress),std::make_pair(2,1)},
				{GetSmbiosType(DeviceErrorAddress),std::make_pair(2,1)},
				{GetSmbiosType(ErrorResolution),std::make_pair(2,1)}
			};
		}
		~Type18Impl() = default;
		Type18Impl(const Type18Impl&) = delete;
		Type18Impl& operator=(const Type18Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}

		std::string GetType()
		{
			return "Type18";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(18, TableNum);
		}
	};
}

Smbios::Type18::Type18(const int32_t& TableNum) : pImpl(std::make_unique<Type18Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type18::Type18(void* PhysicalAddress) : pImpl(std::make_unique<Type18Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type18::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type18::~Type18() = default;

Smbios::Type18::Type18(Type18&&) = default;

Smbios::Type18& Smbios::Type18::operator=(Type18&&) = default;

std::string Smbios::Type18::ErrorType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::ErrorType, Type18Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(MemoryErrorTypeStrs))
		{
			return MemoryErrorTypeStrs[num];
		}
		else
		{
			return std::string();
		}
	});
}

bool Smbios::Type18::ErrorType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::ErrorType, Type18Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
	{
		if (num < _countof(MemoryErrorTypeStrs))
		{
			return MemoryErrorTypeStrs[num];
		}
		else
		{
			return std::string();
		}
	}, Str);
}

std::string Smbios::Type18::ErrorGranularity() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::ErrorGranularity, Type18Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(MemoryErrorGranularityStrs))
		{
			return MemoryErrorGranularityStrs[num];
		}
		else
		{
			return std::string();
		}
	});
}

bool Smbios::Type18::ErrorGranularity(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::ErrorGranularity, Type18Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
	{
		if (num < _countof(MemoryErrorGranularityStrs))
		{
			return MemoryErrorGranularityStrs[num];
		}
		else
		{
			return std::string();
		}
	}, Str);
}

std::string Smbios::Type18::ErrorOperation() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::ErrorOperation, Type18Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		if (num < _countof(MemoryErrorOperationStrs))
		{
			return MemoryErrorOperationStrs[num];
		}
		else
		{
			return std::string();
		}
	});
}

bool Smbios::Type18::ErrorOperation(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::ErrorOperation, Type18Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
	{
		if (num < _countof(MemoryErrorOperationStrs))
		{
			return MemoryErrorOperationStrs[num];
		}
		else
		{
			return std::string();
		}
	}, Str);
}

uint32_t Smbios::Type18::VendorSyndrome() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t>
		(*pImpl, ::VendorSyndrome, Type18Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type18::VendorSyndrome(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::VendorSyndrome, Type18Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type18::MemoryArrayErrorAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t>
		(*pImpl, ::MemoryArrayErrorAddress, Type18Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type18::MemoryArrayErrorAddress(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemoryArrayErrorAddress, Type18Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type18::DeviceErrorAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t>
		(*pImpl, ::DeviceErrorAddress, Type18Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type18::DeviceErrorAddress(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DeviceErrorAddress, Type18Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type18::ErrorResolution() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t>
		(*pImpl, ::ErrorResolution, Type18Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type18::ErrorResolution(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ErrorResolution, Type18Impl::GetFuncRealName(__FUNCTION__), Str);
}