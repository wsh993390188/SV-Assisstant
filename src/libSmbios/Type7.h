#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type7 final
	{
	public:
		explicit Type7(const int32_t& TableNum);
		Type7(void* PhysicalAddress);
		~Type7();

		Type7(const Type7&) = delete;
		Type7& operator=(const Type7&) = delete;

		Type7(Type7&&);
		Type7& operator=(Type7&&);

		bool IsAvailable() const noexcept;

		std::string SocketDesignation()const;
		bool SocketDesignation(std::string& Str)const  noexcept;

		uint16_t CacheConfiguration() const;
		bool CacheConfiguration(std::uint16_t& Str)const  noexcept;

		std::string MaximumCacheSize() const;
		bool MaximumCacheSize(std::string& Str)const  noexcept;

		std::string InstalledSize() const;
		bool InstalledSize(std::string& Str)const  noexcept;

		Smbios::SRAMType SupportedSRAMType() const;
		bool SupportedSRAMType(Smbios::SRAMType& Str)const  noexcept;

		Smbios::SRAMType CurrentSRAMType() const;
		bool CurrentSRAMType(Smbios::SRAMType& Str)const  noexcept;

		uint8_t CacheSpeed() const;
		bool CacheSpeed(std::uint8_t& Str)const  noexcept;

		std::string ErrorCorrectionType() const;
		bool ErrorCorrectionType(std::string& Str)const  noexcept;

		std::string SystemCacheType() const;
		bool SystemCacheType(std::string& Str)const  noexcept;

		std::string Associativity() const;
		bool Associativity(std::string& Str)const  noexcept;

		std::string MaximumCacheSize2() const;
		bool MaximumCacheSize2(std::string& Str)const  noexcept;

		std::string InstalledCacheSize2() const;
		bool InstalledCacheSize2(std::string& Str)const  noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
