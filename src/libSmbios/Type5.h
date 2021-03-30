#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type5 final
	{
	public:
		explicit Type5(const int32_t& TableNum);
		Type5(void* PhysicalAddress);
		bool IsAvailable() const noexcept;
		~Type5();

		Type5(const Type5&) = delete;
		Type5& operator=(const Type5&) = delete;

		Type5(Type5&&);
		Type5& operator=(Type5&&);

		std::string ErrorDetectingMethod();
		bool ErrorDetectingMethod(std::string& Str);

		ErrorCapability ErrorCorrectingCapability();
		bool ErrorCorrectingCapability(Smbios::ErrorCapability& Str);

		std::string SupportedInterleave();
		bool SupportedInterleave(std::string& Str);

		std::string CurrentInterleave();
		bool CurrentInterleave(std::string& Str);

		uint8_t MaximumMemoryMoudulesize();
		bool MaximumMemoryMoudulesize(uint8_t& Str);

		Smbios::MemorySpeeds SupportedSpeeds();
		bool SupportedSpeeds(Smbios::MemorySpeeds& Str);

		Smbios::MemoryTypes SupportedMemoryTypes();
		bool SupportedMemoryTypes(Smbios::MemoryTypes& Str);

		Smbios::MemoryVoltages MemoryModuleVoltage();
		bool MemoryModuleVoltage(Smbios::MemoryVoltages& Str);

		uint8_t NumofMemorySlots();
		bool NumofMemorySlots(uint8_t& Str);

		std::vector<uint16_t> MemoryModuleConfigurationHandles();
		bool MemoryModuleConfigurationHandles(std::vector<uint16_t>& Str);

		uint8_t EnabledErrorCorrectingCapabilities();
		bool EnabledErrorCorrectingCapabilities(uint8_t& Str);
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}