#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type40 final
	{
	public:
		explicit Type40(const int32_t& TableNum);
		Type40(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type40();

		Type40(const Type40&) = delete;
		Type40& operator=(const Type40&) = delete;

		Type40(Type40&&);
		Type40& operator=(Type40&&);

		uint8_t NumofAdditionInfEntries() const;
		bool NumofAdditionInfEntries(uint8_t& Str) const noexcept;

		std::vector<Smbios::AdditionalInformationEntry>	AdditionInfEntries() const;
		bool AdditionInfEntries(std::vector<Smbios::AdditionalInformationEntry>& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
