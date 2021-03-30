#include "stdafx.h"
#include "TypeImpl.h"
#include "Type200.h"
#include "OEMControl.h"

using namespace Smbios::Lenovo;

namespace
{
	constexpr auto Length = 0x01;
	constexpr auto ID = 0x04;
	constexpr auto MTM = 0x05;
	constexpr auto BIOSSpecVersionMajorNumber = 0x06;
	constexpr auto BIOSSpecVersionMinorNumber = 0x07;
	constexpr auto EEPROMSpecVersionMajorNumber = 0x08;
	constexpr auto EEPROMSpecVersionMinorNumber = 0x09;
	constexpr auto PMSpecVersionMajorNumber = 0x0A;
	constexpr auto PMSpecVersionMinorNumber = 0x0B;
	constexpr auto SMIIoPort = 0x0C;
	constexpr auto SoftwareSMITriggerValue = 0x0D;
	constexpr auto LENOVOSpecVersionMajorNumber = 0x0E;
	constexpr auto LENOVOSpecVersionMinorNumber = 0x0F;
}

namespace
{
	class Type200Impl : public Smbios::TypeImpl
	{
	public:
		Type200Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(ID),std::make_pair(0,0)},
				{GetSmbiosType(MTM),std::make_pair(0,0)},
				{GetSmbiosType(BIOSSpecVersionMajorNumber),std::make_pair(0,0)},
				{GetSmbiosType(BIOSSpecVersionMinorNumber),std::make_pair(0,0)},
				{GetSmbiosType(EEPROMSpecVersionMajorNumber),std::make_pair(0,0)},
				{GetSmbiosType(EEPROMSpecVersionMinorNumber),std::make_pair(0,0)},
				{GetSmbiosType(PMSpecVersionMajorNumber),std::make_pair(0,0)},
				{GetSmbiosType(PMSpecVersionMinorNumber),std::make_pair(0,0)},
				{GetSmbiosType(SMIIoPort),std::make_pair(0,0)},
				{GetSmbiosType(SoftwareSMITriggerValue),std::make_pair(0,0)},
				{GetSmbiosType(LENOVOSpecVersionMajorNumber),std::make_pair(0,0)},
				{GetSmbiosType(LENOVOSpecVersionMinorNumber),std::make_pair(0,0)},
			};
		}
		~Type200Impl() = default;
		TypeImpl& Create(const int32_t& TableNum) override
		{
			SwitchTable(TableNum);
			return *this;
		}
		std::string GetType()
		{
			return "Type200(Lenovo)";
		}
	private:
		void SwitchTable(const int32_t& TableNum)
		{
			type.ResetPhyAddress();
			type.SwitchToTypePhyAddress(200, TableNum);
		}
	};
}

Type200::Type200(const int32_t& TableNum) : pImpl(std::make_unique<Type200Impl>())
{
	pImpl->Create(TableNum);
}

Type200::Type200(void* PhysicalAddress) : pImpl(std::make_unique<Type200Impl>())
{
	pImpl->Create(PhysicalAddress);
}

bool Type200::IsAvailable() const
{
	return pImpl->GetTypePhyAddress() != nullptr && Smbios::OEMControl::Instance().GetOemVendor() == Smbios::OEMControl::OemVendor::VENDOR_LENOVO;
}

Smbios::Lenovo::Type200::~Type200()
{}

Smbios::Lenovo::Type200::Type200(Type200&&) = default;

Type200& Smbios::Lenovo::Type200::operator=(Type200&&) = default;

std::string Type200::ID() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::ID, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::ID(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::ID, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

std::string Type200::MTM() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>
		(*pImpl, ::MTM, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::MTM(std::string & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::MTM, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type200::BIOSSpecVersionMajorNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::BIOSSpecVersionMajorNumber, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::BIOSSpecVersionMajorNumber(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::BIOSSpecVersionMajorNumber, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type200::BIOSSpecVersionMinorNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::BIOSSpecVersionMinorNumber, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::BIOSSpecVersionMinorNumber(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::BIOSSpecVersionMinorNumber, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type200::EEPROMSpecVersionMajorNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::EEPROMSpecVersionMajorNumber, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::EEPROMSpecVersionMajorNumber(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::EEPROMSpecVersionMajorNumber, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type200::EEPROMSpecVersionMinorNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::EEPROMSpecVersionMinorNumber, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::EEPROMSpecVersionMinorNumber(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::EEPROMSpecVersionMinorNumber, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type200::PMSpecVersionMajorNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::PMSpecVersionMajorNumber, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::PMSpecVersionMajorNumber(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::PMSpecVersionMajorNumber, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type200::PMSpecVersionMinorNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::PMSpecVersionMinorNumber, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::PMSpecVersionMinorNumber(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::PMSpecVersionMinorNumber, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type200::SMIIoPort() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::SMIIoPort, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::SMIIoPort(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SMIIoPort, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type200::SoftwareSMITriggerValue() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::SoftwareSMITriggerValue, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::SoftwareSMITriggerValue(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::SoftwareSMITriggerValue, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type200::LENOVOSpecVersionMajorNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::LENOVOSpecVersionMajorNumber, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::LENOVOSpecVersionMajorNumber(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LENOVOSpecVersionMajorNumber, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}

uint8_t Type200::LENOVOSpecVersionMinorNumber() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::uint8_t>
		(*pImpl, ::LENOVOSpecVersionMinorNumber, Type200Impl::GetFuncRealName(__FUNCTION__));
}

bool Type200::LENOVOSpecVersionMinorNumber(uint8_t & Str) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed>
		(*pImpl, ::LENOVOSpecVersionMinorNumber, Type200Impl::GetFuncRealName(__FUNCTION__), Str);
}