#include "stdafx.h"
#include "Type20.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto StartingAddress = 0x04;
	constexpr auto EndingAddress = 0x08;
	constexpr auto MemoryDeviceHandle = 0x0C;
	constexpr auto MemoryArrayMappedAddressHandle = 0x0E;
	constexpr auto PartitionRowPosition = 0x10;
	constexpr auto InterleavePosition = 0x11;
	constexpr auto InterleaveDataDepth = 0x12;
	constexpr auto ExtentedStartingAddress = 0x13;
	constexpr auto ExtentedEndingAddress = 0x1B;
}

namespace
{
	class Type20Impl : public Smbios::TypeImpl
	{
	public:
		Type20Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(StartingAddress),std::make_pair(2,1)},
				{GetSmbiosType(EndingAddress),std::make_pair(2,1)},
				{GetSmbiosType(MemoryDeviceHandle),std::make_pair(2,1)},
				{GetSmbiosType(MemoryArrayMappedAddressHandle),std::make_pair(2,1)},
				{GetSmbiosType(PartitionRowPosition),std::make_pair(2,1)},
				{GetSmbiosType(InterleavePosition),std::make_pair(2,1)},
				{GetSmbiosType(InterleaveDataDepth),std::make_pair(2,1)},
				{GetSmbiosType(ExtentedStartingAddress),std::make_pair(2,7)},
				{GetSmbiosType(ExtentedEndingAddress),std::make_pair(2,7)}
			};
		}
		~Type20Impl() = default;
		Type20Impl(const Type20Impl&) = delete;
		Type20Impl& operator=(const Type20Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type20";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(20, TableNum);
		}
	};
}

Smbios::Type20::Type20(const int32_t& TableNum) : pImpl(std::make_unique<Type20Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type20::Type20(void* PhysicalAddress) : pImpl(std::make_unique<Type20Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type20::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type20::~Type20() = default;

Smbios::Type20::Type20(Type20&&) = default;

Smbios::Type20& Smbios::Type20::operator=(Type20&&) = default;

uint32_t Smbios::Type20::StartingAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::StartingAddress, Type20Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type20::StartingAddress(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::StartingAddress, Type20Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint32_t Smbios::Type20::EndingAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::EndingAddress, Type20Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type20::EndingAddress(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::EndingAddress, Type20Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type20::MemoryDeviceHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::MemoryDeviceHandle, Type20Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type20::MemoryDeviceHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemoryDeviceHandle, Type20Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type20::MemoryArrayMappedAddressHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::MemoryArrayMappedAddressHandle, Type20Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type20::MemoryArrayMappedAddressHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemoryArrayMappedAddressHandle, Type20Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type20::PartitionRowPosition() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::PartitionRowPosition, Type20Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type20::PartitionRowPosition(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::PartitionRowPosition, Type20Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type20::InterleavePosition() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::InterleavePosition, Type20Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type20::InterleavePosition(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::InterleavePosition, Type20Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type20::InterleaveDataDepth() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::InterleaveDataDepth, Type20Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type20::InterleaveDataDepth(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::InterleaveDataDepth, Type20Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint64_t Smbios::Type20::ExtentedStartingAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>
		(*pImpl, ::ExtentedStartingAddress, Type20Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type20::ExtentedStartingAddress(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ExtentedStartingAddress, Type20Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint64_t Smbios::Type20::ExtentedEndingAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>
		(*pImpl, ::ExtentedEndingAddress, Type20Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type20::ExtentedEndingAddress(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ExtentedEndingAddress, Type20Impl::GetFuncRealName(__FUNCTION__), Str);
}