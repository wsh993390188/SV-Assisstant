#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type4 final
	{
	public:
		Type4(const int32_t& TableNum);
		Type4(void* PhysicalAddress);
		~Type4();

		bool IsAvailable() const noexcept;

		Type4(const Type4&) = delete;
		Type4& operator=(const Type4&) = delete;

		Type4(Type4&&);
		Type4& operator=(Type4&&);

		std::string SocketDesignation();
		bool SocketDesignation(std::string& Str);

		std::string ProcessorType();
		bool ProcessorType(std::string& Str);

		std::string ProcessorFamily();
		bool ProcessorFamily(std::string& Str);

		std::string Manufacturer();
		bool Manufacturer(std::string& Str);

		std::string ProcessorID();
		bool ProcessorID(std::string& Str);

		std::string ProcessorVersion();
		bool ProcessorVersion(std::string& Str);

		std::string Voltage();
		bool Voltage(std::string& Str);

		std::string ExtClock();
		bool ExtClock(std::string& Str);

		std::string MaxSpeed();
		bool MaxSpeed(std::string& Str);

		std::string CurrentSpeed();
		bool CurrentSpeed(std::string& Str);

		uint8_t Status();
		bool Status(uint8_t& Str);

		std::string ProcessorUpgrade();
		bool ProcessorUpgrade(std::string& Str);

		uint16_t L1CacheHandle();
		bool L1CacheHandle(std::uint16_t& Str);

		uint16_t L2CacheHandle();
		bool L2CacheHandle(std::uint16_t& Str);

		uint16_t L3CacheHandle();
		bool L3CacheHandle(std::uint16_t& Str);

		std::string SerialNumber();
		bool SerialNumber(std::string& Str);

		std::string PartNumber();
		bool PartNumber(std::string& Str);

		std::string AssetTag();
		bool AssetTag(std::string& Str);

		uint8_t CoreCount();
		bool CoreCount(std::uint8_t& Str);

		uint8_t CoreEnable();
		bool CoreEnable(std::uint8_t& Str);

		uint8_t ThreadCount();
		bool ThreadCount(std::uint8_t& Str);

		uint16_t ProcessorCharacteristics();
		bool ProcessorCharacteristics(std::uint16_t& Str);

		std::string ProcessorFamily2();
		bool ProcessorFamily2(std::string& Str);

		uint16_t CoreCount2();
		bool CoreCount2(std::uint16_t& Str);

		uint16_t CoreEnable2();
		bool CoreEnable2(std::uint16_t& Str);

		uint16_t ThreadCount2();
		bool ThreadCount2(std::uint16_t& Str);
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
