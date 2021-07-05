#pragma once
#include "BaseSMBUS.h"
#include "MemoryHandle.h"
namespace Hardware
{
	namespace Memory
	{
		/// @brief IvyBridge架构下SMbus控制器
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

			/// @brief 读取并解析SPD的信息
			/// @param DIMMId DIMM Id
			/// @param MemoryInfo SPD通用信息
			/// @return 解析是否成功
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
