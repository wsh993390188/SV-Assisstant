#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type44 final
	{
	public:
		explicit Type44(const int32_t& TableNum);
		Type44(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type44();

		Type44(const Type44&) = delete;
		Type44& operator=(const Type44&) = delete;

		Type44(Type44&&);
		Type44& operator=(Type44&&);

		uint16_t ReferencedHandle() const;
		bool ReferencedHandle(uint16_t& Str) const noexcept;

		std::vector<Smbios::ProcessorSpecificField> ProcessorSpecificBlock()const;
		bool ProcessorSpecificBlock(std::vector < Smbios::ProcessorSpecificField>& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
