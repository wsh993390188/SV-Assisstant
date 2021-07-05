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
			/// @brief ����ƽ̨��VendorId
			USHORT PlatformId;

			/// @brief �����ڴ������
			/// @param DDRTypes SPD��Typeλ
			/// @return �ڴ��豸����
			DeviceType ParseDeviceType(const uint8_t& DDRTypes);

			/// @brief �Ƿ���DDR3���ڴ�
			/// @param type SPD��Typeλ
			/// @return �Ƿ���DDR3
			bool IsDDR3Device(const DeviceType& type);

			/// @brief �Ƿ���DDR4���ڴ�
			/// @param type SPD��Typeλ
			/// @return �Ƿ���DDR4
			bool IsDDR4Device(const DeviceType& type);

			/// @brief �Ƿ���DDR5���ڴ�
			/// @param type SPD��Typeλ
			/// @return �Ƿ���DDR5
			bool IsDDR5Device(const DeviceType& type);
		private:
			SMBUSControllerBase(const SMBUSControllerBase&) = delete;
			SMBUSControllerBase& operator=(const SMBUSControllerBase&) = delete;
		};
	}
}
