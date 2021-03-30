#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	namespace Lenovo
	{
		class Type134 final
		{
		public:
			explicit Type134(const int32_t& TableNum);
			Type134(void* PhysicalAddress);
			bool IsAvailable() const;
			~Type134();

			Type134(const Type134&) = delete;
			Type134& operator=(const Type134&) = delete;

			Type134(Type134&&);
			Type134& operator=(Type134&&);

			uint8_t Version() const;
			bool Version(uint8_t& Str) const noexcept;

			uint32_t TCPA_CAP_PROPERTY() const;
			bool TCPA_CAP_PROPERTY(uint32_t& Str) const noexcept;

			uint32_t TCPA_CAP_VERSION() const;
			bool TCPA_CAP_VERSION(uint32_t& Str) const noexcept;

			std::string TPMState() const;
			bool TPMState(std::string& Str) const noexcept;

			std::string Description() const;
			bool Description(std::string& Str) const noexcept;

			std::string ReservedString() const;
			bool ReservedString(std::string& Str) const noexcept;

		private:
			std::unique_ptr<Smbios::TypeImpl> pImpl;
		};
	}
}
