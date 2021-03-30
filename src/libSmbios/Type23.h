#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type23 final
	{
	public:
		Type23(const int32_t& TableNum);
		Type23(void* PhysicalAddress);
		~Type23();
		bool IsAvailable() const noexcept;

		Type23(const Type23&) = delete;
		Type23& operator=(const Type23&) = delete;

		Type23(Type23&&);
		Type23& operator=(Type23&&);

		Smbios::SystemResetCapabilitiesField Capabilities() const;
		bool Capabilities(Smbios::SystemResetCapabilitiesField& Str) const noexcept;

		std::uint16_t ResetCount() const;
		bool ResetCount(uint16_t& Str) const noexcept;

		std::uint16_t ResetLimit() const;
		bool ResetLimit(uint16_t& Str) const noexcept;

		std::uint16_t TimerInterval() const;
		bool TimerInterval(uint16_t& Str) const noexcept;

		std::uint16_t Timeout() const;
		bool Timeout(uint16_t& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
