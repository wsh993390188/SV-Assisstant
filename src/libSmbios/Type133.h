#pragma once
#include <memory>
namespace Smbios
{
	class TypeImpl;
	namespace Lenovo
	{
		class Type133 final
		{
		public:
			explicit Type133(const int32_t& TableNum);
			Type133(void* PhysicalAddress);
			bool IsAvailable() const;
			~Type133();

			Type133(const Type133&) = delete;
			Type133& operator=(const Type133&) = delete;

			Type133(Type133&&);
			Type133& operator=(Type133&&);

			std::string OEMData() const;
			bool OEMData(std::string& Str) const noexcept;
		private:
			std::unique_ptr<Smbios::TypeImpl> pImpl;
		};
	}
}
