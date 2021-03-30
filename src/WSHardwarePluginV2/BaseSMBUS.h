#pragma once
#include <windows.h>
#include "PCIHandle.h"
#include "memorydefination.h"

namespace Hardware
{
	namespace Memory
	{
		class SMBUSControllerBase
		{
		public:
			SMBUSControllerBase()
			{
				Hardware::Utils::Ring0::SafePCIHandle PciPtr;
				DWORD value = {};
				if (PciPtr.ReadWORD(0, 0, 0, 0, value))
					this->PlatformId = value;
				else
					this->PlatformId = 0xFFFF;
			}

			virtual ~SMBUSControllerBase() = default;

			virtual bool IsDeviceOnline(const USHORT& DIMMId) = 0;

			virtual bool ParserSPD(const USHORT& DIMMId, MemoryCommonInformation& MemoryInfo) = 0;

		protected:
			/// @brief 主板平台的VendorId
			USHORT PlatformId;

			/// @brief 是否是DDR3的内存
			/// @param DDRTypes SPD的Type位
			/// @return 是否是DDR3
			bool IsDDR3(const uint8_t& DDRTypes);

			/// @brief 是否是DDR4的内存
			/// @param DDRTypes SPD的Type位
			/// @return 是否是DDR4
			bool IsDDR4(const uint8_t& DDRTypes);

		private:
			SMBUSControllerBase(const SMBUSControllerBase&) = delete;
			SMBUSControllerBase& operator=(const SMBUSControllerBase&) = delete;
		};
	}
}
