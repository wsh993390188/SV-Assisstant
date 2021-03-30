#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type43 final
	{
	public:
		explicit Type43(const int32_t& TableNum);
		Type43(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type43();

		Type43(const Type43&) = delete;
		Type43& operator=(const Type43&) = delete;

		Type43(Type43&&);
		Type43& operator=(Type43&&);

		std::string VendorID() const;
		bool VendorID(std::string& Str) const noexcept;

		uint8_t MajorSpecVersion()const;
		bool MajorSpecVersion(uint8_t& Str)const noexcept;

		uint8_t MinorSpecVersion()const;
		bool MinorSpecVersion(uint8_t& Str)const noexcept;

		uint32_t FirmwareVersion1()const;
		bool FirmwareVersion1(uint32_t& Str)const noexcept;

		uint32_t FirmwareVersion2()const;
		bool FirmwareVersion2(uint32_t& Str)const noexcept;

		std::string Description()const;
		bool Description(std::string& Str)const noexcept;

		Smbios::TPMDeviceCharacteristicsField Characteristics()const;
		bool Characteristics(Smbios::TPMDeviceCharacteristicsField& Str)const noexcept;

		uint32_t OEMDefined()const;
		bool OEMDefined(uint32_t& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
