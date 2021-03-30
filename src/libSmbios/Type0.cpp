#include "stdafx.h"

#include "TypeImpl.h"
#include "Type0.h"

namespace
{
	constexpr auto Vendor = 0x04;
	constexpr auto BiosVersion = 0x05;
	constexpr auto BIOSStartADDR = 0x06;
	constexpr auto BiosReleaseDate = 0x08;
	constexpr auto BiosROMSize = 0x09;
	constexpr auto BIOSCharacteristics = 0x0A;//QWORD
	constexpr auto BIOSCharacteristicsExtension = 0x12;//WORD
	constexpr auto BIOSMajorRelease = 0x14;//BYTE
	constexpr auto BIOSMinorRelease = 0x15;//BYTE
	constexpr auto FirmwareMajorRelease = 0x16;//BYTE
	constexpr auto FirmwareMinorRelease = 0x17;//BYTE
	constexpr auto ExtendedBIOSROMSIZE = 0x18;//WORD
}

namespace
{
	class Type0Impl : public Smbios::TypeImpl
	{
	public:
		Type0Impl()
		{
			TypeVersion =
			{
				{GetSmbiosType(VendorName),std::make_pair(2,0)},
				{GetSmbiosType(BiosVersion),std::make_pair(2,0)},
				{GetSmbiosType(BiosRelease),std::make_pair(2,0)},
				{GetSmbiosType(BiosStartAddress),std::make_pair(2,0)},
				{GetSmbiosType(BiosROMSize),std::make_pair(2,0)},
				{GetSmbiosType(BIOSCharacteristics),std::make_pair(2,0)},
				{GetSmbiosType(BIOSCharacteristicsExtension),std::make_pair(2,4)},
				{GetSmbiosType(BiosMajorRelease),std::make_pair(2,4)},
				{GetSmbiosType(BiosMinorRelease),std::make_pair(2,4)},
				{GetSmbiosType(EC_MajorRelease),std::make_pair(2,4)},
				{GetSmbiosType(EC_MinorRelease),std::make_pair(2,4)},
				{GetSmbiosType(BiosROMExtended),std::make_pair(3,1)}
			};
		}
		~Type0Impl() = default;

		TypeImpl& Create(const int32_t&) override
		{
			type.SwitchToTypePhyAddress(0);
			return *this;
		}

		std::string GetType()
		{
			return "Type0";
		}
	};
}

Smbios::Type0::Type0() : pImpl(std::make_unique<Type0Impl>())
{
	pImpl->Create(0);
}

Smbios::Type0::Type0(void* PhysicalAddress) : pImpl(std::make_unique<Type0Impl>())
{
	pImpl->Create(PhysicalAddress);
}

Smbios::Type0::~Type0() = default;

Smbios::Type0::Type0(Type0&&) = default;

Smbios::Type0& Smbios::Type0::operator=(Type0&&) = default;

std::string Smbios::Type0::VendorName() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::Vendor, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::VendorName(std::string & Vendor) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::Vendor, Type0Impl::GetFuncRealName(__FUNCTION__), Vendor);
}

std::string Smbios::Type0::BiosVersion() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::BiosVersion, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::BiosVersion(std::string & BiosVer) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::BiosVersion, Type0Impl::GetFuncRealName(__FUNCTION__), BiosVer);
}

std::string Smbios::Type0::BiosRelease() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, std::string>(*pImpl, ::BiosReleaseDate, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::BiosRelease(std::string & BiosReleaseDate) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::string>(*pImpl, ::BiosReleaseDate, Type0Impl::GetFuncRealName(__FUNCTION__), BiosReleaseDate);
}

uint16_t Smbios::Type0::BiosROMSize() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t, uint16_t>(*pImpl, ::BiosROMSize, Type0Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& ret) {
		return (ret + 1) * 64;
	});
}

bool Smbios::Type0::BiosROMSize(uint16_t & RomSize) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint16_t, uint16_t>(*pImpl, ::BiosROMSize, Type0Impl::GetFuncRealName(__FUNCTION__),
		[](const uint16_t& ret) ->uint16_t {
		return (ret + 1) * 64;
	}, RomSize);
}

uint64_t Smbios::Type0::BIOSCharacteristics() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint64_t>(*pImpl, ::BIOSCharacteristics, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::BIOSCharacteristics(uint64_t & Characteristics) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, uint64_t>(*pImpl, ::BIOSCharacteristics, Type0Impl::GetFuncRealName(__FUNCTION__), Characteristics);
}

uint16_t Smbios::Type0::BIOSCharacteristicsExtension() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>(*pImpl, ::BIOSCharacteristicsExtension, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::BIOSCharacteristicsExtension(uint16_t & CharacteristicsExtension) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::remove_reference_t<decltype(CharacteristicsExtension)>>
		(*pImpl, ::BIOSCharacteristicsExtension, Type0Impl::GetFuncRealName(__FUNCTION__), CharacteristicsExtension);
}

uint16_t Smbios::Type0::BiosStartAddress() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::BIOSStartADDR, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::BiosStartAddress(uint16_t & StartAddress) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::remove_reference_t<decltype(StartAddress)>>
		(*pImpl, ::BIOSStartADDR, Type0Impl::GetFuncRealName(__FUNCTION__), StartAddress);
}

uint8_t Smbios::Type0::BiosMajorRelease() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::BIOSMajorRelease, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::BiosMajorRelease(uint8_t & MajorRelease) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::remove_reference_t<decltype(MajorRelease)>>
		(*pImpl, ::BIOSMajorRelease, Type0Impl::GetFuncRealName(__FUNCTION__), MajorRelease);
}

uint8_t Smbios::Type0::BiosMinorRelease() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::BIOSMinorRelease, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::BiosMinorRelease(uint8_t & MinorRelease) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::remove_reference_t<decltype(MinorRelease)>>
		(*pImpl, ::BIOSMinorRelease, Type0Impl::GetFuncRealName(__FUNCTION__), MinorRelease);
}

uint8_t Smbios::Type0::EC_MajorRelease() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::FirmwareMajorRelease, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::EC_MajorRelease(uint8_t & MajorRelease) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::remove_reference_t<decltype(MajorRelease)>>
		(*pImpl, ::FirmwareMajorRelease, Type0Impl::GetFuncRealName(__FUNCTION__), MajorRelease);
}

uint8_t Smbios::Type0::EC_MinorRelease() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint8_t>
		(*pImpl, ::FirmwareMinorRelease, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::EC_MinorRelease(uint8_t & MinorRelease) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::remove_reference_t<decltype(MinorRelease)>>
		(*pImpl, ::FirmwareMinorRelease, Type0Impl::GetFuncRealName(__FUNCTION__), MinorRelease);
}

uint16_t Smbios::Type0::BiosROMExtended() const
{
	return GetSmbiosInformationWrapper<ExceptionUsed, uint16_t>
		(*pImpl, ::ExtendedBIOSROMSIZE, Type0Impl::GetFuncRealName(__FUNCTION__));
}

bool Smbios::Type0::BiosROMExtended(uint16_t & RomExtended) const noexcept
{
	return GetSmbiosInformationWrapper<ExceptionNoUsed, std::remove_reference_t<decltype(RomExtended)>>
		(*pImpl, ::ExtendedBIOSROMSIZE, Type0Impl::GetFuncRealName(__FUNCTION__), RomExtended);
}