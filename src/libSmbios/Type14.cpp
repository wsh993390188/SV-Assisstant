#include "stdafx.h"
#include "Type14.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Length = 0x01;
	constexpr auto GroupName = 0x04;
	constexpr auto ItemType = 0x05;
	constexpr auto ItemHandle = 0x06;
}

namespace
{
	class Type14Impl : public Smbios::TypeImpl
	{
	public:
		Type14Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(GroupName),std::make_pair(0,0)},
				{GetSmbiosType(Items),std::make_pair(0,0)}
			};
		}
		~Type14Impl() = default;
		Type14Impl(const Type14Impl&) = delete;
		Type14Impl& operator=(const Type14Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type14";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(14, TableNum);
		}
	};
}

Smbios::Type14::Type14(const int32_t& TableNum) : pImpl(std::make_unique<Type14Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type14::Type14(void* PhysicalAddress) : pImpl(std::make_unique<Type14Impl>())
{
	pImpl->Create(PhysicalAddress);
}
bool Smbios::Type14::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type14::~Type14() = default;

Smbios::Type14::Type14(Type14&&) = default;

Smbios::Type14& Smbios::Type14::operator=(Type14&&) = default;

std::string Smbios::Type14::GroupName() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::GroupName, Type14Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type14::GroupName(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::GroupName, Type14Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::vector<Smbios::GroupAssociationItems> Smbios::Type14::Items() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::vector<Smbios::GroupAssociationItems>>
		(*pImpl, ::Length, Type14Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& num)
	{
		std::vector<Smbios::GroupAssociationItems> ret;
		for (WORD n = 1; n <= (num - 5) / 3; ++n)
		{
			Smbios::GroupAssociationItems temp;
			temp.ItemType = GetSmbiosInformation<uint8_t>(*pImpl, ::ItemType + ((n - 1) * 3));
			temp.ItemHandle = GetSmbiosInformation<uint16_t>(*pImpl, ::ItemHandle + ((n - 1) * 3));
			ret.emplace_back(std::move(temp));
		}
		return ret;
	});
}

bool Smbios::Type14::Items(std::vector<Smbios::GroupAssociationItems> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::Length, Type14Impl::GetFuncRealName(__FUNCTION__), [this](const uint8_t& num)
	{
		std::vector<Smbios::GroupAssociationItems> ret;
		for (WORD n = 1; n <= (num - 5) / 3; ++n)
		{
			Smbios::GroupAssociationItems temp;
			temp.ItemType = GetSmbiosInformation<uint8_t>(*pImpl, ::ItemType + ((n - 1) * 3));
			temp.ItemHandle = GetSmbiosInformation<uint16_t>(*pImpl, ::ItemHandle + ((n - 1) * 3));
			ret.emplace_back(std::move(temp));
		}
		return ret;
	}, Str);
}