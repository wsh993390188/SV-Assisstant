#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type28 final
	{
	public:
		explicit Type28(const int32_t& TableNum);
		Type28(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type28();

		Type28(const Type28&) = delete;
		Type28& operator=(const Type28&) = delete;

		Type28(Type28&&);
		Type28& operator=(Type28&&);

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
