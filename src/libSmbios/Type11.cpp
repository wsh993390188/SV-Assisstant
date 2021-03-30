#include "stdafx.h"
#include "Type11.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Count = 0x04;
}

namespace
{
	class Type11Impl : public Smbios::TypeImpl
	{
	public:
		Type11Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Count),std::make_pair(2,0)}
			};
		}
		~Type11Impl() = default;
		Type11Impl(const Type11Impl&) = delete;
		Type11Impl& operator=(const Type11Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum)
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type11";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(11, TableNum);
		}
	};
}

Smbios::Type11::Type11(const int32_t& TableNum) : pImpl(std::make_unique<Type11Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type11::Type11(void* PhysicalAddress) : pImpl(std::make_unique<Type11Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type11::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type11::~Type11() = default;

Smbios::Type11::Type11(Type11&&) = default;

Smbios::Type11& Smbios::Type11::operator=(Type11&&) = default;

std::vector<std::string> Smbios::Type11::Count() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::vector<std::string>>(*pImpl, ::Count, Type11Impl::GetFuncRealName(__FUNCTION__),
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

bool Smbios::Type11::Count(std::vector<std::string> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>(*pImpl, ::Count, Type11Impl::GetFuncRealName(__FUNCTION__),
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