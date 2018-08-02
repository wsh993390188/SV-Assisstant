#pragma once
#include "SuperIoEntry.h"

#pragma region Public
class _SioSensor
{
public:
	_SioSensor();
	const SioSensor& GetSensordata();
	virtual void UpdateData();
protected:
	SioSensor sensor;
};
#pragma endregion

#pragma region Winbond
class W836XX : public _SioSensor
{
public:
	W836XX(std::pair<USHORT, std::string> chip, USHORT LPC_Address);
	void UpdateData();
protected:
private:
	BYTE ReadByte(BYTE bank, BYTE Register);
	void WriteByte(BYTE bank, BYTE Register, BYTE value);
	bool IsWinbondVendor();
	static ULONG SetBit(ULONG target, int bit, int value);
private:
	USHORT Chip_ID;
	USHORT WINBOND_VENDOR_ID;
	USHORT LPC_Base;    // Consts 
	const BYTE HIGH_BYTE;

	// Hardware Monitor
	const BYTE ADDRESS_REGISTER_OFFSET;
	const BYTE DATA_REGISTER_OFFSET;

	// Hardware Monitor Registers
	const BYTE VOLTAGE_VBAT_REG;
	const BYTE BANK_SELECT_REGISTER;
	const BYTE VENDOR_ID_REGISTER;
	const BYTE TEMPERATURE_SOURCE_SELECT_REG;

	const std::vector<BYTE> TEMPERATURE_REG;
	const std::vector<BYTE> TEMPERATURE_BANK;
	const std::vector<BYTE> FAN_TACHO_REG;
	const std::vector<BYTE> FAN_TACHO_BANK;
	const std::vector<BYTE> FAN_BIT_REG;
	const std::vector<BYTE> FAN_DIV_BIT0;
	const std::vector<BYTE> FAN_DIV_BIT1;
	const std::vector<BYTE> FAN_DIV_BIT2;

	std::bitset<3> peciTemperature;
	std::vector<BYTE> voltageRegister;
	std::vector<BYTE> voltageBank;
	float voltageGain;
};
#pragma endregion

#pragma region Fintek
class F718XX : public _SioSensor
{
public:
	F718XX(std::pair<USHORT, std::string> chip, USHORT LPC_Address);
	void UpdateData();
protected:
private:
	BYTE ReadByte(BYTE Register);
private:
	USHORT Chip_Id;
	const USHORT ADDRESS_REGISTER_OFFSET;
	const USHORT DATA_REGISTER_OFFSET;

	// Hardware Monitor Registers
	const BYTE VOLTAGE_BASE_REG;
	const BYTE TEMPERATURE_CONFIG_REG;
	const BYTE TEMPERATURE_BASE_REG;
	const std::vector<BYTE> FAN_TACHOMETER_REG;
	USHORT LPC_Base;
};
#pragma endregion

#pragma region Nuvoton
class NCT677X : public _SioSensor
{
public:
	NCT677X(std::pair<USHORT, std::string> chip,  USHORT LPC_Address);
	void UpdateData();
private:
	void NuvotonDisableIOSpaceLock();
	bool IsNuvotonVendor();
	BYTE Readbyte(USHORT address);
	void WriteByte(USHORT address, BYTE value);
private:
	bool IsNCT6791D;
	USHORT NUVOTON_VENDOR_ID;
	const USHORT ADDRESS_REGISTER_OFFSET;
	const USHORT DATA_REGISTER_OFFSET;
	const BYTE BANK_SELECT_REGISTER;
	USHORT LPC_Base;
	// Hardware Monitor Registers    
	USHORT VENDOR_ID_HIGH_REGISTER;
	USHORT VENDOR_ID_LOW_REGISTER;

	std::vector<USHORT> FAN_PWM_OUT_REG;
	std::vector<USHORT> FAN_PWM_COMMAND_REG;
	std::vector<USHORT> FAN_CONTROL_MODE_REG;

	USHORT fanRpmBaseRegister;
	int minFanRPM;

	bool restoreDefaultFanControlRequired[6];
	BYTE initialFanControlMode[6];
	BYTE initialFanPwmCommand[6];

	std::vector<USHORT> voltageRegisters;
	USHORT voltageVBatRegister;
	USHORT vBatMonitorControlRegister;

	std::vector<BYTE> temperaturesSource;

	std::vector<USHORT> temperatureRegister;
	std::vector<USHORT> temperatureHalfRegister;
	std::vector<int> temperatureHalfBit;
	std::vector<USHORT> temperatureSourceRegister;

	std::vector<USHORT> alternateTemperatureRegister;
};
#pragma endregion

#pragma region SMSC
class SMSCXX : public _SioSensor
{
public:
	SMSCXX(std::pair<USHORT, std::string> chip,USHORT LPC_Address);
	void UpdateData();
protected:
private:
	USHORT LPC_Base;
};
#pragma endregion

#pragma region ITE
class IT87XX : public _SioSensor
{
public:
	IT87XX(std::pair<USHORT, std::string> chip, USHORT LPC_Address);
	void UpdateData();
protected:
private:
	BYTE ReadByte(BYTE Register, _Out_ bool& valid);
	bool WriteByte(BYTE Register, BYTE value);
private:
	const BYTE ITE_VENDOR_ID;

	// Environment Controller Registers    
	const BYTE CONFIGURATION_REGISTER;
	const BYTE TEMPERATURE_BASE_REG;
	const BYTE VENDOR_ID_REGISTER;
	const BYTE FAN_TACHOMETER_DIVISOR_REGISTER;

	float voltageGain;
	bool has16bitFanCounter;
	USHORT LPC_Base;
	USHORT addressReg;
	USHORT dataReg;

	const std::vector<USHORT> FAN_TACHOMETER_REG;
	const std::vector<USHORT> FAN_TACHOMETER_EXT_REG;
	const USHORT VOLTAGE_BASE_REG;
	const std::vector<USHORT> FAN_PWM_CTRL_REG;
};
#pragma endregion

