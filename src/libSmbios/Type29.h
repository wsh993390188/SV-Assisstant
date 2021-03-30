#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type29 final
	{
	public:
		explicit Type29(const int32_t& TableNum);
		Type29(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type29();

		Type29(const Type29&) = delete;
		Type29& operator=(const Type29&) = delete;

		Type29(Type29&&);
		Type29& operator=(Type29&&);

		std::string Description() const;
		bool Description(std::string& Str) const noexcept;

		Smbios::VoltageProbeStatusField LocationAndStatus()const;
		bool LocationAndStatus(Smbios::VoltageProbeStatusField& Str)const noexcept;

		std::uint16_t MaximumValue()const;
		bool MaximumValue(std::uint16_t& Str)const noexcept;

		std::uint16_t MinimumValue()const;
		bool MinimumValue(std::uint16_t& Str)const noexcept;

		std::uint16_t Resolution()const;
		bool Resolution(std::uint16_t& Str)const noexcept;

		std::uint16_t Tolerance()const;
		bool Tolerance(std::uint16_t& Str)const noexcept;

		std::uint16_t Accuracy()const;
		bool Accuracy(std::uint16_t& Str)const noexcept;

		std::uint32_t OEMDefined()const;
		bool OEMDefined(std::uint32_t& Str)const noexcept;

		std::uint16_t NominalValue()const;
		bool NominalValue(std::uint16_t& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
