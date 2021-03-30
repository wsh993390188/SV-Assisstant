#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type20 final
	{
	public:
		explicit Type20(const int32_t& TableNum);
		Type20(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type20();

		Type20(const Type20&) = delete;
		Type20& operator=(const Type20&) = delete;

		Type20(Type20&&);
		Type20& operator=(Type20&&);

		uint32_t StartingAddress() const;
		bool StartingAddress(std::uint32_t& Str) const noexcept;

		uint32_t EndingAddress() const;
		bool EndingAddress(std::uint32_t& Str) const noexcept;

		uint16_t MemoryDeviceHandle() const;
		bool MemoryDeviceHandle(std::uint16_t& Str) const noexcept;

		uint16_t MemoryArrayMappedAddressHandle() const;
		bool MemoryArrayMappedAddressHandle(std::uint16_t& Str) const noexcept;

		uint8_t PartitionRowPosition() const;
		bool PartitionRowPosition(std::uint8_t& Str) const noexcept;

		uint8_t InterleavePosition() const;
		bool InterleavePosition(std::uint8_t& Str) const noexcept;

		uint8_t InterleaveDataDepth() const;
		bool InterleaveDataDepth(std::uint8_t& Str) const noexcept;

		uint64_t ExtentedStartingAddress() const;
		bool ExtentedStartingAddress(std::uint64_t& Str) const noexcept;

		uint64_t ExtentedEndingAddress() const;
		bool ExtentedEndingAddress(std::uint64_t& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
