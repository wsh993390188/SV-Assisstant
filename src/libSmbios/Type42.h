#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type42 final
	{
	public:
		explicit Type42(const int32_t& TableNum);
		Type42(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type42();

		Type42(const Type42&) = delete;
		Type42& operator=(const Type42&) = delete;

		Type42(Type42&&);
		Type42& operator=(Type42&&);

		std::string InterfaceType() const;
		bool InterfaceType(std::string& Str) const noexcept;

		uint8_t InterfaceDataLength()const;
		bool InterfaceDataLength(uint8_t& Str)const noexcept;

		std::vector<uint8_t> InterfaceData()const;
		bool InterfaceData(std::vector<uint8_t>& Str)const noexcept;

		uint8_t NumberOfProtocol()const;
		bool NumberOfProtocol(uint8_t& Str)const noexcept;

		std::vector<Smbios::ProtocolRecordDataField> ProtocolRecords()const;
		bool ProtocolRecords(std::vector<Smbios::ProtocolRecordDataField>& Str)const noexcept;

	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
