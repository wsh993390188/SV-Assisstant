#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type2 final
	{
	public:
		Type2();
		Type2(void* PhysicalAddress);
		~Type2();

		Type2(const Type2&) = delete;
		Type2& operator=(const Type2&) = delete;

		Type2(Type2&&);
		Type2& operator=(Type2&&);

		std::string Manufacturer();
		bool Manufacturer(std::string& ManufacturerStr) const noexcept;

		std::string Product();
		bool Product(std::string& ProductStr) const noexcept;

		std::string Version();
		bool Version(std::string& VersionStr) const noexcept;

		std::string SerialNumber();
		bool SerialNumber(std::string& SerialNumberStr) const noexcept;

		std::string AssetTag();
		bool AssetTag(std::string& AssetTagStr) const noexcept;

		uint8_t FeatureFlags();
		bool FeatureFlags(uint8_t& Feature) const noexcept;

		std::string LocationinChassis();
		bool LocationinChassis(std::string& LocationStr) const noexcept;

		uint16_t ChassisHandle();
		bool ChassisHandle(uint16_t& Handle) const noexcept;

		std::string Board();
		bool Board(std::string& BoardType) const noexcept;

		uint8_t NumberofContainedObjectHandles();
		bool NumberofContainedObjectHandles(uint8_t& Handles) const noexcept;

		std::vector<uint16_t> ContainedObjectHandles();
		bool ContainedObjectHandles(std::vector<uint16_t>& ObjHandles) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
