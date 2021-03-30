#include "stdafx.h"

#include "Type24.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto HardwareSecuritySettings = 0x04;

	class Type24Impl : public Smbios::TypeImpl
	{
	public:
		Type24Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(HardwareSecuritySettings),std::make_pair(0,0)},
			};
		}
		~Type24Impl() = default;
		Type24Impl(const Type24Impl&) = delete;
		Type24Impl& operator=(const Type24Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}

		std::string GetType()
		{
			return "Type24";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(24, TableNum);
		}
	};
}

Smbios::Type24::Type24(const int32_t& TableNum) : pImpl(std::make_unique<Type24Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type24::Type24(void* PhysicalAddress) : pImpl(std::make_unique<Type24Impl>())
{
	pImpl->Create(PhysicalAddress);
}

Smbios::Type24::~Type24() = default;

Smbios::Type24::Type24(Type24&&) = default;

Smbios::Type24& Smbios::Type24::operator=(Type24&&) = default;

bool Smbios::Type24::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::HardwareSecurity Smbios::Type24::HardwareSecuritySettings() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t, Smbios::HardwareSecurity>
		(*pImpl, ::HardwareSecuritySettings, Type24Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		return static_cast<Smbios::HardwareSecurity>(num);
	});
}

bool Smbios::Type24::HardwareSecuritySettings(HardwareSecurity & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint8_t>
		(*pImpl, ::HardwareSecuritySettings, Type24Impl::GetFuncRealName(__FUNCTION__),
			[](const uint8_t& num)
	{
		return static_cast<Smbios::HardwareSecurity>(num);
	}, Str);
}