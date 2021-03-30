#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type34 final
	{
	public:
		explicit Type34(const int32_t& TableNum);
		Type34(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type34();

		Type34(const Type34&) = delete;
		Type34& operator=(const Type34&) = delete;

		Type34(Type34&&);
		Type34& operator=(Type34&&);

		std::string Description() const;
		bool Description(std::string& Str) const noexcept;

		std::string Types()const;
		bool Types(std::string& Str)const noexcept;

		uint32_t Address()const;
		bool Address(uint32_t& Str)const noexcept;

		std::string AddressType()const;
		bool AddressType(std::string& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
