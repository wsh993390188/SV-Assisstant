#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type3 final
	{
	public:
		Type3();
		Type3(void* PhysicalAddress);
		~Type3();

		Type3(const Type3&) = delete;
		Type3& operator=(const Type3&) = delete;

		Type3(Type3&&);
		Type3& operator=(Type3&&);

		std::string Manufacturer();
		bool Manufacturer(std::string& ManufacturerStr) const noexcept;
		std::string Type();
		bool Type(std::string& TypeStr) const noexcept;
		std::string Version();
		bool Version(std::string& VersionStr) const noexcept;
		std::string SerialNumber();
		bool SerialNumber(std::string& SerialNumberStr) const noexcept;
		std::string AssetTag();
		bool AssetTag(std::string& AssetTagStr) const noexcept;

		Type3StatusType BootUpState();
		bool BootUpState(Type3StatusType& Status) const noexcept;

		Type3StatusType PowerSupplyState();
		bool PowerSupplyState(Type3StatusType& Status) const noexcept;

		Type3StatusType ThermalState();
		bool ThermalState(Type3StatusType& Status) const noexcept;

		Type3SecurityStatus SecurityState();
		bool SecurityState(Type3SecurityStatus& Status) const noexcept;

		uint32_t OEMDefined();
		bool OEMDefined(uint32_t& Defined) const noexcept;

		uint8_t Height();
		bool Height(uint8_t& Height) const noexcept;

		uint8_t NumberofPowerCords();
		bool NumberofPowerCords(uint8_t& PowerCord) const noexcept;

		uint8_t ContainedElementCount();
		bool ContainedElementCount(uint8_t& Count) const noexcept;

		uint8_t ContainedElementRecordLength();
		bool ContainedElementRecordLength(uint8_t& Length) const noexcept;

		std::vector<uint8_t> ContainedElements();
		bool ContainedElements(std::vector<uint8_t>& Elements) const noexcept;

		std::string SKUNumber();
		bool SKUNumber(std::string& SKU) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
