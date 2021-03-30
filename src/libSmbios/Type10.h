#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type10 final
	{
	public:
		explicit Type10(const int32_t& TableNum);
		Type10(void* PhysicalAddress);
		bool IsAvailable() const noexcept;
		~Type10();

		Type10(const Type10&) = delete;
		Type10& operator=(const Type10&) = delete;

		Type10(Type10&&);
		Type10& operator=(Type10&&);

		std::vector<Smbios::OnBroadDeviceInformation> DeviceInformation() const;
		bool DeviceInformation(std::vector<Smbios::OnBroadDeviceInformation>& Str) const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
