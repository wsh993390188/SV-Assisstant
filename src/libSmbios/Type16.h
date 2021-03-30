#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type16 final
	{
	public:
		Type16(const int32_t& TableNum);
		Type16(void* PhysicalAddress);
		bool IsAvailable() const noexcept;
		~Type16();

		Type16(const Type16&) = delete;
		Type16& operator=(const Type16&) = delete;

		Type16(Type16&&);
		Type16& operator=(Type16&&);

		std::string Location() const;
		bool Location(std::string& Str) const noexcept;

		std::string Use() const;
		bool Use(std::string& Str) const noexcept;

		std::string MemoryErrorCorrection() const;
		bool MemoryErrorCorrection(std::string& Str) const noexcept;

		std::uint32_t MaximumCapacity() const;
		bool MaximumCapacity(std::uint32_t& Str) const noexcept;

		std::uint16_t MemoryErrorInformationHandle() const;
		bool MemoryErrorInformationHandle(std::uint16_t& Str) const noexcept;

		std::uint16_t NumOfMemoryDevices() const;
		bool NumOfMemoryDevices(std::uint16_t& Str) const noexcept;

		std::uint64_t ExtendedMaximumCapacity() const;
		bool ExtendedMaximumCapacity(std::uint64_t& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
