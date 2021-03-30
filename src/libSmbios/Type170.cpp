#include "stdafx.h"
#include "Type170.h"
#include "TypeImpl.h"
#include "OEMControl.h"

using namespace Smbios::Acer;

namespace
{
	constexpr auto Capability = 0x04;
	constexpr auto ApplicationButton = 0x06;
	constexpr auto MediaButton = 0x08;
	constexpr auto DisplayButton = 0x0A;
	constexpr auto OthersButton = 0x0C;
	constexpr auto FunctionKeyNumber = 0x0E;
}

namespace
{
	class Type170Impl : public Smbios::TypeImpl
	{
	public:
		Type170Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(Capability),std::make_pair(0,0)},
				{GetSmbiosType(ApplicationButton),std::make_pair(0,0)},
				{GetSmbiosType(MediaButton),std::make_pair(0,0)},
				{GetSmbiosType(DisplayButton),std::make_pair(0,0)},
				{GetSmbiosType(OthersButton),std::make_pair(0,0)},
				{GetSmbiosType(FunctionKeyNumber),std::make_pair(0,0)},
			};
		}
		~Type170Impl() = default;
		Type170Impl(const Type170Impl&) = delete;
		Type170Impl& operator=(const Type170Impl&) = delete;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type170(Acer)";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(170, TableNum);
		}
	};
}

Type170::Type170(const int32_t& TableNum) : pImpl(std::make_unique<Type170Impl>())
{
	pImpl->Create(TableNum);
}

Type170::Type170(void* PhysicalAddress) : pImpl(std::make_unique<Type170Impl>())
{
	pImpl->Create(PhysicalAddress);
}
bool Type170::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr && Smbios::OEMControl::Instance().GetOemVendor() == Smbios::OEMControl::OemVendor::VENDOR_ACER;
}

Smbios::Acer::Type170::~Type170()
{
}

Smbios::Acer::Type170::Type170(Type170&&) = default;

Type170& Smbios::Acer::Type170::operator=(Type170&&) = default;

uint16_t Type170::Capability() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::Capability, Type170Impl::GetFuncRealName(__FUNCTION__));
}

bool Type170::Capability(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::Capability, Type170Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Type170::ApplicationButton() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::ApplicationButton, Type170Impl::GetFuncRealName(__FUNCTION__));
}

bool Type170::ApplicationButton(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ApplicationButton, Type170Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Type170::MediaButton() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::MediaButton, Type170Impl::GetFuncRealName(__FUNCTION__));
}

bool Type170::MediaButton(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MediaButton, Type170Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Type170::DisplayButton() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::DisplayButton, Type170Impl::GetFuncRealName(__FUNCTION__));
}

bool Type170::DisplayButton(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::DisplayButton, Type170Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint16_t Type170::OthersButton() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint16_t>
		(*pImpl, ::OthersButton, Type170Impl::GetFuncRealName(__FUNCTION__));
}

bool Type170::OthersButton(uint16_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::OthersButton, Type170Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type170::FunctionKeyNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::FunctionKeyNumber, Type170Impl::GetFuncRealName(__FUNCTION__));
}

bool Type170::FunctionKeyNumber(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::FunctionKeyNumber, Type170Impl::GetFuncRealName(__FUNCTION__), Str);
}