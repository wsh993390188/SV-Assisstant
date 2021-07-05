#include "stdafx.h"
#include "CommonSMBUS.h"
#include "IoHandle.h"
#include "ParserSPD.h"

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
		bool CommonSMBUS::smbus_wait_until_ready()
		{
			ULONG loops = SMBUS_TIMEOUT;
			do {
				DWORD val = {};
				if (!IoPtr.ReadByte(SmbusBase + SMBUS_STATUS_REG, val))
					return false;
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

				if (!IoPtr.ReadByte(SmbusBase + SMBUS_STATUS_REG, val))
					return -6;
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

#pragma endregion

#pragma region Read SPD
		bool CommonSMBUS::ParserSPD(const USHORT& DIMMId, MemoryCommonInformation& MemoryInfo)
		{
			if (PrepareSmbus())
			{
				auto RealDIMMId = ConverterIdToDIMMId(DIMMId);

				return ReadSPD(RealDIMMId, MemoryInfo);
			}
			return false;
		}

		bool CommonSMBUS::ReadSPD(const USHORT& DIMMId, MemoryCommonInformation& data)
		{
			IoPtr.WriteByte(SmbusBase + SMBUS_HOST_CMD_REG, DIMMId | 1);
			DWORD DDRSize = 0, DDRType = 0;
			if (ReadSPDByte(0x00, DDRSize) && ReadSPDByte(0x02, DDRType))
			{
				AllocateSPDSize(DDRSize, DDRType);
				auto DeviceType = ParseDeviceType(DDRType);
				if (IsDDR3Device(DeviceType))
				{
					if (ReadDDR3SPD(DIMMId))
					{
						ParserDDR3SPD Parser;
						if (SPDData.size() >= sizeof(DDR3_INFO_FIRST_128))
							Parser.ParserFirstSPD(reinterpret_cast<const DDR3_INFO_FIRST_128*>(SPDData.data()), data);
						if (SPDData.size() >= sizeof(DDR3_INFO_FIRST_128) + sizeof(DDR3_INFO_SECOND_128))
							Parser.ParserSecondSPD(reinterpret_cast<const DDR3_INFO_SECOND_128*>(SPDData.data() + sizeof(DDR3_INFO_FIRST_128)), data);
						return true;
					}
				}
				else if (IsDDR4Device(DeviceType))
				{
					if (ReadDDR4SPD(DIMMId))
					{
						ParserDDR4SPD Parser;
						if (SPDData.size() >= sizeof(DDR4_INFO_FIRST_128))
							Parser.ParserFirstSPD(reinterpret_cast<const DDR4_INFO_FIRST_128*>(SPDData.data()), data);
						if (SPDData.size() >= sizeof(DDR4_INFO_FIRST_128) + sizeof(DDR4_INFO_SECOND_128))
							Parser.ParserSecondSPD(reinterpret_cast<const DDR4_INFO_SECOND_128*>(SPDData.data() + sizeof(DDR4_INFO_FIRST_128)), data);
						if (SPDData.size() >= sizeof(DDR4_INFO_FIRST_128) + sizeof(DDR4_INFO_SECOND_128) + sizeof(DDR4_INFO_THIRD_128))
							Parser.ParserThirdSPD(reinterpret_cast<const DDR4_INFO_THIRD_128*>(SPDData.data() + sizeof(DDR4_INFO_FIRST_128) + sizeof(DDR4_INFO_SECOND_128)), data);
						if (SPDData.size() >= sizeof(DDR4_INFO_FIRST_128) + sizeof(DDR4_INFO_SECOND_128) + sizeof(DDR4_INFO_THIRD_128) + sizeof(DDR4_INFO_FOURTH_128))
							Parser.ParserFourthSPD(reinterpret_cast<const DDR4_INFO_FOURTH_128*>(SPDData.data() + sizeof(DDR4_INFO_FIRST_128) + sizeof(DDR4_INFO_SECOND_128) + sizeof(DDR4_INFO_THIRD_128)), data);
						return true;
					}
				}
				else if (IsDDR5Device(DeviceType))
				{
					if (ReadDDR5SPD(DIMMId))
						return true;
				}
				else
				{
					spdlog::error("unknown device type ({})", DeviceType);
				}
			}
			else
			{
				spdlog::error("read spd byte 0 | 2 error");
			}
			return false;
		}

		bool CommonSMBUS::ReadDDR3SPD(const USHORT& DIMMId)
		{
			assert(SPDData.size() <= 256);
			if (PrepareSmbus())
			{
				IoPtr.WriteByte(SmbusBase + SMBUS_HOST_CMD_REG, DIMMId | 1);
				for (ULONG offset = 0; offset < SPDData.size(); offset++)
				{
					DWORD val = 0;
					if (ReadSPDByte(offset, val))
						SPDData[offset] = (BYTE)val;
				}
				return true;
			}
			return false;
		}

		bool CommonSMBUS::ReadDDR4SPD(const USHORT& DIMMId)
		{
			assert(SPDData.size() <= 512);
			if (PrepareSmbus())
			{
				IoPtr.WriteByte(SmbusBase + SMBUS_HOST_CMD_REG, DIMMId | 1);

				if (SPDData.size() > 256)
				{
					for (ULONG offset = 0; offset < 0x100; offset++)
					{
						DWORD val = 0;
						if (ReadSPDByte(offset, val))
							SPDData[offset] = val;
					}
					if (SwitchToPage(0x6E))
					{
						IoPtr.WriteByte(SmbusBase + SMBUS_HOST_CMD_REG, DIMMId | 1);
						for (ULONG offset = 0x00; offset < SPDData.size() - 0x100; offset++)
						{
							DWORD val = 0;
							if (ReadSPDByte(offset, val))
								SPDData[offset + 0x100ul] = val;
						}
					}
					else
					{
						return false;
					}
					SwitchToPage(0x6C);
				}
				else
				{
					for (ULONG offset = 0; offset < SPDData.size(); offset++)
					{
						DWORD val = 0;
						if (ReadSPDByte(offset, val))
							SPDData[offset] = val;
					}
				}
				return true;
			}
			return false;
		}

		bool CommonSMBUS::ReadDDR5SPD(const USHORT& DIMMId)
		{
			return false;
		}

		bool CommonSMBUS::ReadSPDByte(const uint8_t& Offset, DWORD& val)
		{
			if (!smbus_wait_until_ready())
				return false;
			IoPtr.WriteByte(SmbusBase + SMBUS_CONTROL_REG, Offset);
			IoPtr.WriteByte(SmbusBase + SMBUS_COMMAND_REG, SMBUS_READ_BYTE_COMMAND);
			if (smbus_wait_until_done() < 0)
				return false;
			return IoPtr.ReadByte(SmbusBase + SMBUS_DATA0_REG, val);
		}

		void CommonSMBUS::AllocateSPDSize(const uint8_t& DDRSize, const uint8_t& DDRType)
		{
			auto DDRTotalSize = Utils::extract_bits_ui(DDRSize, 4, 6);
			auto DDRUsedSize = Utils::extract_bits_ui(DDRSize, 0, 3);
			if ((DDRTotalSize == 0) && (DDRUsedSize == 0))
			{
				if (IsDDR3Device(DeviceType(DDRType)))
					SPDData.resize(256, 0xFF);
				else if (IsDDR4Device(DeviceType(DDRType)))
					SPDData.resize(384, 0xFF);
				else if (IsDDR5Device(DeviceType(DDRType)))
					SPDData.resize(512, 0xFF);
			}
			else if ((DDRTotalSize != 0) && (DDRUsedSize == 0))
			{
				if (DDRTotalSize == 1)
					SPDData.resize(256, 0xFF);
				else if (DDRTotalSize == 2)
					SPDData.resize(512, 0xFF);
				else
					spdlog::error("spd total size using reserved bit Byte: {0} TotalSize: {1}", DDRSize, DDRTotalSize);
			}
			else
			{
				if (DDRUsedSize == 0b0001)
					SPDData.resize(128, 0xFF);
				else if (DDRUsedSize == 0b0010)
					SPDData.resize(256, 0xFF);
				else if (DDRUsedSize == 0b0011)
					SPDData.resize(384, 0xFF);
				else if (DDRUsedSize == 0b0100)
					SPDData.resize(512, 0xFF);
				else
					spdlog::error("spd used size using reserved bit Byte: {0} UsedSize: {1}", DDRSize, DDRUsedSize);
			}
		}
#pragma endregion
	}
}