#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type19 final
	{
	public:
		Type19(const int32_t& TableNum);
		Type19(void* PhysicalAddress);
		bool IsAvailable() const noexcept;
		~Type19();

		Type19(const Type19&) = delete;
		Type19& operator=(const Type19&) = delete;

		Type19(Type19&&);
		Type19& operator=(Type19&&);

		uint32_t StartingAddress() const;
		bool StartingAddress(std::uint32_t& Str) const noexcept;

		uint32_t EndingAddress() const;
		bool EndingAddress(std::uint32_t& Str) const noexcept;

		uint16_t MemoryArrayHandle() const;
		bool MemoryArrayHandle(std::uint16_t& Str) const noexcept;

		uint8_t PartitionWidth() const;
		bool PartitionWidth(std::uint8_t& Str) const noexcept;

		uint64_t ExtentedStartingAddress() const;
		bool ExtentedStartingAddress(std::uint64_t& Str) const noexcept;

		uint64_t ExtentedEndingAddress() const;
		bool ExtentedEndingAddress(std::uint64_t& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
