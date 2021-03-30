#include "stdafx.h"
#include "Type35.h"
#include "TypeImpl.h"

namespace
{
	constexpr auto Description = 0x04;
	constexpr auto ManagementDeviceHandle = 0x05;
	constexpr auto ComponentHandle = 0x07;
	constexpr auto ThresholdHandle = 0x09;
}

namespace
{
	class Type35Impl : public Smbios::TypeImpl
	{
	public:
		Type35Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Description),std::make_pair(0,0)},
				{GetSmbiosType(ManagementDeviceHandle),std::make_pair(0,0)},
				{GetSmbiosType(ComponentHandle),std::make_pair(0,0)},
				{GetSmbiosType(ThresholdHandle),std::make_pair(0,0)}
			};
		}
		~Type35Impl() = default;
		Type35Impl(const Type35Impl&) = delete;
		Type35Impl& operator=(const Type35Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type35";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(35, TableNum);
		}
	};
}

Smbios::Type35::Type35(const int32_t& TableNum) : pImpl(std::make_unique<Type35Impl>())
{
	pImpl->Create(TableNum);
}

Smbios::Type35::Type35(void* PhysicalAddress) : pImpl(std::make_unique<Type35Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Smbios::Type35::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr;
}

Smbios::Type35::~Type35() = default;

Smbios::Type35::Type35(Type35&&) = default;

Smbios::Type35& Smbios::Type35::operator=(Type35&&) = default;

std::string Smbios::Type35::Description() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::Description, Type35Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type35::Description(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Description, Type35Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type35::ManagementDeviceHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::ManagementDeviceHandle, Type35Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type35::ManagementDeviceHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ManagementDeviceHandle, Type35Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::uint16_t Smbios::Type35::ComponentHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::ComponentHandle, Type35Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type35::ComponentHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ComponentHandle, Type35Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Smbios::Type35::ThresholdHandle() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::ThresholdHandle, Type35Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type35::ThresholdHandle(std::uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ThresholdHandle, Type35Impl::GetFuncRealName(__FUNCTION__), Str);
}