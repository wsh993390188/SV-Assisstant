#pragma once
namespace Hardware
{
	namespace NIC
	{
		class NICIFEnry final
		{
		public:
			bool QueryPackages(const uint32_t IFIndex, uint64_t& InOctets, uint64_t& OutOctets);
			bool QueryGUID(const uint32_t IFIndex, GUID& guid);
		};
	}
}