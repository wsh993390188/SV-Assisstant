#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type38 final
	{
	public:
		explicit Type38(const int32_t& TableNum);
		Type38(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type38();

		Type38(const Type38&) = delete;
		Type38& operator=(const Type38&) = delete;

		Type38(Type38&&);
		Type38& operator=(Type38&&);

		std::string InterfaceType() const;
		bool InterfaceType(std::string& Str) const noexcept;

		Smbios::IPMIRevisionField IPMISpecRev()const;
		bool IPMISpecRev(Smbios::IPMIRevisionField& Str)const noexcept;

		uint8_t I2CSlaveAddress()const;
		bool I2CSlaveAddress(uint8_t& Str)const noexcept;

		uint8_t NVStorageDeviceAddress()const;
		bool NVStorageDeviceAddress(uint8_t& Str)const noexcept;

		uint64_t BaseAddress()const;
		bool BaseAddress(uint64_t& Str)const noexcept;

		Smbios::BaseAddrOrInterruptField BaseAddressModInterrupt()const;
		bool BaseAddressModInterrupt(Smbios::BaseAddrOrInterruptField& Str)const noexcept;

		uint8_t InterruptNumber()const;
		bool InterruptNumber(uint8_t& Str)const noexcept;

	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
