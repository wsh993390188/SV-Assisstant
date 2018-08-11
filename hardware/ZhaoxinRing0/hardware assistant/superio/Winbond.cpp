#include "stdafx.h"
#include "SioChip.h"
#include "..\driver\Driverdll.h"

#pragma region Winbond
W836XX::W836XX(std::pair<USHORT, std::string> chip, USHORT LPC_Address) : WINBOND_VENDOR_ID{ 0x5CA3 }, HIGH_BYTE{ 0x80 },
ADDRESS_REGISTER_OFFSET{ 0x05 }, DATA_REGISTER_OFFSET{ 0x06 }, VOLTAGE_VBAT_REG{ 0x51 }, BANK_SELECT_REGISTER{ 0x4E }, VENDOR_ID_REGISTER{ 0x4F }, TEMPERATURE_SOURCE_SELECT_REG{ 0x49 },
TEMPERATURE_REG{ 0x50, 0x50, 0x27 }, TEMPERATURE_BANK{ 1, 2, 0 }, FAN_TACHO_REG{ 0x28, 0x29, 0x2A, 0x3F, 0x53 }, FAN_TACHO_BANK{ 0, 0, 0, 0, 5 }, FAN_BIT_REG{ 0x47, 0x4B, 0x4C, 0x59, 0x5D },
FAN_DIV_BIT0{ 36, 38, 30, 8, 10 }, FAN_DIV_BIT1{ 37, 39, 31, 9, 11 }, FAN_DIV_BIT2{ 5, 6, 7, 23, 15 }, peciTemperature{}, voltageGain{
	0.008f
}
{
	this->Chip_ID = chip.first;
	this->sensor.ChipName = chip.second;
	this->LPC_Base = LPC_Address;
#pragma region Winbond Initiate
	if (!IsWinbondVendor())
		return;

	sensor.Temperatures.resize(3);
	sensor.Temperatures = { INFINITY, INFINITY, INFINITY };
	BYTE flag = 0;
	switch (chip.first) {
	case W83667HG:
	case W83667HGB:
		// note temperature sensor registers that read PECI
		flag = ReadByte(0, TEMPERATURE_SOURCE_SELECT_REG);
		peciTemperature[0] = (flag & 0x04) != 0;
		peciTemperature[1] = (flag & 0x40) != 0;
		peciTemperature[2] = false;
		break;
	case W83627DHG:
	case W83627DHGP:
		// note temperature sensor registers that read PECI
		flag = ReadByte(0, TEMPERATURE_SOURCE_SELECT_REG);
		peciTemperature[0] = (flag & 0x07) != 0;
		peciTemperature[1] = (flag & 0x70) != 0;
		peciTemperature[2] = false;
		break;
	default:
		// no PECI support
		peciTemperature[0] = false;
		peciTemperature[1] = false;
		peciTemperature[2] = false;
		break;
	}

	switch (chip.first) {
	case W83627EHF:
		sensor.Voltages.resize(10);
		sensor.Voltages = { INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY };
		voltageRegister = {
			0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x50, 0x51, 0x52 };
		voltageBank = { 0, 0, 0, 0, 0, 0, 0, 5, 5, 5 };
		voltageGain = 0.008f;
		sensor.Fans.resize(5);
		sensor.Fans = { INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY };
		break;
	case W83627DHG:
	case W83627DHGP:
	case W83667HG:
	case W83667HGB:
		sensor.Voltages.resize(9);
		sensor.Voltages = { INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY };
		voltageRegister = {
			0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x50, 0x51 };
		voltageBank = { 0, 0, 0, 0, 0, 0, 0, 5, 5 };
		voltageGain = 0.008f;
		sensor.Fans.resize(5);
		sensor.Fans = { INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY };
		break;
	case W83627HF:
	case W83627THF:
	case W83687THF:
		sensor.Voltages.resize(7);
		sensor.Voltages = { INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY ,INFINITY };
		voltageRegister = {
			0x20, 0x21, 0x22, 0x23, 0x24, 0x50, 0x51 };
		voltageBank = { 0, 0, 0, 0, 0, 5, 5 };
		voltageGain = 0.016f;
		sensor.Fans.resize(3);
		sensor.Fans = { INFINITY ,INFINITY ,INFINITY };
		break;
	}
#pragma endregion
#ifdef ZX_OutputLog
	std::string ttt = boost::str(boost::format("LPC Address: 0x%lx") % LPC_Address);
	Logger::Instance()->OutputLogInfo(el::Level::Debug, ttt);
	ttt = boost::str(boost::format("LPC VenderId: 0x%lx") % WINBOND_VENDOR_ID);
	Logger::Instance()->OutputLogInfo(el::Level::Debug, ttt);
#endif // ZX_OutputLog
}
void W836XX::UpdateData()
{
	for (int i = 0; i < sensor.Voltages.size(); i++) {
		if (voltageRegister[i] != VOLTAGE_VBAT_REG) {
			// two special VCore measurement modes for W83627THF
			float fvalue;
			if ((Chip_ID == W83627HF || Chip_ID == W83627THF ||
				Chip_ID == W83687THF) && i == 0)
			{
				BYTE vrmConfiguration = ReadByte(0, 0x18);
				int value = ReadByte(voltageBank[i], voltageRegister[i]);
				if ((vrmConfiguration & 0x01) == 0)
					fvalue = 0.016f * value; // VRM8 formula
				else
					fvalue = 0.00488f * value + 0.69f; // VRM9 formula
			}
			else {
				int value = ReadByte(voltageBank[i], voltageRegister[i]);
				fvalue = voltageGain * value;
			}
			if (fvalue > 0)
				sensor.Voltages[i] = fvalue;
			else
				sensor.Voltages[i] = INFINITY;
		}
		else {
			// Battery voltage
			bool valid = (ReadByte(0, 0x5D) & 0x01) > 0;
			if (valid) {
				sensor.Voltages[i] = voltageGain * ReadByte(5, VOLTAGE_VBAT_REG);
			}
			else {
				sensor.Voltages[i] = INFINITY;
			}
		}
	}

	for (int i = 0; i < sensor.Temperatures.size(); i++) {
		int value = ((signed char)ReadByte(TEMPERATURE_BANK[i],
			TEMPERATURE_REG[i])) << 1;
		if (TEMPERATURE_BANK[i] > 0)
			value |= ReadByte(TEMPERATURE_BANK[i],
			(BYTE)(TEMPERATURE_REG[i] + 1)) >> 7;

		float temperature = value / 2.0f;
		if (temperature <= 125 && temperature >= -55 && !peciTemperature[i]) {
			sensor.Temperatures[i] = temperature;
		}
		else {
			sensor.Temperatures[i] = INFINITY;
		}
	}

	ULONG bits = 0;
	for (int i = 0; i < FAN_BIT_REG.size(); i++)
		bits = (bits << 8) | ReadByte(0, FAN_BIT_REG[i]);
	ULONG newBits = bits;
	for (LONGLONG i = 0; i < sensor.Fans.size(); i++) {
		int count = ReadByte(FAN_TACHO_BANK[i], FAN_TACHO_REG[i]);

		// assemble fan divisor
		int divisorBits = (int)(
			(((bits >> FAN_DIV_BIT2[i]) & 1) << 2) |
			(((bits >> FAN_DIV_BIT1[i]) & 1) << 1) |
			((bits >> FAN_DIV_BIT0[i]) & 1));
		int divisor = 1 << divisorBits;

		float value = (count < 0xff) ? 1.35e6f / (count * divisor) : INFINITY;
		sensor.Fans[i] = value;

		// update fan divisor
		if (count > 192 && divisorBits < 7)
			divisorBits++;
		if (count < 96 && divisorBits > 0)
			divisorBits--;

		newBits = SetBit(newBits, FAN_DIV_BIT2[i], (divisorBits >> 2) & 1);
		newBits = SetBit(newBits, FAN_DIV_BIT1[i], (divisorBits >> 1) & 1);
		newBits = SetBit(newBits, FAN_DIV_BIT0[i], divisorBits & 1);
	}

	// write new fan divisors 
	for (LONGLONG i = FAN_BIT_REG.size() - 1; i >= 0; i--) {
		BYTE oldByte = (BYTE)(bits & 0xFF);
		BYTE newByte = (BYTE)(newBits & 0xFF);
		bits = bits >> 8;
		newBits = newBits >> 8;
		if (oldByte != newByte)
			WriteByte(0, FAN_BIT_REG[i], newByte);
	}
}

BYTE W836XX::ReadByte(BYTE bank, BYTE Register) {
	SV_ASSIST::Ring0::WrIOPort(
		(USHORT)(LPC_Base + ADDRESS_REGISTER_OFFSET), 1, BANK_SELECT_REGISTER);
	SV_ASSIST::Ring0::WrIOPort(
		(USHORT)(LPC_Base + DATA_REGISTER_OFFSET), 1, bank);
	SV_ASSIST::Ring0::WrIOPort(
		(USHORT)(LPC_Base + ADDRESS_REGISTER_OFFSET), 1, Register);
	DWORD temp = 0;
	SV_ASSIST::Ring0::RdIOPort(
		(USHORT)(LPC_Base + DATA_REGISTER_OFFSET), 1, temp);
	return (BYTE)temp;
}

void W836XX::WriteByte(BYTE bank, BYTE Register, BYTE value) {
	SV_ASSIST::Ring0::WrIOPort(
		(USHORT)(LPC_Base + ADDRESS_REGISTER_OFFSET), 1, BANK_SELECT_REGISTER);
	SV_ASSIST::Ring0::WrIOPort(
		(USHORT)(LPC_Base + DATA_REGISTER_OFFSET), 1, bank);
	SV_ASSIST::Ring0::WrIOPort(
		(USHORT)(LPC_Base + ADDRESS_REGISTER_OFFSET), 1, Register);
	SV_ASSIST::Ring0::WrIOPort(
		(USHORT)(LPC_Base + DATA_REGISTER_OFFSET), 1, value);
}

bool W836XX::IsWinbondVendor() {
	USHORT vendorId =
		(USHORT)((ReadByte(HIGH_BYTE, VENDOR_ID_REGISTER) << 8) |
			ReadByte(0, VENDOR_ID_REGISTER));
	return vendorId == WINBOND_VENDOR_ID;
}

ULONG W836XX::SetBit(ULONG target, int bit, int value)
{
	if ((value & 1) != value)
		throw ("Value must be one bit only.");

	if (bit < 0 || bit > 63)
		throw ("Bit out of range.");

	ULONG mask = (((ULONG)1) << bit);
	return value > 0 ? target | mask : target & ~mask;
}
#pragma endregion