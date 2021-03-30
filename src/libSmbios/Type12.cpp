#include "stdafx.h"
#include "Type12.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Count = 0x04;
}

namespace
{
	class Type12Impl : public Smbios::TypeImpl
	{
	public:
		Type12Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Count),std::make_pair(0,0)}
			};
		}
		~Type12Impl() = default;
		Type12Impl(const Type12Impl&) = delete;
		Type12Impl& operator=(const Type12Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum)
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type12";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(12, TableNum);
		}
	};
}

Smbios::Type12::Type12(const int32_t& TableNum) : pImpl(std::make_unique<Type12Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type12::Type12(void* PhysicalAddress) : pImpl(std::make_unique<Type12Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type12::IsAvailable() const noexcept
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type12::~Type12() = default;

Smbios::Type12::Type12(Type12&&) = default;

Smbios::Type12& Smbios::Type12::operator=(Type12&&) = default;

std::vector<std::string> Smbios::Type12::Count() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, std::vector<std::string>>(*pImpl, ::Count, Type12Impl::GetFuncRealName(__FUNCTION__),
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

bool Smbios::Type12::Count(std::vector<std::string> & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>(*pImpl, ::Count, Type12Impl::GetFuncRealName(__FUNCTION__),
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