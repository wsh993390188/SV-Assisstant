#pragma once
#include "memorydefination.h"
#include "..\driver\Driverdll.h"

namespace SV_ASSIST
{
	namespace Memory
	{
		class IBaseMemory
		{
		public:
			IBaseMemory();
			virtual bool JudgeSPDType(DIMMType& types, const USHORT DIMMId, ULONG& SmbusControlBase) = 0;
			virtual bool ReadSPD(const DIMMType types, const USHORT DIMMId, PVOID64& spd, const int len, const ULONG SmbusControlBase = 0) = 0;
			~IBaseMemory();
		protected:
			USHORT PlatformId;
		private:
			IBaseMemory(const IBaseMemory& that);
			const IBaseMemory& operator=(const IBaseMemory& that) {};
		};

		class CommonSMBUS final : public IBaseMemory
		{
		public:
			CommonSMBUS();
			explicit CommonSMBUS(const USHORT PlatformId, const USHORT SmbusBase);
			bool ReadSPD(const DIMMType types, const USHORT DIMMId, PVOID64 &spd, const int len, const ULONG SmbusControlBase = 0) override;
			bool JudgeSPDType(DIMMType& types, const USHORT DIMMId, ULONG& SmbusControlBase) override;
		private:
			bool smbus_wait_until_ready();
			int smbus_wait_until_done();
			void writePmReg(UINT8 reg, UINT8 data);
			void setupFch();
		private:
			USHORT SmbusBase;
		};

		class IvyBridgeSMbus final : public IBaseMemory
		{
		public:
			IvyBridgeSMbus();
			bool ReadSPD(const DIMMType types, const USHORT DIMMId, PVOID64 &spd, const int len, const ULONG SmbusControlBase = 0) override;
			void InitSmbuscontrol(const DWORD Smbusbase);
			bool JudgeSPDType(DIMMType& types, const USHORT DIMMId, ULONG& SmbusControlBase) override;
		private:
			bool ReadSPD(const DIMMType types, const USHORT DIMMId, const ULONG SmbusBase, PVOID64 & spd, const int len);
			bool WaitByteCmd(DWORD BaseAddress);
			bool ClearStatus(DWORD BaseAddress);
			bool JudgeSPDType(DIMMType& types, const USHORT DIMMId, const ULONG SmbusBase, ULONG& SmbusControlBase);
		private:
			ULONG SmbusBase1;
			ULONG SmbusBase2;
		};
	}
}
