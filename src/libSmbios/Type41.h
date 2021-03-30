#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type41 final
	{
	public:
		explicit Type41(const int32_t& TableNum);
		Type41(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type41();

		Type41(const Type41&) = delete;
		Type41& operator=(const Type41&) = delete;

		Type41(Type41&&);
		Type41& operator=(Type41&&);

		std::string ReferenceDesignation() const;
		bool ReferenceDesignation(std::string& Str) const noexcept;

		Smbios::ExtendedDeviceTypeField DeviceType()const;
		bool DeviceType(Smbios::ExtendedDeviceTypeField& Str)const noexcept;

		uint8_t DeviceTypeInstance()const;
		bool DeviceTypeInstance(uint8_t& Str)const noexcept;

		uint16_t SegmentGroupNumber()const;
		bool SegmentGroupNumber(uint16_t& Str)const noexcept;

		uint8_t BusNumber()const;
		bool BusNumber(uint8_t& Str)const noexcept;

		Smbios::DeviceFuncField DeviceFunctionNumber()const;
		bool DeviceFunctionNumber(Smbios::DeviceFuncField& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
