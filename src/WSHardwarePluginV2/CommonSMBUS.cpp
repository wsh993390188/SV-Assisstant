#include "stdafx.h"
#include "CommonSMBUS.h"
#include "IoHandle.h"
#include "ExecSpd.h"

namespace Hardware
{
	namespace Memory
	{
#pragma region SMBUS Register
#define PMIO_INDEX_REG			0xCD6
#define PMIO_DATA_REG			0xCD7

#define SMBUS_BAR_LOW_BYTE		0x2C
#define SMBUS_BAR_HIGH_BYTE		0x2D

#define SMBUS_STATUS_REG		0x00
#define SMBUS_SLAVE_STATUS_REG	0x01
#define SMBUS_COMMAND_REG		0x02
#define SMBUS_CONTROL_REG		0x03
#define SMBUS_HOST_CMD_REG		0x04
#define SMBUS_DATA0_REG			0x05
#define SMBUS_CLOCK_REG			0x0E

#define SMBUS_FREQUENCY_CONST	66000000 / 4

		/* Between 1-10 seconds, We should never timeout normally
		* Longer than this is just painful when a timeout condition occurs.
		*/
#define SMBUS_TIMEOUT (100*1000*10)
#pragma endregion

#pragma region SMBUS Command
#define SMBUS_READ_BYTE_COMMAND	0x48
#define SMBUS_READ_COMMAND		0x44
#pragma endregion

#pragma region SMBUS Status
#define SMBHSTSTS_BYTE_DONE	(1 << 7)
#define SMBHSTSTS_INUSE_STS	(1 << 6)
#define SMBHSTSTS_SMBALERT_STS	(1 << 5)
#define SMBHSTSTS_FAILED	(1 << 4)
#define SMBHSTSTS_BUS_ERR	(1 << 3)
#define SMBHSTSTS_DEV_ERR	(1 << 2)
#define SMBHSTSTS_INTR		(1 << 1)
#define SMBHSTSTS_HOST_BUSY	(1 << 0)
#pragma endregion

#pragma region Initialize
		CommonSMBUS::CommonSMBUS()
		{
			if (!GetSMbusBaseAddr(PlatformId, SmbusBase))
				throw std::exception("Platfrom Not Support");
		}

		USHORT CommonSMBUS::ConverterIdToDIMMId(const USHORT& id)
		{
			return 0xA0 + ((id % 8) * 2);
		}

#pragma endregion

#pragma region SMBUS Ready & Done
		bool CommonSMBUS::ReadSPDByte(const uint8_t& Offset, DWORD& val)
		{
			if (!smbus_wait_until_ready())
				return false;
			IoPtr.WriteByte(SmbusBase + SMBUS_CONTROL_REG, Offset);
			IoPtr.WriteByte(SmbusBase + SMBUS_COMMAND_REG, SMBUS_READ_BYTE_COMMAND);
			if (smbus_wait_until_done() < 0)
				return false;
			IoPtr.ReadByte(SmbusBase + SMBUS_DATA0_REG, val);
			return true;
		}
		bool CommonSMBUS::smbus_wait_until_ready()
		{
			ULONG loops = SMBUS_TIMEOUT;
			do {
				DWORD val = {};
				IoPtr.ReadByte(SmbusBase + SMBUS_STATUS_REG, val);
				val &= 0x1f;
				if (val == 0) {	/* ready now */
					return true;
				}
				IoPtr.WriteByte(SmbusBase + SMBUS_STATUS_REG, 0xFF);
			} while (--loops);
			return false;		/* time out */
		}

		int CommonSMBUS::smbus_wait_until_done()
		{
			DWORD loops = SMBUS_TIMEOUT;
			do {
				DWORD val = {};

				IoPtr.ReadByte(SmbusBase + SMBUS_STATUS_REG, val);
				val &= 0x1f;	/* mask off reserved bits */
				if (val & 0x1c) {
					if ((val & SMBHSTSTS_DEV_ERR) == SMBHSTSTS_DEV_ERR)
						return -2;
					if ((val & SMBHSTSTS_BUS_ERR) == SMBHSTSTS_BUS_ERR)
						return -3;
					if ((val & SMBHSTSTS_FAILED) == SMBHSTSTS_FAILED)
						return -4;
					return -5;	/* error */
				}
				if ((val & 0x03) == 0x02) {
					return 0;
				}
			} while (--loops);
			return -1;		/* timeout */
		}

		bool CommonSMBUS::PrepareSmbus()
		{
			if (PlatformId == 0x1002 || PlatformId == 0x1022)
			{
				this->setupFch();
				this->setupFch();
			}
			if (smbus_wait_until_ready())
			{
				SwitchToPage(0x6C);
				return true;
			}
			return false;
		}
#pragma endregion

#pragma region AMD SMBUS need this Function
		void CommonSMBUS::writePmReg(const UINT8& reg, const UINT8& data)
		{
			IoPtr.WriteByte(PMIO_INDEX_REG, reg);
			IoPtr.WriteByte(PMIO_DATA_REG, data);
		}

		void CommonSMBUS::setupFch()
		{
			/* set up SMBUS - Set to SMBUS 0 & set base address */
			/* For SB800 & Hudson1 to SB900 & Hudson 2/3 */
			writePmReg(SMBUS_BAR_HIGH_BYTE, SmbusBase >> 8);
			writePmReg(SMBUS_BAR_LOW_BYTE, (SmbusBase & 0xe0) | 1);

			/* set SMBus clock to 400 KHz */
			IoPtr.WriteByte(SmbusBase + SMBUS_CLOCK_REG, SMBUS_FREQUENCY_CONST / 400000);
		}
		bool CommonSMBUS::SwitchToPage(const UINT8& data)
		{
			IoPtr.WriteByte(SmbusBase + SMBUS_STATUS_REG, 0xFF);
			IoPtr.WriteByte(SmbusBase + SMBUS_HOST_CMD_REG, data);
			IoPtr.WriteByte(SmbusBase + SMBUS_COMMAND_REG, 0x48);
			return smbus_wait_until_ready();
		}
#pragma endregion

#pragma region Judge SPD Type
		bool CommonSMBUS::IsDeviceOnline(const USHORT& DIMMId)
		{
			if (PrepareSmbus())
			{
				IoPtr.WriteByte(SmbusBase + SMBUS_HOST_CMD_REG, ConverterIdToDIMMId(DIMMId) | 1);
				DWORD val = 0;
				return ReadSPDByte(0x02, val);
			}
			return false;
		}

		bool CommonSMBUS::GetSMbusBaseAddr(const USHORT& VendorID, USHORT& SMbusBaseAddress)
		{
			bool Success = false;
			ULONG tmp = 0;
			Utils::Ring0::SafePCIHandle PciPtr;
			USHORT bus = 0, dev = 31, func = 0;
			switch (VendorID)
			{
			case 0x8086:
				if (FindIntelSmbusControl(bus, dev, func))
				{
					if (PciPtr.ReadWORD(bus, dev, func, 0x20, tmp))
					{
						SMbusBaseAddress = (USHORT)((tmp & 0xFFFE));
						PciPtr.ReadByte(bus, dev, func, 0x04, tmp);
						if ((tmp & 3) == 0)
						{
							//Enable Io
							PciPtr.WriteByte(bus, dev, func, 0x04, tmp | 3);
						}
						Success = true;
					}
				}
				break;
			case 0x1106:
				SMbusBaseAddress = 0x400;
				Success = true;
				break;
			case 0x1002:
			case 0x1022:
				SMbusBaseAddress = 0xB00;
				Success = true;
				break;
			default:
				break;
			}

			if (!Success)
			{
				SmbusBase = 0xFFFF;
			}
			return Success;
		}

		bool CommonSMBUS::FindIntelSmbusControl(const USHORT& bus, const USHORT& dev, USHORT& func)
		{
			Utils::Ring0::SafePCIHandle PciPtr;
			DWORD ClassCode = 0;
			DWORD SubClass = 0;
			DWORD VendorId = 0;
			for (std::remove_reference_t<decltype(func)> i = 0; i < 8; ++i)
			{
				if (PciPtr.ReadWORD(bus, dev, i, 0, VendorId))
				{
					if (VendorId != 0x8086)
					{
						continue;
					}
					if (PciPtr.ReadByte(bus, dev, i, 0xB, ClassCode))
					{
						if (PciPtr.ReadByte(bus, dev, i, 0xA, SubClass))
						{
							if (ClassCode == 0xC && SubClass == 0x5)
							{
								func = i;
								return true;
							}
						}
					}
				}
			}
			return false;
		}

		bool CommonSMBUS::ParserSPD(const USHORT& DIMMId, MemoryCommonInformation& MemoryInfo)
		{
			if (PrepareSmbus())
			{
				auto RealDIMMId = ConverterIdToDIMMId(DIMMId);
				IoPtr.WriteByte(SmbusBase + SMBUS_HOST_CMD_REG, RealDIMMId | 1);
				DWORD DDRType = 0;
				if (ReadSPDByte(0x02, DDRType))
				{
					if (IsDDR3(DDRType))
					{
						DDR3_INFO spd{};
						if (ReadSPD(RealDIMMId, spd))
						{
							ExecMemoryDDR3 TempExec(spd);
							TempExec.Execute(MemoryInfo);
							return true;
						}
						else
						{
							spdlog::error("read ddr3 error");
						}
					}
					else if (IsDDR4(DDRType))
					{
						DDR4_INFO spd{};
						if (ReadSPD(RealDIMMId, spd))
						{
							ExecMemoryDDR4 TempExec(spd);
							TempExec.Execute(MemoryInfo);
							return true;
						}
						else
						{
							spdlog::error("read ddr4 error");
						}
					}
					else
					{
						spdlog::error("unknown ddr type({})", DDRType);
					}
				}
			}
			return false;
		}
#pragma endregion

#pragma region Read SPD
		bool CommonSMBUS::ReadSPD(const USHORT& DIMMId, DDR3_INFO& spd)
		{
			if (PrepareSmbus())
			{
				IoPtr.WriteByte(SmbusBase + SMBUS_HOST_CMD_REG, DIMMId | 1);
				for (ULONG offset = 0; offset < 0x100; offset++)
				{
					if (!smbus_wait_until_ready())
						return false;
					IoPtr.WriteByte(SmbusBase + SMBUS_CONTROL_REG, offset);
					IoPtr.WriteByte(SmbusBase + SMBUS_COMMAND_REG, SMBUS_READ_BYTE_COMMAND);
					if (smbus_wait_until_done() < 0)
						return false;
					DWORD val = 0;
					IoPtr.ReadByte(SmbusBase + SMBUS_DATA0_REG, val);
					memcpy(reinterpret_cast<PBYTE>(&spd) + offset, &val, 1);
				}
				return true;
			}
			return false;
		}

		bool CommonSMBUS::ReadSPD(const USHORT& DIMMId, DDR4_INFO& spd)
		{
			if (PrepareSmbus())
			{
				IoPtr.WriteByte(SmbusBase + SMBUS_HOST_CMD_REG, DIMMId | 1);
				for (ULONG offset = 0; offset < 0x100; offset++)
				{
					DWORD val = 0;
					ReadSPDByte(offset, val);
					memcpy(reinterpret_cast<PBYTE>(&spd) + offset, &val, 1);
				}
				if (SwitchToPage(0x6E))
				{
					IoPtr.WriteByte(SmbusBase + SMBUS_HOST_CMD_REG, DIMMId | 1);
					for (ULONG offset = 0x00; offset < 128; offset++)
					{
						DWORD val = 0;
						ReadSPDByte(offset, val);
						memcpy(reinterpret_cast<PBYTE>(&spd) + offset + 0x100, &val, 1);
					}
				}
				else
				{
					return false;
				}

				SwitchToPage(0x6C);
				return true;
			}
			return false;
		}
#pragma endregion
	}
}