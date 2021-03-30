#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	namespace Lenovo
	{
		class Type200 final
		{
		public:
			explicit Type200(const int32_t& TableNum);
			Type200(void* PhysicalAddress);
			bool IsAvailable() const;
			~Type200();

			Type200(const Type200&) = delete;
			Type200& operator=(const Type200&) = delete;

			Type200(Type200&&);
			Type200& operator=(Type200&&);

			std::string ID() const;
			bool ID(std::string& Str) const noexcept;

			std::string MTM() const;
			bool MTM(std::string& Str) const noexcept;

			uint8_t BIOSSpecVersionMajorNumber() const;
			bool BIOSSpecVersionMajorNumber(uint8_t& Str) const noexcept;

			uint8_t BIOSSpecVersionMinorNumber() const;
			bool BIOSSpecVersionMinorNumber(uint8_t& Str) const noexcept;

			uint8_t EEPROMSpecVersionMajorNumber() const;
			bool EEPROMSpecVersionMajorNumber(uint8_t& Str) const noexcept;

			uint8_t EEPROMSpecVersionMinorNumber() const;
			bool EEPROMSpecVersionMinorNumber(uint8_t& Str) const noexcept;

			uint8_t PMSpecVersionMajorNumber() const;
			bool PMSpecVersionMajorNumber(uint8_t& Str) const noexcept;

			uint8_t PMSpecVersionMinorNumber() const;
			bool PMSpecVersionMinorNumber(uint8_t& Str) const noexcept;

			uint8_t SMIIoPort() const;
			bool SMIIoPort(uint8_t& Str) const noexcept;

			uint8_t SoftwareSMITriggerValue() const;
			bool SoftwareSMITriggerValue(uint8_t& Str) const noexcept;

			uint8_t LENOVOSpecVersionMajorNumber() const;
			bool LENOVOSpecVersionMajorNumber(uint8_t& Str) const noexcept;

			uint8_t LENOVOSpecVersionMinorNumber() const;
			bool LENOVOSpecVersionMinorNumber(uint8_t& Str) const noexcept;
		private:
			std::unique_ptr<Smbios::TypeImpl> pImpl;
		};
	}
}
