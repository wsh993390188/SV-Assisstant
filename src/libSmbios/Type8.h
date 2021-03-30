#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	class Type8 final
	{
	public:
		explicit Type8(const int32_t& TableNum);
		Type8(void* PhysicalAddress);
		bool IsAvailable() const;
		~Type8();

		Type8(const Type8&) = delete;
		Type8& operator=(const Type8&) = delete;

		Type8(Type8&&);
		Type8& operator=(Type8&&);

		std::string InternalReferenceDesignator()const;
		bool InternalReferenceDesignator(std::string& Str)const noexcept;

		std::string InternalConnectorType()const;
		bool InternalConnectorType(std::string& Str)const noexcept;

		std::string ExternalReferenceDesignator()const;
		bool ExternalReferenceDesignator(std::string& Str)const noexcept;

		std::string ExternalConnectorType()const;
		bool ExternalConnectorType(std::string& Str)const noexcept;

		std::string PortType()const;
		bool PortType(std::string& Str)const noexcept;
	private:
		std::unique_ptr<TypeImpl> pImpl;
	};
}
