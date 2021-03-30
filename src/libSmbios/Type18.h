#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type18 final
	{
	public:
		explicit Type18(const int32_t& TableNum);
		Type18(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type18();

		Type18(const Type18&) = delete;
		Type18& operator=(const Type18&) = delete;

		Type18(Type18&&);
		Type18& operator=(Type18&&);

		std::string ErrorType() const;
		bool ErrorType(std::string& Str) const noexcept;

		std::string ErrorGranularity()const;
		bool ErrorGranularity(std::string& Str)const noexcept;

		std::string ErrorOperation()const;
		bool ErrorOperation(std::string& Str)const noexcept;

		uint32_t VendorSyndrome()const;
		bool VendorSyndrome(std::uint32_t& Str)const noexcept;

		std::uint32_t MemoryArrayErrorAddress()const;
		bool MemoryArrayErrorAddress(std::uint32_t& Str)const noexcept;

		std::uint32_t DeviceErrorAddress()const;
		bool DeviceErrorAddress(std::uint32_t& Str)const noexcept;

		std::uint32_t ErrorResolution()const;
		bool ErrorResolution(std::uint32_t& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
