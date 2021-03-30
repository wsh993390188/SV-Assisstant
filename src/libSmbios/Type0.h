#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type0 final
	{
	public:
		Type0();
		Type0(void* PhysicalAddress);
		~Type0();

		Type0(const Type0&) = delete;
		Type0& operator=(const Type0&) = delete;

		Type0(Type0&&);
		Type0& operator=(Type0&&);

		std::string VendorName() const;
		bool VendorName(std::string& Vendor) const noexcept;

		std::string BiosVersion() const;
		bool BiosVersion(std::string& BiosVer) const noexcept;

		uint16_t BiosStartAddress() const;
		bool BiosStartAddress(uint16_t& StartAddress) const noexcept;

		std::string BiosRelease() const;
		bool BiosRelease(std::string& BiosReleaseDate) const noexcept;

		uint16_t BiosROMSize() const;
		bool BiosROMSize(uint16_t& RomSize) const noexcept;

		uint64_t BIOSCharacteristics() const;
		bool BIOSCharacteristics(uint64_t& Characteristics) const noexcept;

		uint16_t BIOSCharacteristicsExtension() const;
		bool BIOSCharacteristicsExtension(uint16_t& CharacteristicsExtension) const noexcept;

		uint8_t BiosMajorRelease() const;
		bool BiosMajorRelease(uint8_t& MajorRelease) const noexcept;

		uint8_t BiosMinorRelease() const;
		bool BiosMinorRelease(uint8_t& MinorRelease) const noexcept;

		uint8_t EC_MajorRelease() const;
		bool EC_MajorRelease(uint8_t& MajorRelease) const noexcept;

		uint8_t EC_MinorRelease() const;
		bool EC_MinorRelease(uint8_t& MinorRelease) const noexcept;

		uint16_t BiosROMExtended() const;
		bool BiosROMExtended(uint16_t& RomExtended) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
