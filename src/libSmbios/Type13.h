#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type13 final
	{
	public:
		Type13(const int32_t& TableNum);
		Type13(void* PhysicalAddress);
		bool IsAvailable() const noexcept;
		~Type13();

		Type13(const Type13&) = delete;
		Type13& operator=(const Type13&) = delete;

		Type13(Type13&&);
		Type13& operator=(Type13&&);

		std::vector<std::string> InstallableLanguages() const;
		bool InstallableLanguages(std::vector<std::string>& Str) const noexcept;

		uint8_t Flags() const;
		bool Flags(uint8_t& Str) const noexcept;

		std::string CurrentLanguages() const;
		bool CurrentLanguages(std::string& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
