#pragma once
#include "BaseSMBUS.h"
#include "MemoryHandle.h"
namespace Hardware
{
	namespace Memory
	{
		/// @brief IvyBridge�ܹ���SMbus������
		class IvyBridgeSMbus final : public SMBUSControllerBase
		{
		public:
			IvyBridgeSMbus();

			bool IsDeviceOnline(const USHORT& DIMMId) override;
		private:
			struct IvyBridgeDIMMId
			{
				USHORT DIMMId;
				ULONG SmbusBase;
			};
			IvyBridgeDIMMId ConverterIdToDIMMId(const USHORT& DIMMId);
			bool IsDeviceOnline(const IvyBridgeDIMMId& DIMMId);

			/// @brief ��ȡ������SPD����Ϣ
			/// @param DIMMId DIMM Id
			/// @param MemoryInfo SPDͨ����Ϣ
			/// @return �����Ƿ�ɹ�
			bool ParserSPD(const USHORT& DIMMId, MemoryCommonInformation& MemoryInfo)override final;

			bool ReadSPD(const USHORT& DIMMId, DDR3_Normal_Info& spd);
			void InitSmbuscontrol(const DWORD& Smbusbase);
			bool WaitByteCmd(DWORD BaseAddress);
			bool ClearStatus(DWORD BaseAddress);
		private:
			ULONG SmbusBase1;
			ULONG SmbusBase2;
			Utils::Ring0::SafeMemoryHandle MemoryPtr;
		};
	}
}
