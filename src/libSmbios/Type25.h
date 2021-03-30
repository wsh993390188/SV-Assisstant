#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type25 final
	{
	public:
		explicit Type25(const int32_t& TableNum);
		Type25(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type25();

		Type25(const Type25&) = delete;
		Type25& operator=(const Type25&) = delete;

		Type25(Type25&&);
		Type25& operator=(Type25&&);

		std::uint8_t NextScheduledPoweronMonth() const;
		bool NextScheduledPoweronMonth(std::uint8_t& Str) const noexcept;

		std::uint8_t NextScheduledPoweronDay()const;
		bool NextScheduledPoweronDay(std::uint8_t& Str)const noexcept;

		std::uint8_t NextScheduledPoweronHour()const;
		bool NextScheduledPoweronHour(std::uint8_t& Str)const noexcept;

		std::uint8_t NextScheduledPoweronMinute()const;
		bool NextScheduledPoweronMinute(std::uint8_t& Str)const noexcept;

		std::uint8_t NextScheduledPoweronSecond()const;
		bool NextScheduledPoweronSecond(std::uint8_t& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
