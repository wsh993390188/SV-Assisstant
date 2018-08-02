#pragma once
#include "SuperIOStruct.h"

class SuperIoEntry
{
public:
	SuperIoEntry();
	~SuperIoEntry();
	bool ReadWinbondFintekPage(USHORT logicalDeviceNumber, Super_IO_Data& SIO_Data);
	bool ReadSMSCPage(USHORT logicalDeviceNumber, Super_IO_Data& SIO_Data);
	bool ReadITEPage(USHORT logicalDeviceNumber, Super_IO_Data& SIO_Data);
	bool DetectSIO();
	USHORT LPCBASE();
	bool IsIte();
	bool IsSMSC();
	bool IsWinbond();
	bool IsFintek();
	bool IsNuvoton();
	std::pair<USHORT, std::string> ChipID;
protected:
	static std::pair<USHORT, USHORT>  using_SIO_port;
private:
	explicit SuperIoEntry(const SuperIoEntry& that);
	SuperIoEntry operator=(const SuperIoEntry& that);
	bool Ite;
	bool SMSC;
	bool Winbond;
	bool Fintek;
	bool Nuvoton;
	DWORD LPC_Address;
#pragma region SIO Register
	const std::vector<std::pair<USHORT, USHORT>> SIO_Port;
	const UCHAR CHIP_ID_REGISTER;
	const UCHAR CHIP_REVISION_REGISTER;
	const UCHAR BASE_ADDRESS_REGISTER;
	const UCHAR DEVCIE_SELECT_REGISTER;
#pragma endregion

#pragma region Common Operation
	void SelectPage(USHORT port, USHORT data, UCHAR logicalDeviceNumber);
	void SIOReadWord(USHORT port, USHORT data, DWORD offset, DWORD& output);
	void SIOReadByte(USHORT port, USHORT data, DWORD offset, DWORD& output);
#pragma endregion

#pragma region ITE
	bool DetectITE();
	bool ReadITE(USHORT port, USHORT data);
	bool ReadITEPage(USHORT port, USHORT data, USHORT logicalDeviceNumber, Super_IO_Data& SIO_Data);
	void EntryITE(USHORT port);
	void ExitITE(USHORT port, USHORT data);
#pragma region ITE Register
	const UCHAR IT87_ENVIRONMENT_CONTROLLER;
	const UCHAR IT87_CHIP_VERSION_REGISTER;
#pragma endregion
#pragma endregion

#pragma region Winbond, Nuvoton, Fintek
	bool DetectWinbondFintek();
	bool ReadWinbondFintek(USHORT port, USHORT data);
	bool ReadWinbondFintekPage(USHORT port, USHORT data, USHORT logicalDeviceNumber, Super_IO_Data& SIO_Data);
	void NuvotonDisableIOSpaceLock(USHORT port, USHORT data);
	void EntryWinbondFintek(USHORT port);
	void ExitWinbondFintek(USHORT port);
	const BYTE FINTEK_VENDOR_ID_REGISTER;
	const USHORT FINTEK_VENDOR_ID;
	const USHORT NUVOTON_HARDWARE_MONITOR_IO_SPACE_LOCK;
	const BYTE WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;

	const BYTE F71858_HARDWARE_MONITOR_LDN;
	const BYTE FINTEK_HARDWARE_MONITOR_LDN;
#pragma endregion

#pragma region SMSC
	bool DetectSMSC();
	bool ReadSMSC(USHORT port, USHORT data);
	bool ReadSMSCPage(USHORT port, USHORT data, USHORT logicalDeviceNumber, Super_IO_Data& SIO_Data);
	void EntrySMSC(USHORT port);
	void ExitSMSC(USHORT port);
#pragma endregion
};
