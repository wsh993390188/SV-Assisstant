#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type35 final
	{
	public:
		explicit Type35(const int32_t& TableNum);
		Type35(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type35();

		Type35(const Type35&) = delete;
		Type35& operator=(const Type35&) = delete;

		Type35(Type35&&);
		Type35& operator=(Type35&&);

		std::string Description()const;
		bool Description(std::string& Str)const noexcept;

		std::uint16_t ManagementDeviceHandle()const;
		bool ManagementDeviceHandle(std::uint16_t& Str)const noexcept;

		std::uint16_t ComponentHandle()const;
		bool ComponentHandle(std::uint16_t& Str)const noexcept;

		std::uint16_t ThresholdHandle()const;
		bool ThresholdHandle(std::uint16_t& Str)const noexcept;

	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
