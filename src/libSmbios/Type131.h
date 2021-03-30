#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	namespace Lenovo
	{
		class Type131 final
		{
		public:
			explicit Type131(const int32_t& TableNum);
			Type131(void* PhysicalAddress);
			bool IsAvailable() const;
			~Type131();

			Type131(const Type131&) = delete;
			Type131& operator=(const Type131&) = delete;

			Type131(Type131&&);
			Type131& operator=(Type131&&);

			uint8_t Version() const;
			bool Version(uint8_t& Str) const noexcept;

			std::vector<uint8_t> TVTStructure() const;
			bool TVTStructure(std::vector<uint8_t>& Str) const noexcept;

			std::string Description() const;
			bool Description(std::string& Str) const noexcept;

		private:
			std::unique_ptr<Smbios::TypeImpl> pImpl;
		};
	}
}
