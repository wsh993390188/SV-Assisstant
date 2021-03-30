#include "stdafx.h"
#include "Type133.h"
#include "TypeImpl.h"
#include "OEMControl.h"

using namespace Smbios::Lenovo;

namespace
{
	constexpr auto Length = 0x01;
	constexpr auto OEMData = 0x04;
}

namespace
{
	class Type133Impl : public Smbios::TypeImpl
	{
	public:
		Type133Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(OEMData),std::make_pair(0,0)},
			};
		}
		~Type133Impl() = default;
		Type133Impl(const Type133Impl&) = delete;
		Type133Impl& operator=(const Type133Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type133(Lenovo)";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(133, TableNum);
		}
	};
}

Type133::Type133(const int32_t& TableNum) : pImpl(std::make_unique<Type133Impl>())
{
	pImpl->Create(TableNum);
}

Type133::Type133(void* PhysicalAddress) : pImpl(std::make_unique<Type133Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Type133::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr && Smbios::OEMControl::Instance().GetOemVendor() == Smbios::OEMControl::OemVendor::VENDOR_LENOVO;
}

Smbios::Lenovo::Type133::~Type133()
{
}

Smbios::Lenovo::Type133::Type133(Type133&&) = default;

Type133& Smbios::Lenovo::Type133::operator=(Type133&&) = default;

std::string Type133::OEMData() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::OEMData, Type133Impl::GetFuncRealName(__FUNCTION__));
}

bool Type133::OEMData(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OEMData, Type133Impl::GetFuncRealName(__FUNCTION__), Str);
}