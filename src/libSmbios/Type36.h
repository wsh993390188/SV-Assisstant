#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type36 final
	{
	public:
		explicit Type36(const int32_t& TableNum);
		Type36(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type36();

		Type36(const Type36&) = delete;
		Type36& operator=(const Type36&) = delete;

		Type36(Type36&&);
		Type36& operator=(Type36&&);

		uint16_t LowerThresholdNoncritical() const;
		bool LowerThresholdNoncritical(uint16_t& Str) const noexcept;

		uint16_t UpperThresholdNoncritical()const;
		bool UpperThresholdNoncritical(uint16_t& Str)const noexcept;

		uint16_t LowerThresholdCritical()const;
		bool LowerThresholdCritical(uint16_t& Str)const noexcept;

		uint16_t UpperThresholdCritical()const;
		bool UpperThresholdCritical(uint16_t& Str)const noexcept;

		uint16_t LowerThresholdNonrecoverable()const;
		bool LowerThresholdNonrecoverable(uint16_t& Str)const noexcept;

		std::uint16_t UpperThresholdNonrecoverable()const;
		bool UpperThresholdNonrecoverable(std::uint16_t& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
