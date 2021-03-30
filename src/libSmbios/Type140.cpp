#include "stdafx.h"
#include "Type140.h"
#include "TypeImpl.h"
#include "OEMControl.h"

using namespace Smbios::Lenovo;

namespace
{
	constexpr auto Signature = 0x04;
	constexpr auto OEMStructOffset = 0x0A;
	constexpr auto OEMStructNumber = 0x0B;
	constexpr auto OEMStructRevision = 0x0C;
	constexpr auto ECPVersionID = 0x0D;
	constexpr auto ECPReleaseDate = 0x0E;
}

namespace
{
	class Type140Impl : public Smbios::TypeImpl
	{
	public:
		Type140Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Signature),std::make_pair(0,0)},
				{GetSmbiosType(OEMStructOffset),std::make_pair(0,0)},
				{GetSmbiosType(OEMStructNumber),std::make_pair(0,0)},
				{GetSmbiosType(OEMStructRevision),std::make_pair(0,0)},
				{GetSmbiosType(ECPVersionID),std::make_pair(0,0)},
				{GetSmbiosType(ECPReleaseDate),std::make_pair(0,0)},
			};
		}
		~Type140Impl() = default;
		Type140Impl(const Type140Impl&) = delete;
		Type140Impl& operator=(const Type140Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type140(Lenovo)";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(140, TableNum);
		}
	};
}

Type140::Type140(const int32_t& TableNum) : pImpl(std::make_unique<Type140Impl>())
{
	pImpl->Create(TableNum);
}

Type140::Type140(void* PhysicalAddress) : pImpl(std::make_unique<Type140Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Type140::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr && Smbios::OEMControl::Instance().GetOemVendor() == Smbios::OEMControl::OemVendor::VENDOR_LENOVO;
}

Smbios::Lenovo::Type140::~Type140()
{
}

Smbios::Lenovo::Type140::Type140(Type140&&) = default;

Type140& Smbios::Lenovo::Type140::operator=(Type140&&) = default;

std::string Type140::Signature() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::string>
		(*pImpl, ::Signature, Type140Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& num)
	{
		std::string temp;
		for (auto index = 0; index < 6; ++index)
		{
			temp.push_back(GetSmbiosInformation<uint8_t>(*pImpl, ::Signature + index));
		}
		return temp;
	});
}

bool Type140::Signature(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::Signature, Type140Impl::GetFuncRealName(__FUNCTION__),
			[this](const uint8_t& num)
	{
		std::string temp;
		for (auto index = 0; index < 6; ++index)
		{
			temp.push_back(GetSmbiosInformation<uint8_t>(*pImpl, ::Signature + index));
		}
		return temp;
	}, Str);
}

uint8_t Type140::OEMStructOffset() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::OEMStructOffset, Type140Impl::GetFuncRealName(__FUNCTION__));
}

bool Type140::OEMStructOffset(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMStructOffset, Type140Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type140::OEMStructNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::OEMStructNumber, Type140Impl::GetFuncRealName(__FUNCTION__));
}

bool Type140::OEMStructNumber(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMStructNumber, Type140Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type140::OEMStructRevision() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::OEMStructRevision, Type140Impl::GetFuncRealName(__FUNCTION__));
}

bool Type140::OEMStructRevision(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMStructRevision, Type140Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Type140::ECPVersionID() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::ECPVersionID, Type140Impl::GetFuncRealName(__FUNCTION__));
}

bool Type140::ECPVersionID(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ECPVersionID, Type140Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Type140::ECPReleaseDate() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::ECPReleaseDate, Type140Impl::GetFuncRealName(__FUNCTION__));
}

bool Type140::ECPReleaseDate(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ECPReleaseDate, Type140Impl::GetFuncRealName(__FUNCTION__), Str);
}