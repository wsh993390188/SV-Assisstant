#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type17 final
	{
	public:
		explicit Type17(const int32_t& TableNum);
		Type17(void* PhysicalAddress);
		~Type17();

		Type17(const Type17&) = delete;
		Type17& operator=(const Type17&) = delete;

		Type17(Type17&&);
		Type17& operator=(Type17&&);

		bool IsAvailable() const;

		std::uint16_t PhysicalMemoryArrayHandle() const;
		bool PhysicalMemoryArrayHandle(std::uint16_t& Str) const noexcept;

		std::uint16_t MemoryErrorInformationHandle() const;
		bool MemoryErrorInformationHandle(std::uint16_t& Str) const noexcept;

		std::uint16_t MemoryTotalWidth() const;
		bool MemoryTotalWidth(std::uint16_t& Str) const noexcept;

		std::uint16_t Datawidth() const;
		bool Datawidth(std::uint16_t& Str) const noexcept;

		std::uint16_t Size() const;
		bool Size(std::uint16_t& Str) const noexcept;

		std::string FormFactor() const;
		bool FormFactor(std::string& Str) const noexcept;

		std::uint8_t DeviceSet() const;
		bool DeviceSet(std::uint8_t& Str) const noexcept;

		std::string DeviceLocator() const;
		bool DeviceLocator(std::string& Str) const noexcept;

		std::string BankLocator() const;
		bool BankLocator(std::string& Str) const noexcept;

		std::string MemoryType() const;
		bool MemoryType(std::string& Str) const noexcept;

		Smbios::MemoryTypeDetailField TypeDetail() const;
		bool TypeDetail(Smbios::MemoryTypeDetailField& Str) const noexcept;

		std::uint16_t Speed() const;
		bool Speed(std::uint16_t& Str) const noexcept;

		std::string Manufacturer() const;
		bool Manufacturer(std::string& Str) const noexcept;

		std::string SerialNumber() const;
		bool SerialNumber(std::string& Str) const noexcept;

		std::string AssetTag() const;
		bool AssetTag(std::string& Str) const noexcept;

		std::string PartNumber() const;
		bool PartNumber(std::string& Str) const noexcept;

		Smbios::MemoryAttributeField Attributes() const;
		bool Attributes(Smbios::MemoryAttributeField& Str) const noexcept;

		std::uint32_t ExtentedSize() const;
		bool ExtentedSize(std::uint32_t& Str) const noexcept;

		std::uint16_t ConfiguredMemoryClockSpeed() const;
		bool ConfiguredMemoryClockSpeed(std::uint16_t& Str) const noexcept;

		std::uint16_t MinimumVoltage() const;
		bool MinimumVoltage(std::uint16_t& Str) const noexcept;

		std::uint16_t MaximumVoltage() const;
		bool MaximumVoltage(std::uint16_t& Str) const noexcept;

		std::uint16_t ConfiguredVoltage() const;
		bool ConfiguredVoltage(std::uint16_t& Str) const noexcept;

		std::string MemoryTechnology() const;
		bool MemoryTechnology(std::string& Str) const noexcept;

		Smbios::MemoryModeCapability MemoryOperatingModeCapability() const;
		bool MemoryOperatingModeCapability(Smbios::MemoryModeCapability& Str) const noexcept;

		std::string FirmwareVersion() const;
		bool FirmwareVersion(std::string& Str) const noexcept;

		std::uint16_t ModuleManufacturerID() const;
		bool ModuleManufacturerID(std::uint16_t& Str) const noexcept;

		std::uint16_t ModuleProductID() const;
		bool ModuleProductID(std::uint16_t& Str) const noexcept;

		std::uint16_t MemorySubsystemControllerManufacturerID() const;
		bool MemorySubsystemControllerManufacturerID(std::uint16_t& Str) const noexcept;

		std::uint16_t MemorySubsystemControllerProductID() const;
		bool MemorySubsystemControllerProductID(std::uint16_t& Str) const noexcept;

		std::uint64_t NonVolatileSize() const;
		bool NonVolatileSize(std::uint64_t& Str) const noexcept;

		std::uint64_t VolatileSize() const;
		bool VolatileSize(std::uint64_t& Str) const noexcept;

		std::uint64_t CacheSize() const;
		bool CacheSize(std::uint64_t& Str) const noexcept;

		std::uint64_t LogicalSize() const;
		bool LogicalSize(std::uint64_t& Str) const noexcept;

		std::uint32_t ExtendedSpeed() const;
		bool ExtendedSpeed(std::uint32_t& Str) const noexcept;

		std::uint32_t ExtendedConfiguredMemorySpeed() const;
		bool ExtendedConfiguredMemorySpeed(std::uint32_t& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
