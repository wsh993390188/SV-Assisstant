#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type6 final
	{
	public:
		explicit Type6(const int32_t& TableNum);
		Type6(void* PhysicalAddress);
		~Type6();
		bool IsAvailable() const noexcept;

		Type6(const Type6&) = delete;
		Type6& operator=(const Type6&) = delete;

		Type6(Type6&&);
		Type6& operator=(Type6&&);

		std::string SocketDesignation();
		bool SocketDesignation(std::string& Str) const noexcept;

		uint8_t BankConnections();
		bool BankConnections(std::uint8_t& Str) const noexcept;

		std::uint8_t CurrentSpeed();
		bool CurrentSpeed(std::uint8_t& Str) const noexcept;

		Smbios::MemoryTypes CurrentMemoryTypes();
		bool CurrentMemoryTypes(Smbios::MemoryTypes& Str) const noexcept;

		std::uint8_t InstalledSize();
		bool InstalledSize(std::uint8_t& Str) const noexcept;

		std::uint8_t EnabledSize();
		bool EnabledSize(std::uint8_t& Str) const noexcept;

		std::uint8_t ErrorStatus();
		bool ErrorStatus(std::uint8_t& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
