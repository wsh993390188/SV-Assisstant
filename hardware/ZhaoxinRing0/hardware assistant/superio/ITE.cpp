#include "stdafx.h"
#include "SioChip.h"
#include "..\..\Driverdll\Driverdll.h"


#pragma region ITE
IT87XX::IT87XX(std::pair<USHORT, std::string> chip, USHORT LPC_Address) : ITE_VENDOR_ID(0x90), CONFIGURATION_REGISTER(0x00),
TEMPERATURE_BASE_REG{ 0x29 }, VENDOR_ID_REGISTER{ 0x58 }, FAN_TACHOMETER_DIVISOR_REGISTER{ 0x0B },
FAN_TACHOMETER_REG{ 0x0d, 0x0e, 0x0f, 0x80, 0x82 }, FAN_TACHOMETER_EXT_REG{ 0x18, 0x19, 0x1a, 0x81, 0x83 }, VOLTAGE_BASE_REG{ 0x20 }, FAN_PWM_CTRL_REG{ 0x15, 0x16, 0x17 }
{
	this->sensor.ChipName = chip.second;
	this->LPC_Base = LPC_Address;
#pragma region ITE initiate
	this->addressReg = (USHORT)(LPC_Address + 0x05);
	this->dataReg = (USHORT)(LPC_Address + 0x06);

	// Check vendor id
	bool valid = {};
	BYTE vendorId = ReadByte(VENDOR_ID_REGISTER, valid);
	if (!valid || vendorId != ITE_VENDOR_ID)
		return;

	// Bit 0x10 of the configuration register should always be 1
	// 	if ((ReadByte(CONFIGURATION_REGISTER, valid) & 0x10) == 0)
	// 		return;
	// 	if (!valid)
	// 		return;

	this->sensor.Voltages = { INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY,INFINITY, INFINITY, INFINITY };
	this->sensor.Temperatures = { INFINITY, INFINITY, INFINITY };
	if (chip.first == IT8705F)
	{
		this->sensor.Fans = { INFINITY, INFINITY, INFINITY };
	}
	else
	{
		this->sensor.Fans = { INFINITY, INFINITY, INFINITY, INFINITY, INFINITY };
	}
	this->sensor.Controls = { INFINITY, INFINITY, INFINITY };

	// IT8620E, IT8628E, IT8721F, IT8728F and IT8772E use a 12mV resultion 
	// IT8625E use a 11mV
	// IT8665E use a 10.9mV
	// ADC, all others 16mV
	if (chip.first == IT8620E || chip.first == IT8628E || chip.first == IT8721F
		|| chip.first == IT8728F || chip.first == IT8771E || chip.first == IT8772E)
		voltageGain = 0.012f;
	else if (chip.first == IT8665E)
		voltageGain = 0.0109f;
	else
		voltageGain = 0.016f;

	// older IT8705F and IT8721F revisions do not have 16-bit fan counters
	if ((chip.first == IT8705F /*&& version < 3*/) ||
		(chip.first == IT8712F/* && version < 8*/))
	{
		has16bitFanCounter = false;
	}
	else {
		has16bitFanCounter = true;
	}
#pragma endregion

#ifdef ZX_OutputLog
	std::string ttt = boost::str(boost::format("LPC Address: 0x%lx") % LPC_Address);
	Logger::Instance()->OutputLogInfo(el::Level::Debug, ttt);
	ttt = boost::str(boost::format("LPC VenderId: 0x%lx") % vendorId);
	Logger::Instance()->OutputLogInfo(el::Level::Debug, ttt);
#endif // ZX_OutputLog
}
void IT87XX::UpdateData()
{
	for (int i = 0; i < sensor.Voltages.size(); i++) {
		bool valid;

		float value =
			voltageGain * ReadByte((BYTE)(VOLTAGE_BASE_REG + i), valid);

		if (!valid)
			continue;
		if (value > 0)
			sensor.Voltages[i] = value;
		else
			sensor.Voltages[i] = INFINITY;
	}

	for (int i = 0; i < sensor.Temperatures.size(); i++) {
		bool valid;
		signed char value = (signed char)ReadByte(
			(BYTE)(TEMPERATURE_BASE_REG + i), valid);
		if (!valid)
			continue;

		if (value < 128 && value > 0)
			sensor.Temperatures[i] = value;
		else
			sensor.Temperatures[i] = INFINITY;
	}

	if (has16bitFanCounter) {
		for (int i = 0; i < sensor.Fans.size(); i++) {
			bool valid;
			int value = ReadByte(FAN_TACHOMETER_REG[i], valid);
			if (!valid)
				continue;
			value |= ReadByte(FAN_TACHOMETER_EXT_REG[i], valid) << 8;
			if (!valid)
				continue;

			if (value > 0x3f) {
				sensor.Fans[i] = (value < 0xffff) ? 1.35e6f / (value * 2) : INFINITY;
			}
			else {
				sensor.Fans[i] = INFINITY;
			}
		}
	}
	else {
		for (int i = 0; i < sensor.Fans.size(); i++) {
			bool valid;
			int value = ReadByte(FAN_TACHOMETER_REG[i], valid);
			if (!valid)
				continue;

			int divisor = 2;
			if (i < 2) {
				int divisors = ReadByte(FAN_TACHOMETER_DIVISOR_REGISTER, valid);
				if (!valid)
					continue;
				divisor = 1 << ((divisors >> (3 * i)) & 0x7);
			}

			if (value > 0) {
				sensor.Fans[i] = (value < 0xff) ? 1.35e6f / (value * divisor) : INFINITY;
			}
			else {
				sensor.Fans[i] = INFINITY;
			}
		}
	}

	for (int i = 0; i < sensor.Controls.size(); i++) {
		bool valid;
		BYTE value = ReadByte(FAN_PWM_CTRL_REG[i], valid);
		if (!valid)
			continue;

		if ((value & 0x80) > 0) {
			// automatic operation (value can't be read)
			sensor.Controls[i] = INFINITY;
		}
		else {
			// software operation
			sensor.Controls[i] = (float)round((value & 0x7F) * 100.0f / 0x7F);
		}
	}
}
BYTE IT87XX::ReadByte(BYTE Register, bool & valid)
{
	SV_ASSIST::Ring0::WrIOPort(addressReg, 1, Register);
	DWORD value = 0, temp = 0;
	SV_ASSIST::Ring0::RdIOPort(dataReg, 1, value);
	SV_ASSIST::Ring0::RdIOPort(addressReg, 1, temp);
	valid = Register == (BYTE)temp;
	return (BYTE)value;
}
bool IT87XX::WriteByte(BYTE Register, BYTE value)
{
	SV_ASSIST::Ring0::WrIOPort(addressReg, 1, Register);
	SV_ASSIST::Ring0::WrIOPort(dataReg, 1, value);
	DWORD temp = 0;
	SV_ASSIST::Ring0::RdIOPort(addressReg, 1, temp);

	return Register == (BYTE)temp;
}
#pragma endregion