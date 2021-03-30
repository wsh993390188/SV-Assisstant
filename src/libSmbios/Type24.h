#pragma once
#include <memory>
#include "SmbiosDefination.h"
namespace Smbios
{
	class TypeImpl;
	class Type24 final
	{
	public:
		explicit Type24(const int32_t& TableNum);
		Type24(void* PhysicalAddress);
		~Type24();

		Type24(const Type24&) = delete;
		Type24& operator=(const Type24&) = delete;

		Type24(Type24&&);
		Type24& operator=(Type24&&);

		bool IsAvailable() const;

		HardwareSecurity HardwareSecuritySettings() const;
		bool HardwareSecuritySettings(HardwareSecurity& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
