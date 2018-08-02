#include "stdafx.h"
#include "SioChip.h"
#include "..\..\Driverdll\Driverdll.h"

#pragma region Fintek
F718XX::F718XX(std::pair<USHORT, std::string> chip, USHORT LPC_Address) : ADDRESS_REGISTER_OFFSET{ 0x05 }, DATA_REGISTER_OFFSET{ 0x06 }, VOLTAGE_BASE_REG{ 0x20 }, TEMPERATURE_CONFIG_REG{ 0x69 },
TEMPERATURE_BASE_REG{ 0x70 }, FAN_TACHOMETER_REG{ 0xA0, 0xB0, 0xC0, 0xD0 }
{
#ifdef ZX_OutputLog
	std::string ttt = boost::str(boost::format("LPC Address: 0x%lx") % LPC_Address);
	Logger::Instance()->OutputLogInfo(el::Level::Debug, ttt);
#endif // ZX_OutputLog
	this->Chip_Id = chip.first;
	this->sensor.ChipName = chip.second;
	this->LPC_Base = LPC_Address;

	if (chip.first == F71858)
	{
		sensor.Voltages.resize(3);
		sensor.Voltages = { INFINITY ,INFINITY ,INFINITY };
	}
	else
	{
		sensor.Voltages.resize(9);
		sensor.Voltages = { INFINITY ,INFINITY ,INFINITY,INFINITY ,INFINITY ,INFINITY,INFINITY ,INFINITY ,INFINITY };
	}
	if (chip.first == F71808E)
	{
		sensor.Temperatures.resize(2);
		sensor.Temperatures = { INFINITY ,INFINITY };
	}
	else
	{
		sensor.Temperatures.resize(3);
		sensor.Temperatures = { INFINITY ,INFINITY };
	}
	if (chip.first == F71882 || chip.first == F71858)
	{
		sensor.Fans.resize(4);
		sensor.Fans = { INFINITY ,INFINITY,INFINITY ,INFINITY };
	}
	else
	{
		sensor.Fans.resize(3);
		sensor.Fans = { INFINITY ,INFINITY,INFINITY };
	}
	sensor.Controls.resize(0);
}
void F718XX::UpdateData()
{
	for (int i = 0; i < sensor.Voltages.size(); i++) {
		if (Chip_Id == F71808E && i == 6) {
			// 0x26 is reserved on F71808E
			sensor.Voltages[i] = 0;
		}
		else {
			int value = ReadByte((BYTE)(VOLTAGE_BASE_REG + i));
			sensor.Voltages[i] = 0.008f * value;
		}
	}

	for (int i = 0; i < sensor.Temperatures.size(); i++) {
		switch (Chip_Id) {
		case F71858: {
			int tableMode = 0x3 & ReadByte(TEMPERATURE_CONFIG_REG);
			int high =
				ReadByte((BYTE)(TEMPERATURE_BASE_REG + 2 * i));
			int low =
				ReadByte((BYTE)(TEMPERATURE_BASE_REG + 2 * i + 1));
			if (high != 0xbb && high != 0xcc) {
				int bits = 0;
				switch (tableMode) {
				case 0: bits = 0; break;
				case 1: bits = 0; break;
				case 2: bits = (high & 0x80) << 8; break;
				case 3: bits = (low & 0x01) << 15; break;
				}
				bits |= high << 7;
				bits |= (low & 0xe0) >> 1;
				short value = (short)(bits & 0xfff0);
				sensor.Temperatures[i] = value / 128.0f;
			}
			else {
				sensor.Temperatures[i] = INFINITY;
			}
		} break;
		default: {
			signed char value = (signed char)ReadByte((BYTE)(
				TEMPERATURE_BASE_REG + 2 * (i + 1)));
			if (value < 128 && value > 0)
				sensor.Temperatures[i] = value;
			else
				sensor.Temperatures[i] = INFINITY;
		} break;
		}
	}

	for (int i = 0; i <sensor.Fans.size(); i++) {
		int value = ReadByte(FAN_TACHOMETER_REG[i]) << 8;
		value |= ReadByte((BYTE)(FAN_TACHOMETER_REG[i] + 1));

		if (value > 0)
			sensor.Fans[i] = (value < 0x0fff) ? 1.5e6f / value : 0;
		else
			sensor.Fans[i] = INFINITY;
	}
}

BYTE F718XX::ReadByte(BYTE Register) {
	SV_ASSIST::Ring0::WrIOPort(
		(USHORT)(LPC_Base + ADDRESS_REGISTER_OFFSET), 1, Register);
	DWORD temp = 0;
	SV_ASSIST::Ring0::RdIOPort((USHORT)(LPC_Base + DATA_REGISTER_OFFSET), 1, temp);
	return (BYTE)temp;
}
#pragma endregion