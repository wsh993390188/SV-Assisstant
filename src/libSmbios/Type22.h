#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type22 final
	{
	public:
		explicit Type22(const int32_t& TableNum);
		Type22(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type22();

		Type22(const Type22&) = delete;
		Type22& operator=(const Type22&) = delete;

		Type22(Type22&&);
		Type22& operator=(Type22&&);

		std::string Location() const;
		bool Location(std::string& Str) const noexcept;

		std::string Manufacturer() const;
		bool Manufacturer(std::string& Str) const noexcept;

		std::string ManufacturerDate() const;
		bool ManufacturerDate(std::string& Str) const noexcept;

		std::string SerialNumber() const;
		bool SerialNumber(std::string& Str) const noexcept;

		std::string DeviceName() const;
		bool DeviceName(std::string& Str) const noexcept;

		std::string DeviceChemistry() const;
		bool DeviceChemistry(std::string& Str) const noexcept;

		std::uint16_t DesignCapacity() const;
		bool DesignCapacity(std::uint16_t& Str) const noexcept;

		std::uint16_t DesignVoltage() const;
		bool DesignVoltage(std::uint16_t& Str) const noexcept;

		std::string SBDSVersionNumber() const;
		bool SBDSVersionNumber(std::string& Str) const noexcept;

		uint8_t MaximumErrorInBatteryData() const;
		bool MaximumErrorInBatteryData(std::uint8_t& Str) const noexcept;

		std::uint16_t SDBSSerialNumber() const;
		bool SDBSSerialNumber(std::uint16_t& Str) const noexcept;

		Smbios::ManufacturerDateField SDBSManufacturerDate() const;
		bool SDBSManufacturerDate(Smbios::ManufacturerDateField& Str) const noexcept;

		std::string SDBSDeviceChemistry() const;
		bool SDBSDeviceChemistry(std::string& Str) const noexcept;

		uint8_t DesignCapacityMultiplier() const;
		bool DesignCapacityMultiplier(std::uint8_t& Str) const noexcept;

		std::uint32_t OEMspecific() const;
		bool OEMspecific(std::uint32_t& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
