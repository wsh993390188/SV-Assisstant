#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type39 final
	{
	public:
		explicit Type39(const int32_t& TableNum);
		Type39(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type39();

		Type39(const Type39&) = delete;
		Type39& operator=(const Type39&) = delete;

		Type39(Type39&&);
		Type39& operator=(Type39&&);

		uint8_t PowerUnitGroup() const;
		bool PowerUnitGroup(uint8_t& Str) const noexcept;

		std::string Location()const;
		bool Location(std::string& Str)const noexcept;

		std::string DeviceName()const;
		bool DeviceName(std::string& Str)const noexcept;

		std::string Manufacturer()const;
		bool Manufacturer(std::string& Str)const noexcept;

		std::string SerialNumber()const;
		bool SerialNumber(std::string& Str)const noexcept;

		std::string AssetTagNumber()const;
		bool AssetTagNumber(std::string& Str)const noexcept;

		std::string ModelPartNumber()const;
		bool ModelPartNumber(std::string& Str)const noexcept;

		std::string RevisionLevel()const;
		bool RevisionLevel(std::string& Str)const noexcept;

		std::uint16_t MaxPowerCapacity()const;
		bool MaxPowerCapacity(std::uint16_t& Str)const noexcept;

		Smbios::PowerSupplyCharacteristicsField PowerSupplyCharacteristics()const;
		bool PowerSupplyCharacteristics(Smbios::PowerSupplyCharacteristicsField& Str)const noexcept;

		std::uint16_t InputVoltageProbeHandle()const;
		bool InputVoltageProbeHandle(std::uint16_t& Str)const noexcept;

		std::uint16_t CoolingDeviceHandle()const;
		bool CoolingDeviceHandle(std::uint16_t& Str)const noexcept;

		std::uint16_t InputCurrentProbeHandle()const;
		bool InputCurrentProbeHandle(std::uint16_t& Str)const noexcept;

	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
