#include "stdafx.h"
#include "SioChip.h"
#include "..\..\Driverdll\Driverdll.h"

#pragma region Nuvoton Chip Register 
enum class  SourceNCT6771F
{
	SYSTIN = 1,
	CPUTIN = 2,
	AUXTIN = 3,
	SMBUSMASTER = 4,
	PECI_0 = 5,
	PECI_1 = 6,
	PECI_2 = 7,
	PECI_3 = 8,
	PECI_4 = 9,
	PECI_5 = 10,
	PECI_6 = 11,
	PECI_7 = 12,
	PCH_CHIP_CPU_MAX_TEMP = 13,
	PCH_CHIP_TEMP = 14,
	PCH_CPU_TEMP = 15,
	PCH_MCH_TEMP = 16,
	PCH_DIM0_TEMP = 17,
	PCH_DIM1_TEMP = 18,
	PCH_DIM2_TEMP = 19,
	PCH_DIM3_TEMP = 20
};
enum class  SourceNCT6776F {
	SYSTIN = 1,
	CPUTIN = 2,
	AUXTIN = 3,
	SMBUSMASTER_0 = 4,
	SMBUSMASTER_1 = 5,
	SMBUSMASTER_2 = 6,
	SMBUSMASTER_3 = 7,
	SMBUSMASTER_4 = 8,
	SMBUSMASTER_5 = 9,
	SMBUSMASTER_6 = 10,
	SMBUSMASTER_7 = 11,
	PECI_0 = 12,
	PECI_1 = 13,
	PCH_CHIP_CPU_MAX_TEMP = 14,
	PCH_CHIP_TEMP = 15,
	PCH_CPU_TEMP = 16,
	PCH_MCH_TEMP = 17,
	PCH_DIM0_TEMP = 18,
	PCH_DIM1_TEMP = 19,
	PCH_DIM2_TEMP = 20,
	PCH_DIM3_TEMP = 21,
	BYTE_TEMP = 22
};
enum class  SourceNCT67XXD {
	SYSTIN = 1,
	CPUTIN = 2,
	AUXTIN0 = 3,
	AUXTIN1 = 4,
	AUXTIN2 = 5,
	AUXTIN3 = 6,
	SMBUSMASTER_0 = 8,
	SMBUSMASTER_1 = 9,
	SMBUSMASTER_2 = 10,
	SMBUSMASTER_3 = 11,
	SMBUSMASTER_4 = 12,
	SMBUSMASTER_5 = 13,
	SMBUSMASTER_6 = 14,
	SMBUSMASTER_7 = 15,
	PECI_0 = 16,
	PECI_1 = 17,
	PCH_CHIP_CPU_MAX_TEMP = 18,
	PCH_CHIP_TEMP = 19,
	PCH_CPU_TEMP = 20,
	PCH_MCH_TEMP = 21,
	PCH_DIM0_TEMP = 22,
	PCH_DIM1_TEMP = 23,
	PCH_DIM2_TEMP = 24,
	PCH_DIM3_TEMP = 25,
	BYTE_TEMP = 26
};
enum class SourceNCT610X {
	SYSTIN = 1,
	CPUTIN = 2,
	AUXTIN = 3,
	SMBUSMASTER_0 = 4,
	SMBUSMASTER_1 = 5,
	SMBUSMASTER_2 = 6,
	SMBUSMASTER_3 = 7,
	SMBUSMASTER_4 = 8,
	SMBUSMASTER_5 = 9,
	SMBUSMASTER_6 = 10,
	SMBUSMASTER_7 = 11,
	PECI_0 = 12,
	PECI_1 = 13,
	PCH_CHIP_CPU_MAX_TEMP = 14,
	PCH_CHIP_TEMP = 15,
	PCH_CPU_TEMP = 16,
	PCH_MCH_TEMP = 17,
	PCH_DIM0_TEMP = 18,
	PCH_DIM1_TEMP = 19,
	PCH_DIM2_TEMP = 20,
	PCH_DIM3_TEMP = 21,
	BYTE_TEMP = 22
};
#pragma endregion

#pragma region Nuvoton
NCT677X::NCT677X(std::pair<USHORT, std::string> chip, USHORT LPC_Address) : NUVOTON_VENDOR_ID{ 0x5CA3 },
ADDRESS_REGISTER_OFFSET{ 0x05 }, DATA_REGISTER_OFFSET{ 0x06 }, BANK_SELECT_REGISTER{ 0x4E }
{
	this->sensor.ChipName = chip.second;
	IsNCT6791D = chip.first == NCT6791D;
	this->LPC_Base = LPC_Address;
#pragma region Nuvoton Initiate
	if (chip.first == NCT610X)
	{
		VENDOR_ID_HIGH_REGISTER = 0x80FE;
		VENDOR_ID_LOW_REGISTER = 0x00FE;
		FAN_PWM_OUT_REG.emplace_back(0x04A);
		FAN_PWM_OUT_REG.emplace_back(0x04B);
		FAN_PWM_OUT_REG.emplace_back(0x04C);
		FAN_PWM_COMMAND_REG.emplace_back(0x119);
		FAN_PWM_COMMAND_REG.emplace_back(0x129);
		FAN_PWM_COMMAND_REG.emplace_back(0x139);
		FAN_CONTROL_MODE_REG.emplace_back(0x113);
		FAN_CONTROL_MODE_REG.emplace_back(0x123);
		FAN_CONTROL_MODE_REG.emplace_back(0x133);
		vBatMonitorControlRegister = 0x0318;
	}
	else
	{
		VENDOR_ID_HIGH_REGISTER = 0x804F;
		VENDOR_ID_LOW_REGISTER = 0x004F;
		FAN_PWM_OUT_REG.emplace_back(0x001);
		FAN_PWM_OUT_REG.emplace_back(0x003);
		FAN_PWM_OUT_REG.emplace_back(0x011);
		FAN_PWM_OUT_REG.emplace_back(0x013);
		FAN_PWM_OUT_REG.emplace_back(0x015);
		FAN_PWM_OUT_REG.emplace_back(0x017);
		FAN_PWM_COMMAND_REG.emplace_back(0x109);
		FAN_PWM_COMMAND_REG.emplace_back(0x209);
		FAN_PWM_COMMAND_REG.emplace_back(0x309);
		FAN_PWM_COMMAND_REG.emplace_back(0x809);
		FAN_PWM_COMMAND_REG.emplace_back(0x909);
		FAN_PWM_COMMAND_REG.emplace_back(0xA09);
		FAN_CONTROL_MODE_REG.emplace_back(0x102);
		FAN_CONTROL_MODE_REG.emplace_back(0x202);
		FAN_CONTROL_MODE_REG.emplace_back(0x302);
		FAN_CONTROL_MODE_REG.emplace_back(0x802);
		FAN_CONTROL_MODE_REG.emplace_back(0x902);
		FAN_CONTROL_MODE_REG.emplace_back(0xA02);
		vBatMonitorControlRegister = 0x005D;
	}

	if (!IsNuvotonVendor())
		return;

	switch (chip.first)
	{
	case NCT6771F:
	case NCT6776F:
		if (chip.first == NCT6771F) {
			sensor.Fans = { INFINITY,INFINITY,INFINITY,INFINITY };

			// min value RPM value with 16-bit fan counter
			minFanRPM = (int)(1.35e6 / 0xFFFF);

			temperaturesSource.emplace_back((BYTE)SourceNCT6771F::PECI_0);
			temperaturesSource.emplace_back((BYTE)SourceNCT6771F::CPUTIN);
			temperaturesSource.emplace_back((BYTE)SourceNCT6771F::AUXTIN);
			temperaturesSource.emplace_back((BYTE)SourceNCT6771F::SYSTIN);
		}
		else {
			sensor.Fans = { INFINITY,INFINITY,INFINITY,INFINITY,INFINITY };

			// min value RPM value with 13-bit fan counter
			minFanRPM = (int)(1.35e6 / 0x1FFF);

			temperaturesSource.emplace_back((BYTE)SourceNCT6776F::PECI_0);
			temperaturesSource.emplace_back((BYTE)SourceNCT6776F::CPUTIN);
			temperaturesSource.emplace_back((BYTE)SourceNCT6776F::AUXTIN);
			temperaturesSource.emplace_back((BYTE)SourceNCT6776F::SYSTIN);
		}

		fanRpmBaseRegister = 0x656;

		sensor.Controls = { INFINITY,INFINITY,INFINITY };

		sensor.Voltages = { INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY };

		voltageRegisters = { 0x020 ,0x021,0x022,0x023,0x024,0x025,0x026,0x550 ,0x551 };
		voltageVBatRegister = 0x551;

		sensor.Temperatures = { INFINITY,INFINITY,INFINITY,INFINITY };

		temperatureRegister.emplace_back(0x027);
		temperatureRegister.emplace_back(0x073);
		temperatureRegister.emplace_back(0x075);
		temperatureRegister.emplace_back(0x077);
		temperatureRegister.emplace_back(0x150);
		temperatureRegister.emplace_back(0x250);
		temperatureRegister.emplace_back(0x62B);
		temperatureRegister.emplace_back(0x62C);
		temperatureRegister.emplace_back(0x62D);

		temperatureHalfRegister.emplace_back(0);
		temperatureHalfRegister.emplace_back(0x074);
		temperatureHalfRegister.emplace_back(0x076);
		temperatureHalfRegister.emplace_back(0x078);
		temperatureHalfRegister.emplace_back(0x151);
		temperatureHalfRegister.emplace_back(0x251);
		temperatureHalfRegister.emplace_back(0x62E);
		temperatureHalfRegister.emplace_back(0x62E);
		temperatureHalfRegister.emplace_back(0x62E);

		temperatureHalfBit.emplace_back(-1);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(0);
		temperatureHalfBit.emplace_back(1);
		temperatureHalfBit.emplace_back(2);
		temperatureSourceRegister.emplace_back(0x621);
		temperatureSourceRegister.emplace_back(0x100);
		temperatureSourceRegister.emplace_back(0x200);
		temperatureSourceRegister.emplace_back(0x300);
		temperatureSourceRegister.emplace_back(0x622);
		temperatureSourceRegister.emplace_back(0x623);
		temperatureSourceRegister.emplace_back(0x624);
		temperatureSourceRegister.emplace_back(0x625);
		temperatureSourceRegister.emplace_back(0x626);
		break;
	case NCT6779D:
	case NCT6791D:
		if (chip.first == NCT6779D) {
			sensor.Fans = { INFINITY,INFINITY,INFINITY,INFINITY,INFINITY };
			sensor.Controls = { INFINITY,INFINITY,INFINITY,INFINITY,INFINITY };
		}
		else {
			sensor.Fans = { INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY };
			sensor.Controls = { INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY };
		}

		fanRpmBaseRegister = 0x4C0;

		// min value RPM value with 13-bit fan counter
		minFanRPM = (int)(1.35e6 / 0x1FFF);

		sensor.Voltages = { INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY };
		voltageRegisters.emplace_back(0x480);
		voltageRegisters.emplace_back(0x481);
		voltageRegisters.emplace_back(0x482);
		voltageRegisters.emplace_back(0x483);
		voltageRegisters.emplace_back(0x484);
		voltageRegisters.emplace_back(0x485);
		voltageRegisters.emplace_back(0x486);
		voltageRegisters.emplace_back(0x487);
		voltageRegisters.emplace_back(0x488);
		voltageRegisters.emplace_back(0x489);
		voltageRegisters.emplace_back(0x48A);
		voltageRegisters.emplace_back(0x48B);
		voltageRegisters.emplace_back(0x48C);
		voltageRegisters.emplace_back(0x48D);
		voltageRegisters.emplace_back(0x48E);
		voltageVBatRegister = 0x488;

		sensor.Temperatures = { INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY };

		temperaturesSource.emplace_back((BYTE)SourceNCT67XXD::PECI_0);
		temperaturesSource.emplace_back((BYTE)SourceNCT67XXD::CPUTIN);
		temperaturesSource.emplace_back((BYTE)SourceNCT67XXD::SYSTIN);
		temperaturesSource.emplace_back((BYTE)SourceNCT67XXD::AUXTIN0);
		temperaturesSource.emplace_back((BYTE)SourceNCT67XXD::AUXTIN1);
		temperaturesSource.emplace_back((BYTE)SourceNCT67XXD::AUXTIN2);
		temperaturesSource.emplace_back((BYTE)SourceNCT67XXD::AUXTIN3);

		temperatureRegister.emplace_back(0x027);
		temperatureRegister.emplace_back(0x073);
		temperatureRegister.emplace_back(0x075);
		temperatureRegister.emplace_back(0x077);
		temperatureRegister.emplace_back(0x079);
		temperatureRegister.emplace_back(0x07B);
		temperatureRegister.emplace_back(0x150);
		temperatureHalfRegister.emplace_back(0);
		temperatureHalfRegister.emplace_back(0x074);
		temperatureHalfRegister.emplace_back(0x076);
		temperatureHalfRegister.emplace_back(0x078);
		temperatureHalfRegister.emplace_back(0x07A);
		temperatureHalfRegister.emplace_back(0x07C);
		temperatureHalfRegister.emplace_back(0x151);
		temperatureHalfBit.emplace_back(-1);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);
		temperatureSourceRegister.emplace_back(0x621);
		temperatureSourceRegister.emplace_back(0x100);
		temperatureSourceRegister.emplace_back(0x200);
		temperatureSourceRegister.emplace_back(0x300);
		temperatureSourceRegister.emplace_back(0x800);
		temperatureSourceRegister.emplace_back(0x900);
		temperatureSourceRegister.emplace_back(0x622);

		alternateTemperatureRegister.emplace_back(0);
		alternateTemperatureRegister.emplace_back(0x491);
		alternateTemperatureRegister.emplace_back(0x490);
		alternateTemperatureRegister.emplace_back(0x492);
		alternateTemperatureRegister.emplace_back(0x493);
		alternateTemperatureRegister.emplace_back(0x494);
		alternateTemperatureRegister.emplace_back(0x495);
		break;
	case NCT610X:
		sensor.Fans = { INFINITY,INFINITY,INFINITY };

		sensor.Controls = { INFINITY,INFINITY,INFINITY };

		fanRpmBaseRegister = 0x030;

		// min value RPM value with 13-bit fan counter
		minFanRPM = (int)(1.35e6 / 0x1FFF);

		sensor.Voltages = { INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY,INFINITY };

		voltageRegisters.emplace_back(0x300);
		voltageRegisters.emplace_back(0x301);
		voltageRegisters.emplace_back(0x302);
		voltageRegisters.emplace_back(0x303);
		voltageRegisters.emplace_back(0x304);
		voltageRegisters.emplace_back(0x305);
		voltageRegisters.emplace_back(0x307);
		voltageRegisters.emplace_back(0x308);
		voltageRegisters.emplace_back(0x309);

		voltageVBatRegister = 0x308;

		sensor.Temperatures = { INFINITY,INFINITY,INFINITY,INFINITY };

		temperaturesSource.emplace_back((BYTE)SourceNCT610X::PECI_0);
		temperaturesSource.emplace_back((BYTE)SourceNCT610X::SYSTIN);
		temperaturesSource.emplace_back((BYTE)SourceNCT610X::CPUTIN);
		temperaturesSource.emplace_back((BYTE)SourceNCT610X::AUXTIN);

		temperatureRegister.emplace_back(0x027);
		temperatureRegister.emplace_back(0x018);
		temperatureRegister.emplace_back(0x019);
		temperatureRegister.emplace_back(0x01A);

		temperatureHalfRegister.emplace_back(0);
		temperatureHalfRegister.emplace_back(0x01B);
		temperatureHalfRegister.emplace_back(0x11B);
		temperatureHalfRegister.emplace_back(0x21B);

		temperatureHalfBit.emplace_back(-1);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);
		temperatureHalfBit.emplace_back(7);

		temperatureSourceRegister.emplace_back(0x621);
		temperatureSourceRegister.emplace_back(0x100);
		temperatureSourceRegister.emplace_back(0x200);
		temperatureSourceRegister.emplace_back(0x300);

		alternateTemperatureRegister.emplace_back(0);
		alternateTemperatureRegister.emplace_back(0x018);
		alternateTemperatureRegister.emplace_back(0x019);
		alternateTemperatureRegister.emplace_back(0x01A);

		break;
	default:
		break;
	}
#pragma endregion

#ifdef ZX_OutputLog
	std::string ttt = boost::str(boost::format("LPC Address: 0x%lx") % LPC_Address);
	Logger::Instance()->OutputLogInfo(el::Level::Debug, ttt);
	ttt = boost::str(boost::format("LPC VenderId: 0x%lx") % NUVOTON_VENDOR_ID);
	Logger::Instance()->OutputLogInfo(el::Level::Debug, ttt);
#endif // ZX_OutputLog
}

void NCT677X::NuvotonDisableIOSpaceLock()
{
	//懒得改 强制用2E/2F
	SV_ASSIST::Ring0::WrIOPort(0x2E, 1, 0x87);
	SV_ASSIST::Ring0::WrIOPort(0x2E, 1, 0x87);
	DWORD options = 0;
	SV_ASSIST::Ring0::WrIOPort(0x2E, 1, 0x28);
	SV_ASSIST::Ring0::RdIOPort(0x2F, 1, options);

	if ((options & 0x10) > 0) {
		// disable the i/o space lock
		SV_ASSIST::Ring0::WrIOPort(0x2E, 1, 0x28);
		SV_ASSIST::Ring0::WrIOPort(0x2F, 1, (BYTE)(options & ~0x10));
	}
	SV_ASSIST::Ring0::WrIOPort(0x2E, 1, 0xAA);
}

void NCT677X::UpdateData()
{
	if (!IsNuvotonVendor() && !IsNCT6791D)
		return;

	NuvotonDisableIOSpaceLock();

	for (int i = 0; i < sensor.Voltages.size(); i++)
	{
		float value = 0.008f * Readbyte(voltageRegisters[i]);
		bool valid = value > 0;

		// check if battery voltage monitor is enabled
		if (valid && voltageRegisters[i] == voltageVBatRegister)
			valid = (Readbyte(vBatMonitorControlRegister) & 0x01) > 0;

		sensor.Voltages[i] = valid ? value : INFINITY;
	}

	int temperatureSourceMask = 0;
	for (LONGLONG i = temperatureRegister.size() - 1; i >= 0; i--) {
		if (!temperatureRegister[i])
			continue;
		int value = ((char)Readbyte(temperatureRegister[i])) << 1;
		if (temperatureHalfBit[i] > 0) {
			value |= ((Readbyte(temperatureHalfRegister[i]) >>
				temperatureHalfBit[i]) & 0x1);
		}

		BYTE source = Readbyte(temperatureSourceRegister[i]);
		temperatureSourceMask |= 1 << source;

		float temperature = 0.5f * value;
		if (temperature > 125 || temperature < -55)
			temperature = INFINITY;

		for (int j = 0; j < sensor.Temperatures.size(); j++)
			if (temperaturesSource[j] == source)
				sensor.Temperatures[j] = temperature;
	}
	for (int i = 0; i < alternateTemperatureRegister.size(); i++) {
		if (!alternateTemperatureRegister[i])
			continue;

		if ((temperatureSourceMask & (1 << temperaturesSource[i])) > 0)
			continue;

		float temperature = (char)
			Readbyte(alternateTemperatureRegister[i]);

		if (temperature > 125 || temperature < -55)
			temperature = INFINITY;

		sensor.Temperatures[i] = temperature;
	}

	for (int i = 0; i < sensor.Fans.size(); i++) {
		BYTE high = Readbyte((USHORT)(fanRpmBaseRegister + (i << 1)));
		BYTE low = Readbyte((USHORT)(fanRpmBaseRegister + (i << 1) + 1));
		int value = (high << 8) | low;

		sensor.Fans[i] = value > minFanRPM ? value : INFINITY;
	}

	for (int i = 0; i < sensor.Controls.size(); i++) {
		int value = Readbyte(FAN_PWM_OUT_REG[i]);
		sensor.Controls[i] = value / 2.55f;
	}
}

bool NCT677X::IsNuvotonVendor()
{
	return ((Readbyte(VENDOR_ID_HIGH_REGISTER) << 8) |
		Readbyte(VENDOR_ID_LOW_REGISTER)) == NUVOTON_VENDOR_ID;
}

BYTE NCT677X::Readbyte(USHORT address)
{
	BYTE bank = (BYTE)(address >> 8);
	BYTE Register = (BYTE)(address & 0xFF);
	SV_ASSIST::Ring0::WrIOPort(LPC_Base + ADDRESS_REGISTER_OFFSET, 1, BANK_SELECT_REGISTER);
	SV_ASSIST::Ring0::WrIOPort(LPC_Base + DATA_REGISTER_OFFSET, 1, bank);
	SV_ASSIST::Ring0::WrIOPort(LPC_Base + ADDRESS_REGISTER_OFFSET, 1, Register);
	DWORD re = 0;
	SV_ASSIST::Ring0::RdIOPort(LPC_Base + DATA_REGISTER_OFFSET, 1, re);
	return (BYTE)re;
}

void NCT677X::WriteByte(USHORT address, BYTE value)
{
	BYTE bank = (BYTE)(address >> 8);
	BYTE Register = (BYTE)(address & 0xFF);
	SV_ASSIST::Ring0::WrIOPort(LPC_Base + ADDRESS_REGISTER_OFFSET, 1, BANK_SELECT_REGISTER);
	SV_ASSIST::Ring0::WrIOPort(LPC_Base + DATA_REGISTER_OFFSET, 1, bank);
	SV_ASSIST::Ring0::WrIOPort(LPC_Base + ADDRESS_REGISTER_OFFSET, 1, Register);
	SV_ASSIST::Ring0::WrIOPort(LPC_Base + DATA_REGISTER_OFFSET, 1, value);
}
#pragma endregion
