#include "stdafx.h"
#include "CMemory.h"

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
using namespace SV_ASSIST;
Memory::IBaseMemory::IBaseMemory()
{

}

Memory::IBaseMemory::~IBaseMemory()
{

}

SV_ASSIST::Memory::CommonSMBUS::CommonSMBUS()
{
	this->PlatformId = Ring0::GetPCIVendorID();
	Ring0::GetSMbusBaseAddr(PlatformId, SmbusBase);
}

SV_ASSIST::Memory::CommonSMBUS::CommonSMBUS(const USHORT PlatformId, const USHORT SmbusBase)
{
	this->PlatformId = PlatformId;
	this->SmbusBase = SmbusBase;
}

bool SV_ASSIST::Memory::CommonSMBUS::ReadSPD(const DIMMType types, const USHORT DIMMId, PVOID64& spd, const int len, const ULONG SmbusControlBase)
{
	if(((types == DDR4|| types == DDR4E || types == LPDDR4) && len < 384) || ((types == LPDDR3 || types == DDR3 || types == DDR2 || types == DDR) && len < 256))
		return false;

	if (PlatformId == 1002 || PlatformId == 1022)
	{
		this->setupFch();
		this->setupFch();
	}

	if (types == DDR4 || types == LPDDR4)
	{
		if (smbus_wait_until_ready())
		{
			Ring0::WrIOPort(SmbusBase + SMBUS_HOST_CMD_REG, 1, DIMMId | 1);
			for (ULONG offset = 0; offset < 0x100; offset++)
			{
				if (!smbus_wait_until_ready())
					return false;
				Ring0::WrIOPort(SmbusBase + SMBUS_CONTROL_REG, 1, offset);
				Ring0::WrIOPort(SmbusBase + SMBUS_COMMAND_REG, 1, SMBUS_READ_BYTE_COMMAND);
				if (smbus_wait_until_done() < 0)
					return false;
				DWORD val = 0;
				Ring0::RdIOPort(SmbusBase + SMBUS_DATA0_REG, 1, val);
				memcpy((PBYTE)spd + offset, &val, 1);
			}
			Ring0::WrIOPort(SmbusBase + SMBUS_HOST_CMD_REG, 1, 0x6E);
			Ring0::WrIOPort(SmbusBase + SMBUS_CONTROL_REG, 1, 1);
			Ring0::WrIOPort(SmbusBase + SMBUS_COMMAND_REG, 1, 0x40);
			Sleep(1);
			for (ULONG offset = 0x00; offset < 128; offset++)
			{
				if (!smbus_wait_until_ready())
					return false;
				Ring0::WrIOPort(SmbusBase + SMBUS_CONTROL_REG, 1, offset);
				Ring0::WrIOPort(SmbusBase + SMBUS_COMMAND_REG, 1, SMBUS_READ_BYTE_COMMAND);
				if (smbus_wait_until_done() < 0)
					return false;
				DWORD val = 0;
				Ring0::RdIOPort(SmbusBase + SMBUS_DATA0_REG, 1, val);
				memcpy((PBYTE)spd + offset + 0x100, &val, 1);
			}

			Ring0::WrIOPort(SmbusBase + SMBUS_HOST_CMD_REG, 1, 0x6C);
			Ring0::WrIOPort(SmbusBase + SMBUS_CONTROL_REG, 1, 1);
			Ring0::WrIOPort(SmbusBase + SMBUS_COMMAND_REG, 1, 0x40);
			Sleep(1);
		}
		else
			return false;
	}
	else
	{
		if (smbus_wait_until_ready())
		{
			Ring0::WrIOPort(SmbusBase + SMBUS_HOST_CMD_REG, 1, DIMMId | 1);
			for (ULONG offset = 0; offset < 0x100; offset++)
			{
				if (!smbus_wait_until_ready())
					return false;
				Ring0::WrIOPort(SmbusBase + SMBUS_CONTROL_REG, 1, offset);
				Ring0::WrIOPort(SmbusBase + SMBUS_COMMAND_REG, 1, SMBUS_READ_BYTE_COMMAND);
				if (smbus_wait_until_done() < 0)
					return false;
				DWORD val = 0;
				Ring0::RdIOPort(SmbusBase + SMBUS_DATA0_REG, 1, val);
				memcpy((PBYTE)spd + offset, &val, 1);
			}

		}
		else
			return false;
	}
	return true;
}

bool SV_ASSIST::Memory::CommonSMBUS::smbus_wait_until_ready()
{
	ULONG loops = SMBUS_TIMEOUT;
	do {
		DWORD val = {};
		Ring0::RdIOPort(SmbusBase + SMBUS_STATUS_REG,1, val);
		val &= 0x1f;
		if (val == 0) {	/* ready now */
			return true;
		}
		Ring0::WrIOPort(SmbusBase + SMBUS_STATUS_REG,1,0xFE);
	} while (--loops);
	return false;		/* time out */
}

int SV_ASSIST::Memory::CommonSMBUS::smbus_wait_until_done()
{
	DWORD loops = SMBUS_TIMEOUT;
	do {
		DWORD val = {};

		Ring0::RdIOPort(SmbusBase + SMBUS_STATUS_REG, 1, val);
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

#pragma region AMD SMBUS need this Function
void SV_ASSIST::Memory::CommonSMBUS::writePmReg(UINT8 reg, UINT8 data)
{
	Ring0::WrIOPort(PMIO_INDEX_REG,1, reg);
	Ring0::WrIOPort(PMIO_DATA_REG,1, data);
}

void SV_ASSIST::Memory::CommonSMBUS::setupFch()
{
	/* set up SMBUS - Set to SMBUS 0 & set base address */
	/* For SB800 & Hudson1 to SB900 & Hudson 2/3 */
	writePmReg(SMBUS_BAR_HIGH_BYTE, SmbusBase >> 8);
	writePmReg(SMBUS_BAR_LOW_BYTE, (SmbusBase & 0xe0) | 1);

	/* set SMBus clock to 400 KHz */
	Ring0::WrIOPort(SmbusBase + SMBUS_CLOCK_REG, 1, SMBUS_FREQUENCY_CONST / 400000);
}
#pragma endregion

bool SV_ASSIST::Memory::CommonSMBUS::JudgeSPDType(DIMMType& types,const USHORT DIMMId, ULONG& SmbusControlBase)
{
	SmbusControlBase = 0;
	types = DIMM_UNKNOWN;
	if (PlatformId == 1002 || PlatformId == 1022)
	{
		this->setupFch();
		this->setupFch();
	}
	if (smbus_wait_until_ready())
	{
		Ring0::WrIOPort(SmbusBase + SMBUS_HOST_CMD_REG, 1, DIMMId | 1);
		Ring0::WrIOPort(SmbusBase + SMBUS_CONTROL_REG, 1, 0x02);
		Ring0::WrIOPort(SmbusBase + SMBUS_COMMAND_REG, 1, SMBUS_READ_BYTE_COMMAND);
		if (smbus_wait_until_done() < 0)
			return false;
		DWORD val = 0;
		Ring0::RdIOPort(SmbusBase + SMBUS_DATA0_REG, 1, val);
		switch (val)
		{
		case 6:
		case 7:
			types = DDR;
			break;
		case 8:
		case 9:
		case 10:
			types = DDR2;
			break;
		case 11:
			types = DDR3;
			break;
		case 12:
			types = DDR4;
			break;
		case 14:
			types = DDR4E;
			break;
		case 15:
			types = LPDDR3;
			break;
		case 16:
			types = LPDDR4;
			break;
		default:
			types = DIMM_UNKNOWN;
			break;
		}
		SmbusControlBase = SmbusBase;
	}
	else
		return false;
	return true;
}

SV_ASSIST::Memory::IvyBridgeSMbus::IvyBridgeSMbus()
{
	PCI_COMMON_CONFIG pci = {};
	Ring0::ReadPci(0, 5, 0, pci);
	DWORD LocalA = (pci.DeviceSpecific[68]) | (pci.DeviceSpecific[69] << 8) | (pci.DeviceSpecific[70] << 16) | (pci.DeviceSpecific[71] << 24);
	LocalA &= 0xFC000000;
	DWORD LocalB = (LocalA + 0x28109);
	Ring0::RdMemory(LocalB, 1, LocalB);
	LocalB = (LocalB << 20);
	LocalA = LocalB + LocalA;
	//SmbusBase #1
	SmbusBase1 = LocalA + 0x78180;
	//SmbusBase #2
	SmbusBase2 = LocalA + 0x78190;

}

bool SV_ASSIST::Memory::IvyBridgeSMbus::ReadSPD(const DIMMType types, const USHORT DIMMId, PVOID64 & spd, const int len, const ULONG SmbusControlBase)
{
	if (((types == LPDDR3 || types == DDR3 || types == DDR2 || types == DDR) && len < 0x100) ||( (types == DDR4 || types == DDR4E || types == LPDDR4) && len < 384) || SmbusControlBase == 0)
		return false;
	InitSmbuscontrol(SmbusBase1);
	InitSmbuscontrol(SmbusBase2);
	return ReadSPD(types, DIMMId, SmbusControlBase, spd, len);
}

bool SV_ASSIST::Memory::IvyBridgeSMbus::ReadSPD(const DIMMType types, const USHORT DIMMId, const ULONG SmbusBase, PVOID64& spd, const int len)
{
	if (!ClearStatus(SmbusBase))
		return false;
	DWORD Local9 = (DIMMId << 24);

	for (size_t offset = 0; offset < 0xFF; offset += 2)
	{
		Local9 |= (offset << 16);
		Ring0::WrMemory(SmbusBase + 4, 4, Local9);

		if (!WaitByteCmd(SmbusBase))
			return false;

		DWORD val = 0, success = 0;
		Ring0::RdMemory(SmbusBase, 2, val);
		val = (((val & 0xFF00) >> 8) | ((val & 0x00FF) << 8));
		Ring0::RdMemory(SmbusBase + 3, 1, success);
		if (success & 0x20)
			return false;
		memcpy((PBYTE)spd + offset, &val, 2);
		Local9 = (DIMMId << 24);
	}
	return true;
}

void SV_ASSIST::Memory::IvyBridgeSMbus::InitSmbuscontrol(const DWORD Smbusbase)
{
	DWORD Local2 = (Smbusbase + 8);
	DWORD Local8 = 0;
	Ring0::RdMemory(Local2, 4, Local8);
	Local8 = 0xA8000000;
	Ring0::WrMemory(Local2, 4, Local8);
}


bool SV_ASSIST::Memory::IvyBridgeSMbus::JudgeSPDType(DIMMType& types, const USHORT DIMMId, ULONG& SmbusControlBase)
{
	InitSmbuscontrol(SmbusBase1);
	InitSmbuscontrol(SmbusBase2);
	bool state = false;
	if (SmbusControlBase == 1)
	{
		SmbusControlBase = 0;
		state = JudgeSPDType(types, DIMMId, SmbusBase1, SmbusControlBase);
	}
	else
		state = JudgeSPDType(types, DIMMId, SmbusBase2, SmbusControlBase);
	return state;
}

bool SV_ASSIST::Memory::IvyBridgeSMbus::WaitByteCmd(DWORD BaseAddress)
{
	DWORD loops = 0x8000;
	do 
	{
		DWORD Local8 = 0;
		Ring0::RdMemory(BaseAddress + 4, 4, Local8);
		Local8 &= 0x10000000;
		if ((Local8 & 0x20000000) == 0)
			return true;
	} while (--loops);
	return false;
}

bool SV_ASSIST::Memory::IvyBridgeSMbus::ClearStatus(DWORD BaseAddress)
{
	DWORD loops = 0x8000;
	do
	{
		Ring0::WrIOPort(BaseAddress + 4, 4, 0);
		DWORD Local8 = {};
		Ring0::RdMemory(BaseAddress, 4, Local8);
		if((Local8 & 0x10000000) == 0)
			return true;
	} while (--loops);
	return false;
}

bool SV_ASSIST::Memory::IvyBridgeSMbus::JudgeSPDType(DIMMType & types, const USHORT DIMMId, const ULONG SmbusBase, ULONG& SmbusControlBase)
{
	if (!ClearStatus(SmbusBase))
		return false;

	DWORD Local8 = {};
	Ring0::RdMemory(SmbusBase + 4, 4, Local8);
	Local8 &= 0xF8FFFFFF;
	DWORD Local9 = (DIMMId << 24);
	Local9 |= (2 << 16);
	Ring0::WrMemory(SmbusBase + 4, 4, Local9);

	if (!WaitByteCmd(SmbusBase))
		return false;

	DWORD val = 0, success = 0;
	Ring0::RdMemory(SmbusBase + 1, 1, val); 
	Ring0::RdMemory(SmbusBase + 3, 1, success);
	if (success & 0x20)
		return false;
	switch (val)
	{
	case 6:
	case 7:
		types = DDR;
		break;
	case 8:
	case 9:
	case 10:
		types = DDR2;
		break;
	case 11:
		types = DDR3;
		break;
	case 12:
		types = DDR4;
		break;
	case 14:
		types = DDR4E;
		break;
	case 15:
		types = LPDDR3;
		break;
	case 16:
		types = LPDDR4;
		break;
	default:
		types = DIMM_UNKNOWN;
		break;
	}
	SmbusControlBase = SmbusBase;
	return true;
}

