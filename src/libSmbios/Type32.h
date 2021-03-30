#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type32 final
	{
	public:
		explicit Type32(const int32_t& TableNum);
		Type32(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type32();

		Type32(const Type32&) = delete;
		Type32& operator=(const Type32&) = delete;

		Type32(Type32&&);
		Type32& operator=(Type32&&);

		std::vector<std::string> BootStatus() const;
		bool BootStatus(std::vector<std::string>& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
