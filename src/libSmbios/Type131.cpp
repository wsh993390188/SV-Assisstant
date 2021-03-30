#include "stdafx.h"
#include "Type131.h"
#include "TypeImpl.h"
#include "OEMControl.h"

using namespace Smbios::Lenovo;

namespace
{
	constexpr auto Length = 0x01;
	constexpr auto Version = 0x04;
	constexpr auto TVTStructure = 0x06;
	constexpr auto Description = 0x15;
}

namespace
{
	class Type131Impl : public Smbios::TypeImpl
	{
	public:
		Type131Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Version),std::make_pair(0,0)},
				{GetSmbiosType(Description),std::make_pair(0,0)},
				{GetSmbiosType(TVTStructure),std::make_pair(0,0)},
			};
		}
		~Type131Impl() = default;
		Type131Impl(const Type131Impl&) = delete;
		Type131Impl& operator=(const Type131Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type131(Lenovo)";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(131, TableNum);
		}
	};
}

Type131::Type131(const int32_t& TableNum) : pImpl(std::make_unique<Type131Impl>())
{
	pImpl->Create(TableNum);
}

Type131::Type131(void* PhysicalAddress) : pImpl(std::make_unique<Type131Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Type131::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr && Smbios::OEMControl::Instance().GetOemVendor() == Smbios::OEMControl::OemVendor::VENDOR_LENOVO;
}

Smbios::Lenovo::Type131::~Type131() = default;

Smbios::Lenovo::Type131::Type131(Type131&&) = default;

Type131& Smbios::Lenovo::Type131::operator=(Type131&&) = default;

uint8_t Type131::Version() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::Version, Type131Impl::GetFuncRealName(__FUNCTION__));
}

bool Type131::Version(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Version, Type131Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::vector<uint8_t> Type131::TVTStructure() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::vector<uint8_t>>
		(*pImpl, ::Length, Type131Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& length)
	{
		std::vector<uint8_t> ret;
		if (length == 0x16)
		{
			for (auto index = 0; index < 16; ++index)
			{
				ret.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, ::TVTStructure + index));
			}
		}
		return ret;
	});
}

bool Type131::TVTStructure(std::vector<uint8_t> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::TVTStructure, Type131Impl::GetFuncRealName(__FUNCTION__), [this](const uint8_t& length)
	{
		std::vector<uint8_t> ret;
		if (length == 0x16)
		{
			for (auto index = 0; index < 16; ++index)
			{
				ret.emplace_back(GetSmbiosInformation<uint8_t>(*pImpl, ::TVTStructure + index));
			}
		}
		return ret;
	}, Str);
}

std::string Type131::Description() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Description, Type131Impl::GetFuncRealName(__FUNCTION__));
}

bool Type131::Description(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Description, Type131Impl::GetFuncRealName(__FUNCTION__), Str);
}