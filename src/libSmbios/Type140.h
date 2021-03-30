#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	namespace Lenovo
	{
		class Type140 final
		{
		public:
			explicit Type140(const int32_t& TableNum);
			Type140(void* PhysicalAddress);
			bool IsAvailable() const;
			~Type140();

			Type140(const Type140&) = delete;
			Type140& operator=(const Type140&) = delete;

			Type140(Type140&&);
			Type140& operator=(Type140&&);

			std::string Signature() const;
			bool Signature(std::string& Str) const noexcept;

			uint8_t OEMStructOffset() const;
			bool OEMStructOffset(uint8_t& Str) const noexcept;

			uint8_t OEMStructNumber() const;
			bool OEMStructNumber(uint8_t& Str) const noexcept;

			uint8_t OEMStructRevision() const;
			bool OEMStructRevision(uint8_t& Str) const noexcept;

			std::string ECPVersionID() const;
			bool ECPVersionID(std::string& Str) const noexcept;

			std::string ECPReleaseDate() const;
			bool ECPReleaseDate(std::string& Str) const noexcept;

		private:
			std::unique_ptr<Smbios::TypeImpl> pImpl;
		};
	}
}
