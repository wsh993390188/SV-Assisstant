#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type33 final
	{
	public:
		explicit Type33(const int32_t& TableNum);
		Type33(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type33();

		Type33(const Type33&) = delete;
		Type33& operator=(const Type33&) = delete;

		Type33(Type33&&);
		Type33& operator=(Type33&&);

		std::string ErrorType() const;
		bool ErrorType(std::string& Str) const noexcept;

		std::string ErrorGranularity()const;
		bool ErrorGranularity(std::string& Str)const noexcept;

		std::string ErrorOperation()const;
		bool ErrorOperation(std::string& Str)const noexcept;

		uint32_t VendorSyndrome()const;
		bool VendorSyndrome(std::uint32_t& Str)const noexcept;

		std::uint64_t MemoryArrayErrorAddress()const;
		bool MemoryArrayErrorAddress(std::uint64_t& Str)const noexcept;

		std::uint64_t DeviceErrorAddress()const;
		bool DeviceErrorAddress(std::uint64_t& Str)const noexcept;

		std::uint32_t ErrorResolution()const;
		bool ErrorResolution(std::uint32_t& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
