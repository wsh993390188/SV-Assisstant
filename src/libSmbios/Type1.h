#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type1 final
	{
	public:
		Type1();
		Type1(void* PhysicalAddress);
		~Type1();

		Type1(const Type1&) = delete;
		Type1& operator=(const Type1&) = delete;

		Type1(Type1&&);
		Type1& operator=(Type1&&);

		std::string Manufacturer();
		bool Manufacturer(std::string& ManufacturerStr) const noexcept;

		std::string ProductName();
		bool ProductName(std::string& ProductNameStr) const noexcept;

		std::string Version();
		bool Version(std::string& VersionStr) const noexcept;

		std::string SerialNumber();
		bool SerialNumber(std::string& SerialNumberStr) const noexcept;

		std::string UUID();
		bool UUID(std::string& UUIDStr) const noexcept;

		WakeUpType WakeUp();
		bool WakeUp(WakeUpType& WakeUpStr) const noexcept;

		std::string SKUNumber();
		bool SKUNumber(std::string& SKUNumberStr) const noexcept;

		std::string Family();
		bool Family(std::string& FamilyStr) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}