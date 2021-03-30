#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	namespace Acer
	{
		class Type170 final
		{
		public:
			explicit Type170(const int32_t& TableNum);
			Type170(void* PhysicalAddress);
			bool IsAvailable() const;
			~Type170();

			Type170(const Type170&) = delete;
			Type170& operator=(const Type170&) = delete;

			Type170(Type170&&);
			Type170& operator=(Type170&&);

			uint16_t Capability() const;
			bool Capability(uint16_t& Str) const noexcept;

			uint16_t ApplicationButton() const;
			bool ApplicationButton(uint16_t& Str) const noexcept;

			uint16_t MediaButton() const;
			bool MediaButton(uint16_t& Str) const noexcept;

			uint16_t DisplayButton() const;
			bool DisplayButton(uint16_t& Str) const noexcept;

			uint16_t OthersButton() const;
			bool OthersButton(uint16_t& Str) const noexcept;

			uint8_t FunctionKeyNumber()const;
			bool FunctionKeyNumber(uint8_t& Str)const noexcept;

		private:
			std::unique_ptr<TypeImpl> pImpl;
		};
	}
}
