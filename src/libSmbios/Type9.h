#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type9 final
	{
	public:
		explicit Type9(const int32_t& TableNum);
		Type9(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type9();

		Type9(const Type9&) = delete;
		Type9& operator=(const Type9&) = delete;

		Type9(Type9&&);
		Type9& operator=(Type9&&);

		std::string SlotDesignation() const;
		bool SlotDesignation(std::string& Str) const noexcept;

		std::string SlotType()const;
		bool SlotType(std::string& Str)const noexcept;

		std::string SlotDataBusWidth()const;
		bool SlotDataBusWidth(std::string& Str)const noexcept;

		std::string CurrentUsage()const;
		bool CurrentUsage(std::string& Str)const noexcept;

		std::string SlotLength()const;
		bool SlotLength(std::string& Str)const noexcept;

		std::uint16_t SlotID()const;
		bool SlotID(std::uint16_t& Str)const noexcept;

		Smbios::SlotCharacteristics1Field SlotCharacteristics1()const;
		bool SlotCharacteristics1(Smbios::SlotCharacteristics1Field& Str)const noexcept;

		Smbios::SlotCharacteristics2Field SlotCharacteristics2()const;
		bool SlotCharacteristics2(Smbios::SlotCharacteristics2Field& Str)const noexcept;

		std::uint16_t SegmentGroupNum()const;
		bool SegmentGroupNum(std::uint16_t& Str)const noexcept;

		std::uint8_t BusNumber()const;
		bool BusNumber(std::uint8_t& Str)const noexcept;

		Smbios::DeviceFuncField DevFuncNum()const;
		bool DevFuncNum(Smbios::DeviceFuncField& Str)const noexcept;

		std::uint8_t DataBusWidth()const;
		bool DataBusWidth(std::uint8_t& Str)const noexcept;

		std::uint8_t PeerGroupingCount()const;
		bool PeerGroupingCount(std::uint8_t& Str)const noexcept;

		std::vector<uint64_t> PeerGroups()const;
		bool PeerGroups(std::vector<uint64_t>& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
