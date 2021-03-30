#include "stdafx.h"
#include "Type19.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto StartingAddress = 0x04;
	constexpr auto EndingAddress = 0x08;
	constexpr auto MemoryArrayHandle = 0x0C;
	constexpr auto PartitionWidth = 0x0E;
	constexpr auto ExtentedStartingAddress = 0x0F;
	constexpr auto ExtentedEndingAddress = 0x17;
}

namespace
{
	class Type19Impl : public Smbios::TypeImpl
	{
	public:
		Type19Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(StartingAddress),std::make_pair(2,1)},
				{GetSmbiosType(EndingAddress),std::make_pair(2,1)},
				{GetSmbiosType(MemoryArrayHandle),std::make_pair(2,1)},
				{GetSmbiosType(PartitionWidth),std::make_pair(2,1)},
				{GetSmbiosType(ExtentedStartingAddress),std::make_pair(2,7)},
				{GetSmbiosType(ExtentedEndingAddress),std::make_pair(2,7)},
			};
		}
		~Type19Impl() = default;
		Type19Impl(const Type19Impl&) = delete;
		Type19Impl& operator=(const Type19Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum)
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type19";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(19, TableNum);
		}
	};
}
Smbios::Type19::Type19(const int32_t& TableNum) : pImpl(std::make_unique<Type19Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type19::Type19(void* PhysicalAddress) : pImpl(std::make_unique<Type19Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type19::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type19::~Type19() = default;

Smbios::Type19::Type19(Type19&&) = default;

Smbios::Type19& Smbios::Type19::operator=(Type19&&) = default;

uint32_t Smbios::Type19::StartingAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::StartingAddress, Type19Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type19::StartingAddress(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::StartingAddress, Type19Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint32_t Smbios::Type19::EndingAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint32_t>
		(*pImpl, ::EndingAddress, Type19Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type19::EndingAddress(std::uint32_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::EndingAddress, Type19Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type19::MemoryArrayHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::MemoryArrayHandle, Type19Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type19::MemoryArrayHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MemoryArrayHandle, Type19Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Smbios::Type19::PartitionWidth() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::PartitionWidth, Type19Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type19::PartitionWidth(std::uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::PartitionWidth, Type19Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint64_t Smbios::Type19::ExtentedStartingAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>
		(*pImpl, ::ExtentedStartingAddress, Type19Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type19::ExtentedStartingAddress(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ExtentedStartingAddress, Type19Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint64_t Smbios::Type19::ExtentedEndingAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint64_t>
		(*pImpl, ::ExtentedEndingAddress, Type19Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type19::ExtentedEndingAddress(std::uint64_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ExtentedEndingAddress, Type19Impl::GetFuncRealName(__FUNCTION__), Str);
}