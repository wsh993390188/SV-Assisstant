#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type30 final
	{
	public:
		explicit Type30(const int32_t& TableNum);
		Type30(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type30();

		Type30(const Type30&) = delete;
		Type30& operator=(const Type30&) = delete;

		Type30(Type30&&);
		Type30& operator=(Type30&&);

		std::string ManufacturerName() const;
		bool ManufacturerName(std::string& Str) const noexcept;

		Smbios::RemoteAccessConnectField Connections()const;
		bool Connections(Smbios::RemoteAccessConnectField& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
