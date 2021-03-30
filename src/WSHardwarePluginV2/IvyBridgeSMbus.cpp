#include "stdafx.h"
#include "IvyBridgeSMbus.h"
#include "ExecSpd.h"

namespace Hardware
{
	namespace Memory
	{
#pragma region Initialize
		IvyBridgeSMbus::IvyBridgeSMbus() : MemoryPtr()
		{
			Utils::Ring0::SafePCIHandle pciPtr;
			DWORD CommandValue = 0;
			DWORD LocalA(0);
			pciPtr.ReadDWORD(0, 5, 0, 0x84, LocalA);
			LocalA &= 0xFC000000;
			DWORD LocalB = (LocalA + 0x28109);
			MemoryPtr.ReadByte(LocalB, LocalB);
			LocalB = (LocalB << 20);
			LocalA = LocalB + LocalA;
			//SmbusBase #1
			SmbusBase1 = LocalA + 0x78180;
			//SmbusBase #2
			SmbusBase2 = LocalA + 0x78190;
		}

		void IvyBridgeSMbus::InitSmbuscontrol(const DWORD& Smbusbase)
		{
			DWORD Local2 = (Smbusbase + 8);
			DWORD Local8 = 0;
			MemoryPtr.ReadDWORD(Local2, Local8);
			Local8 = 0xA8000000;
			MemoryPtr.WriteDWORD(Local2, Local8);
		}
#pragma endregion

#pragma region Read SPD

		IvyBridgeSMbus::IvyBridgeDIMMId IvyBridgeSMbus::ConverterIdToDIMMId(const USHORT& DIMMId)
		{
			IvyBridgeDIMMId ret;
			ret.DIMMId = DIMMId % 16;
			ret.SmbusBase = SmbusBase1;
			return ret;
		}

		bool IvyBridgeSMbus::ParserSPD(const USHORT& DIMMId, MemoryCommonInformation& MemoryInfo)
		{
			DDR3_INFO spd{};
			if (ReadSPD(DIMMId, spd))
			{
				ExecMemoryDDR3 TempExec(spd);
				TempExec.Execute(MemoryInfo);
				return true;
			}
			else
			{
				spdlog::error("read ddr3 spd info error");
			}
			return false;
		}

		bool IvyBridgeSMbus::ReadSPD(const USHORT& DIMMId, DDR3_INFO& spd)
		{
			auto DIMMInfo = ConverterIdToDIMMId(DIMMId);
			if (!ClearStatus(DIMMInfo.SmbusBase))
				return false;
			DWORD Local9 = (DIMMInfo.DIMMId << 24);

			for (size_t offset = 0; offset < 0xFF; offset += 2)
			{
				Local9 |= (offset << 16);
				MemoryPtr.WriteDWORD(DIMMInfo.SmbusBase + 4, Local9);

				if (!WaitByteCmd(DIMMInfo.SmbusBase))
					return false;

				DWORD val = 0, success = 0;
				MemoryPtr.ReadWORD(DIMMInfo.SmbusBase, val);
				val = (((val & 0xFF00) >> 8) | ((val & 0x00FF) << 8));
				MemoryPtr.ReadByte(DIMMInfo.SmbusBase + 3, success);
				if (success & 0x20)
					return false;
				memcpy(reinterpret_cast<PBYTE>(&spd) + offset, &val, 2);
				Local9 = (DIMMInfo.DIMMId << 24);
			}
			return true;
		}

#pragma endregion

#pragma region DIMMbus Commond
		bool IvyBridgeSMbus::WaitByteCmd(DWORD BaseAddress)
		{
			DWORD loops = 0x8000;
			do
			{
				DWORD Local8 = 0;
				MemoryPtr.ReadDWORD(BaseAddress + 4, Local8);
				Local8 &= 0x10000000;
				if ((Local8 & 0x20000000) == 0)
					return true;
			} while (--loops);
			return false;
		}

		bool IvyBridgeSMbus::ClearStatus(DWORD BaseAddress)
		{
			DWORD loops = 0x8000;
			do
			{
				MemoryPtr.WriteDWORD(BaseAddress + 4, 0);
				DWORD Local8 = {};
				MemoryPtr.ReadDWORD(BaseAddress, Local8);
				if ((Local8 & 0x10000000) == 0)
					return true;
			} while (--loops);
			return false;
		}
#pragma endregion

#pragma region Judge SPD
		bool IvyBridgeSMbus::IsDeviceOnline(const USHORT& DIMMId)
		{
			return IsDeviceOnline(ConverterIdToDIMMId(DIMMId));
		}

		bool IvyBridgeSMbus::IsDeviceOnline(const IvyBridgeDIMMId& DIMMId)
		{
			if (!ClearStatus(DIMMId.SmbusBase))
				return false;

			DWORD Local8 = {};
			MemoryPtr.ReadDWORD(DIMMId.SmbusBase + 4, Local8);
			Local8 &= 0xF8FFFFFF;
			DWORD Local9 = (DIMMId.DIMMId << 24);
			Local9 |= (2 << 16);
			MemoryPtr.WriteDWORD(DIMMId.SmbusBase + 4, Local9);

			if (!WaitByteCmd(DIMMId.SmbusBase))
				return false;

			DWORD val = 0, success = 0;
			MemoryPtr.ReadByte(DIMMId.SmbusBase + 1, val);
			MemoryPtr.ReadByte(DIMMId.SmbusBase + 3, success);
			if (success & 0x20)
				return false;
			return true;
		}
#pragma endregion
	}
}