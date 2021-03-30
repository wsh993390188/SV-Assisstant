#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type11 final
	{
	public:
		Type11(const int32_t& TableNum);
		Type11(void* PhysicalAddress);
		bool IsAvailable() const noexcept;
		~Type11();

		Type11(const Type11&) = delete;
		Type11& operator=(const Type11&) = delete;

		Type11(Type11&&);
		Type11& operator=(Type11&&);

		std::vector<std::string> Count() const;
		bool Count(std::vector <std::string>& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
