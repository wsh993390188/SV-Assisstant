#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type12 final
	{
	public:
		Type12(const int32_t& TableNum);
		Type12(void* PhysicalAddress);
		bool IsAvailable() const noexcept;
		~Type12();

		Type12(const Type12&) = delete;
		Type12& operator=(const Type12&) = delete;

		Type12(Type12&&);
		Type12& operator=(Type12&&);

		std::vector<std::string> Count() const;
		bool Count(std::vector<std::string>& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
