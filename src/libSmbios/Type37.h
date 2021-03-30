#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type37 final
	{
	public:
		explicit Type37(const int32_t& TableNum);
		Type37(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type37();

		Type37(const Type37&) = delete;
		Type37& operator=(const Type37&) = delete;

		Type37(Type37&&);
		Type37& operator=(Type37&&);

		std::string ChannelType() const;
		bool ChannelType(std::string& Str) const noexcept;

		uint8_t MaximumChannelLoad()const;
		bool MaximumChannelLoad(uint8_t& Str)const noexcept;

		uint8_t MemoryDeviceCount()const;
		bool MemoryDeviceCount(uint8_t& Str)const noexcept;

		std::vector<Smbios::MemoryDeviceHandle> Memory1Devices()const;
		bool Memory1Devices(std::vector<Smbios::MemoryDeviceHandle>& Str)const noexcept;

	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
