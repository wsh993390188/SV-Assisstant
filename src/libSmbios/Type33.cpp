#include "stdafx.h"
#include "Type33.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto ErrorType = 0x04;
	constexpr auto ErrorGranularity = 0x05;
	constexpr auto ErrorOperation = 0x06;
	constexpr auto VendorSyndrome = 0x07;
	constexpr auto MemoryArrayErrorAddress = 0x0B;
	constexpr auto DeviceErrorAddress = 0x13;
	constexpr auto ErrorResolution = 0x1B;

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
	class Type33Impl : public Smbios::TypeImpl
	{
	public:
		Type33Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(ErrorType),std::make_pair(0,0)},
				{GetSmbiosType(ErrorGranularity),std::make_pair(0,0)},
				{GetSmbiosType(ErrorOperation),std::make_pair(0,0)},
				{GetSmbiosType(VendorSyndrome),std::make_pair(0,0)},
				{GetSmbiosType(MemoryArrayErrorAddress),std::make_pair(0,0)},
				{GetSmbiosType(DeviceErrorAddress),std::make_pair(0,0)},
				{GetSmbiosType(ErrorResolution),std::make_pair(0,0)}
			};
		}
		~Type33Impl() = default;
		Type33Impl(const Type33Impl&) = delete;
		Type33Impl& operator=(const Type33Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type33";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(33, TableNum);
		}
	};
}

Smbios::Type33::Type33(const int32_t& TableNum) : pImpl(std::make_unique<Type33Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type33::Type33(void* PhysicalAddress) : pImpl(std::make_unique<Type33Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type33::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type33::~Type33() = default;

Smbios::Type33::Type33(Type33&&) = default;

Smbios::Type33& Smbios::Type33::operator=(Type33&&) = default;

std::string Smbios::Type33::ErrorType() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::ErrorType, Type33Impl::GetFuncRealName(__FUNCTION__),
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

bool Smbios::Type33::ErrorType(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::ErrorType, Type33Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
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

std::string Smbios::Type33::ErrorGranularity() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::ErrorGranularity, Type33Impl::GetFuncRealName(__FUNCTION__),
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

bool Smbios::Type33::ErrorGranularity(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::ErrorGranularity, Type33Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
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

std::string Smbios::Type33::ErrorOperation() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::ErrorOperation, Type33Impl::GetFuncRealName(__FUNCTION__),
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

bool Smbios::Type33::ErrorOperation(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::ErrorOperation, Type33Impl::GetFuncRealName(__FUNCTION__), [](const uint8_t& num)
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

uint32_t Smbios::Type33::VendorSyndrome() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t>
		(*pImpl, ::VendorSyndrome, Type33Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type33::VendorSyndrome(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::VendorSyndrome, Type33Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint64_t Smbios::Type33::MemoryArrayErrorAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint64_t>
		(*pImpl, ::MemoryArrayErrorAddress, Type33Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type33::MemoryArrayErrorAddress(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemoryArrayErrorAddress, Type33Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint64_t Smbios::Type33::DeviceErrorAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint64_t>
		(*pImpl, ::DeviceErrorAddress, Type33Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type33::DeviceErrorAddress(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DeviceErrorAddress, Type33Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint32_t Smbios::Type33::ErrorResolution() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint32_t>
		(*pImpl, ::ErrorResolution, Type33Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type33::ErrorResolution(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ErrorResolution, Type33Impl::GetFuncRealName(__FUNCTION__), Str);
}