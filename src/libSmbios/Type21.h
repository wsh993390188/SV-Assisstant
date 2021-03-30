#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type21 final
	{
	public:
		explicit Type21(const int32_t& TableNum);
		Type21(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type21();

		Type21(const Type21&) = delete;
		Type21& operator=(const Type21&) = delete;

		Type21(Type21&&);
		Type21& operator=(Type21&&);

		std::string Types() const;
		bool Types(std::string& Str) const noexcept;

		std::string Interface() const;
		bool Interface(std::string& Str) const noexcept;

		uint8_t NumOfButtons() const;
		bool NumOfButtons(std::uint8_t& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
