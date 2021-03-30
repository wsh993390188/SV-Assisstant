#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type14 final
	{
	public:
		explicit Type14(const int32_t& TableNum);
		Type14(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type14();

		Type14(const Type14&) = delete;
		Type14& operator=(const Type14&) = delete;

		Type14(Type14&&);
		Type14& operator=(Type14&&);

		std::string GroupName() const;
		bool GroupName(std::string& Str) const noexcept;

		std::vector<Smbios::GroupAssociationItems> Items()const;
		bool Items(std::vector<Smbios::GroupAssociationItems>& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
