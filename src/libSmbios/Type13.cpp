#include "stdafx.h"
#include "Type13.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto InstallableLanguages = 0x04;
	constexpr auto Flags = 0x05;
	constexpr auto CurrentLanguages = 0x015;
}

namespace
{
	class Type13Impl : public Smbios::TypeImpl
	{
	public:
		Type13Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(InstallableLanguages),std::make_pair(2,0)},
				{GetSmbiosType(Flags),std::make_pair(2,1)},
				{GetSmbiosType(CurrentLanguages),std::make_pair(2,0)}
			};
		}
		~Type13Impl() = default;
		Type13Impl(const Type13Impl&) = delete;
		Type13Impl& operator=(const Type13Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum)
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type13";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(13, TableNum);
		}
	};
}

Smbios::Type13::Type13(const int32_t& TableNum) : pImpl(std::make_unique<Type13Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type13::Type13(void* PhysicalAddress) : pImpl(std::make_unique<Type13Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type13::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type13::~Type13() = default;

Smbios::Type13::Type13(Type13&&) = default;

Smbios::Type13& Smbios::Type13::operator=(Type13&&) = default;

std::vector<std::string> Smbios::Type13::InstallableLanguages() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::vector<std::string>>(*pImpl, ::InstallableLanguages, Type13Impl::GetFuncRealName(__FUNCTION__),
		[this](const uint8_t& num)
	{
		std::vector<std::string>ret;
		for (auto i = 1; i <= num; ++i)
		{
			ret.emplace_back(pImpl->GetTargetString(i));
		}
		return ret;
	});
}

bool Smbios::Type13::InstallableLanguages(std::vector<std::string> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>(*pImpl, ::InstallableLanguages, Type13Impl::GetFuncRealName(__FUNCTION__),
		[this](const uint8_t& num)
	{
		std::vector<std::string>ret;
		for (auto i = 1; i <= num; ++i)
		{
			ret.emplace_back(pImpl->GetTargetString(i));
		}
		return ret;
	}, Str);
}

uint8_t Smbios::Type13::Flags() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>(*pImpl, ::Flags, Type13Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type13::Flags(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::Flags, Type13Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Smbios::Type13::CurrentLanguages() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::CurrentLanguages, Type13Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type13::CurrentLanguages(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>(*pImpl, ::CurrentLanguages, Type13Impl::GetFuncRealName(__FUNCTION__), Str);
}