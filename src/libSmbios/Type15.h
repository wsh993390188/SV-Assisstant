#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type15 final
	{
	public:
		Type15(const int32_t& TableNum);
		Type15(void* PhysicalAddress);
		bool IsAvailable() const noexcept;
		~Type15();

		Type15(const Type15&) = delete;
		Type15& operator=(const Type15&) = delete;

		Type15(Type15&&);
		Type15& operator=(Type15&&);

		uint16_t LogAreaLength() const;
		bool LogAreaLength(uint16_t& Str) const noexcept;

		uint16_t LogHeaderStartOffset() const;
		bool LogHeaderStartOffset(uint16_t& Str) const noexcept;

		uint16_t LogDataStartOffset() const;
		bool LogDataStartOffset(uint16_t& Str) const noexcept;

		std::string  AccessMethod() const;
		bool AccessMethod(std::string& Str) const noexcept;

		Smbios::LogStatusField LogStatus() const;
		bool LogStatus(Smbios::LogStatusField& Str) const noexcept;

		uint32_t LogChangeToken() const;
		bool LogChangeToken(uint32_t& Str) const noexcept;

		Smbios::AccessMethodAddressField AccessMethodAddress() const;
		bool AccessMethodAddress(Smbios::AccessMethodAddressField& Str) const noexcept;

		std::string LogHeaderFormat() const;
		bool LogHeaderFormat(std::string& Str) const noexcept;

		std::uint8_t NumofSupportedLogTypes() const;
		bool NumofSupportedLogTypes(std::uint8_t& Str) const noexcept;

		std::uint8_t LengthLogTypeDescriptor() const;
		bool LengthLogTypeDescriptor(std::uint8_t& Str) const noexcept;

		std::vector<Smbios::EventLogType> ListofSupportedEventLogTypeDescriptors() const;
		bool ListofSupportedEventLogTypeDescriptors(std::vector<Smbios::EventLogType>& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
