#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type27 final
	{
	public:
		explicit Type27(const int32_t& TableNum);
		Type27(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type27();

		Type27(const Type27&) = delete;
		Type27& operator=(const Type27&) = delete;

		Type27(Type27&&);
		Type27& operator=(Type27&&);

		uint16_t TemperatureProbeHandle() const;
		bool TemperatureProbeHandle(uint16_t& Str) const noexcept;

		Smbios::DeviceStatusField DeviceTypeAndStatus()const;
		bool DeviceTypeAndStatus(Smbios::DeviceStatusField& Str)const noexcept;

		std::uint8_t CoolingUnitGroup()const;
		bool CoolingUnitGroup(std::uint8_t& Str)const noexcept;

		uint32_t OEMDefined()const;
		bool OEMDefined(uint32_t& Str)const noexcept;

		uint16_t NominalSpeed()const;
		bool NominalSpeed(uint16_t& Str)const noexcept;

		std::string Description()const;
		bool Description(std::string& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
