#include "stdafx.h"
#include "..\driver\Driverdll.h"
#include "SuperIoEntry.h"
#include <boost/format.hpp>

#pragma region Public SuperIo
SuperIoEntry::SuperIoEntry() : LPC_Address{ 0xFFFFFFFF }, SIO_Port{ std::make_pair(0x2Eu, 0x2fu), std::make_pair(0x4Eu, 0x4fu) },
					Ite{}, SMSC{}, Nuvoton{}, Winbond{}, Fintek{},
					CHIP_ID_REGISTER{ 0x20u }, CHIP_REVISION_REGISTER{ 0x21u }, BASE_ADDRESS_REGISTER{ 0x60u },
					DEVCIE_SELECT_REGISTER{ 0x07u }, IT87_ENVIRONMENT_CONTROLLER{ 0x04u }, IT87_CHIP_VERSION_REGISTER{ 0x22u },
					ChipID{ std::make_pair(0, std::string("Not exist")) }, FINTEK_VENDOR_ID_REGISTER{ 0x23u }, FINTEK_VENDOR_ID{ 0x1934u }, WINBOND_NUVOTON_HARDWARE_MONITOR_LDN{ 0x0Bu },
					F71858_HARDWARE_MONITOR_LDN{ 0x02u }, FINTEK_HARDWARE_MONITOR_LDN{ 0x04u }, NUVOTON_HARDWARE_MONITOR_IO_SPACE_LOCK{ 0x28u }
{
	DetectSIO();
}

std::pair<USHORT, USHORT> SuperIoEntry::using_SIO_port = std::make_pair(0x2E, 0x2F);

SuperIoEntry::~SuperIoEntry()
{
}

bool SuperIoEntry::ReadWinbondFintekPage(USHORT logicalDeviceNumber, Super_IO_Data & SIO_Data)
{
	bool state = false;
	for (auto SIO : SIO_Port)
	{
		if (this->ReadWinbondFintekPage(SIO.first, SIO.second, logicalDeviceNumber, SIO_Data))
		{
			using_SIO_port = SIO;
			state = true;
			break;
		}
	}
	return state;
}

bool SuperIoEntry::ReadSMSCPage(USHORT logicalDeviceNumber, Super_IO_Data & SIO_Data)
{
	bool state = false;
	for (auto SIO : SIO_Port)
	{
		if (this->ReadSMSCPage(SIO.first, SIO.second, logicalDeviceNumber, SIO_Data))
		{
			using_SIO_port = SIO;
			state = true;
			break;
		}
	}
	return state;
}

bool SuperIoEntry::ReadITEPage(USHORT logicalDeviceNumber, Super_IO_Data & SIO_Data)
{
	bool state = false;
	for (auto SIO : SIO_Port)
	{
		if (this->ReadITEPage(SIO.first, SIO.second, logicalDeviceNumber, SIO_Data))
		{
			using_SIO_port = SIO;
			state = true;
			break;
		}
	}
	return state;
}

bool SuperIoEntry::DetectSIO()
{
	bool state = false;

	if (DetectWinbondFintek())
	{
		if (ChipID.second.find("Fintek") != std::string::npos)
			Fintek = true;
		if (ChipID.second.find("Winbond") != std::string::npos)
			Winbond = true;
		if (ChipID.second.find("Nuvoton") != std::string::npos)
			Nuvoton = true;
		state = true;
	}
	if (DetectITE())
	{
		Ite = true;
		state = true;
	}
	if (DetectSMSC())
	{
		SMSC = true;
		state = true;
	}
	return state;
}

USHORT SuperIoEntry::LPCBASE()
{
	return (USHORT)LPC_Address;
}

bool SuperIoEntry::IsIte()
{
	return Ite;
}

bool SuperIoEntry::IsSMSC()
{
	return SMSC;
}

bool SuperIoEntry::IsWinbond()
{
	return Winbond;
}

bool SuperIoEntry::IsFintek()
{
	return Fintek;
}

bool SuperIoEntry::IsNuvoton()
{
	return Nuvoton;
}
#pragma endregion

#pragma region Private Operation
void SuperIoEntry::SelectPage(USHORT port, USHORT data, UCHAR logicalDeviceNumber)
{
	SV_ASSIST::Ring0::WrIOPort(port, 1, DEVCIE_SELECT_REGISTER);
	SV_ASSIST::Ring0::WrIOPort(data, 1, logicalDeviceNumber);
}

void SuperIoEntry::SIOReadWord(USHORT port, USHORT data, DWORD offset, DWORD & output)
{
	DWORD temp1 = 0, temp2 = 0;
	SV_ASSIST::Ring0::WrIOPort(port, 1, offset);
	SV_ASSIST::Ring0::RdIOPort(data, 1, temp1);

	SV_ASSIST::Ring0::WrIOPort(port, 1, offset + 1);
	SV_ASSIST::Ring0::RdIOPort(data, 1, temp2);
	output = temp1 << 8 | temp2;
}

void SuperIoEntry::SIOReadByte(USHORT port, USHORT data, DWORD offset, DWORD & output)
{
	SV_ASSIST::Ring0::WrIOPort(port, 1, offset);
	SV_ASSIST::Ring0::RdIOPort(data, 1, output);
}

#pragma endregion

#pragma region ITE
bool SuperIoEntry::DetectITE()
{
	bool state = false;
	for (auto SIO : SIO_Port)
	{
		if (this->ReadITE(SIO.first, SIO.second))
		{
			state = true;
			break;
		}
	}
	return state;
}

bool SuperIoEntry::ReadITE(USHORT port, USHORT data)
{
	bool state = true;
	if (port != 0x2E)
		return false;
	EntryITE(port);
	DWORD ChipID = 0;
	SIOReadWord(port, data,CHIP_ID_REGISTER, ChipID);
	if (ChipID == 0 || ChipID == 0xFFFF)
	{
		OutputDebugPrintf(_T("Probably Ite Doesnt exist\n"));
		state = false;
	}
	else
	{
		switch (ChipID)
		{
		case IT8603E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8603E");
			break;
		case IT8606E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8606E");
			break;
		case IT8607E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8607E");
			break;
		case IT8613E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8613E");
			break;
		case IT8620E: 			
			this->ChipID = std::make_pair(ChipID, "ITE IT8620E");
			break; 
		case IT8622E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8622E");
			break;
		case IT8623E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8623E");
			break;
		case IT8625E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8625E");
			break;
		case IT8628E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8628E");
			break;
		case IT8655E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8655E");
			break;
		case IT8665E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8665E");
			break;
		case IT8686E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8686E");
			break;
		case IT8705F: 
			this->ChipID = std::make_pair(ChipID, "ITE IT8705F");
			break;
		case IT8712F:
			this->ChipID = std::make_pair(ChipID, "ITE IT8712F");
			break;
		case IT8716F:
			this->ChipID = std::make_pair(ChipID, "ITE IT8716F");
			break;
		case IT8718F:
			this->ChipID = std::make_pair(ChipID, "ITE IT8718F");
			break;
		case IT8720F:
			this->ChipID = std::make_pair(ChipID, "ITE IT8720F");
			break;
		case IT8721F:
			this->ChipID = std::make_pair(ChipID, "ITE IT8721F");
			break;
		case IT8726F:
			this->ChipID = std::make_pair(ChipID, "ITE IT8726F");
			break;
		case IT8728F:
			this->ChipID = std::make_pair(ChipID, "ITE IT8728F");
			break;
		case IT8732F:
			this->ChipID = std::make_pair(ChipID, "ITE IT8732F");
			break;
		case IT8733E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8733E");
			break;
		case IT8771E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8771E");
			break;
		case IT8772E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8772E");
			break;
		case IT8783E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8783E");
			break;
		case IT8790E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8790E");
			break;
		case IT8792E:
			this->ChipID = std::make_pair(ChipID, "ITE IT8792E");
			break;
		default:
			boost::format fmt = boost::format("ITE Unknown Id %04X") % ChipID;
			this->ChipID = std::make_pair(ChipID, fmt.str());
			state = false;
			break;
		}
		this->SelectPage(port, data, IT87_ENVIRONMENT_CONTROLLER);
		SIOReadWord(port, data, BASE_ADDRESS_REGISTER, LPC_Address);
		Sleep(1);
		DWORD verify = 0;
		SIOReadWord(port, data, BASE_ADDRESS_REGISTER, verify);

		if (verify != LPC_Address)
			LPC_Address = 0;
		ExitITE(port, data);
	}
	return state;
}

bool SuperIoEntry::ReadITEPage(USHORT port, USHORT data, USHORT logicalDeviceNumber, Super_IO_Data& SIO_Data)
{
	bool state = true;
	if (port != 0x2E)
		return false;
	this->EntryITE(port);
	if (Ite)
	{
		this->SelectPage(port, data, (UCHAR)logicalDeviceNumber);
		for (USHORT j = 0; j < 0x100; ++j)
		{
			DWORD SuperIO_Data = 0;
			SIOReadByte(port, data, j, SuperIO_Data);
			SIO_Data.SuperioData[j] = (UCHAR)SuperIO_Data;
		}
		ExitITE(port, data);
	}
	return state;
}

void SuperIoEntry::EntryITE(USHORT port)
{
	SV_ASSIST::Ring0::WrIOPort(port, 1, 0x87);
	SV_ASSIST::Ring0::WrIOPort(port, 1, 0x01);
	SV_ASSIST::Ring0::WrIOPort(port, 1, 0x55);
	SV_ASSIST::Ring0::WrIOPort(port, 1, 0x55);
}


void SuperIoEntry::ExitITE(USHORT port, USHORT data)
{
	SV_ASSIST::Ring0::WrIOPort(port, 1, 0x02);
	SV_ASSIST::Ring0::WrIOPort(data, 1, 0x02);
}
#pragma endregion

#pragma region Winbond, Nuvoton, Fintek
bool SuperIoEntry::DetectWinbondFintek()
{
	bool state = false;
	for (auto SIO : SIO_Port)
	{
		if (this->ReadWinbondFintek(SIO.first, SIO.second))
		{
			state = true;
			break;
		}
	}
	return state;
}

bool SuperIoEntry::ReadWinbondFintek(USHORT port, USHORT data)
{
	bool state = true;
	USHORT logicalDeviceNumber = 0;
	EntryWinbondFintek(port);
	DWORD ChipID = 0;
	SIOReadByte(port, data, CHIP_ID_REGISTER, ChipID);
	DWORD Revision = 0;
	SIOReadByte(port, data, CHIP_REVISION_REGISTER, Revision);
	if (ChipID == 0 || ChipID == 0xFF)
	{
		OutputDebugPrintf(_T("Probably Winbond Or Fintek doesnt exist\n"));
		state = false;
	}
	else
	{
		switch (ChipID) {
		case 0x05:
			switch (Revision) {
			case 0x07:
				this->ChipID = std::make_pair(F71858, "Fintek F71858");
				logicalDeviceNumber = F71858_HARDWARE_MONITOR_LDN;
				break;
			case 0x41:
				this->ChipID = std::make_pair(F71882, "Fintek F71882");
				logicalDeviceNumber = FINTEK_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0x06:
			switch (Revision) {
			case 0x01:
				this->ChipID = std::make_pair(F71862, "Fintek F71862");
				logicalDeviceNumber = FINTEK_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0x07:
			switch (Revision) {
			case 0x23:
				this->ChipID = std::make_pair(F71889F, "Fintek F71889F");
				logicalDeviceNumber = FINTEK_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0x08:
			switch (Revision) {
			case 0x14:
				this->ChipID = std::make_pair(F71869, "Fintek F71869");
				logicalDeviceNumber = FINTEK_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0x09:
			switch (Revision) {
			case 0x01:
				this->ChipID = std::make_pair(F71808E, "Fintek F71808E");
				logicalDeviceNumber = FINTEK_HARDWARE_MONITOR_LDN;
				break;
			case 0x09:
				this->ChipID = std::make_pair(F71889ED, "Fintek F71889ED");
				logicalDeviceNumber = FINTEK_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0x10:
			switch (Revision) {
			case 0x05:
				this->ChipID = std::make_pair(F71889AD, "Fintek F71889AD");
				logicalDeviceNumber = FINTEK_HARDWARE_MONITOR_LDN;
				break;
			case 0x07:
				this->ChipID = std::make_pair(F71869A, "Fintek F71869A");
				logicalDeviceNumber = FINTEK_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0x52:
			switch (Revision) {
			case 0x17:
			case 0x3A:
			case 0x41:
				this->ChipID = std::make_pair(W83627HF, "Winbond W83627HF");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0x82:
			switch (Revision & 0xF0) {
			case 0x80:
				this->ChipID = std::make_pair(W83627THF, "Winbond W83627THF");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0x85:
			switch (Revision) {
			case 0x41:
				this->ChipID = std::make_pair(W83687THF, "Winbond W83687THF");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0x88:
			switch (Revision & 0xF0) {
			case 0x50:
			case 0x60:
				this->ChipID = std::make_pair(W83627EHF, "Winbond W83627EHF");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0xA0:
			switch (Revision & 0xF0) {
			case 0x20:
				this->ChipID = std::make_pair(W83627DHG, "Winbond W83627DHG");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0xA5:
			switch (Revision & 0xF0) {
			case 0x10:
				this->ChipID = std::make_pair(W83667HG, "Winbond W83667HG");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0xB0:
			switch (Revision & 0xF0) {
			case 0x70:
				this->ChipID = std::make_pair(W83627DHGP, "Winbond W83627DHGP");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0xB3:
			switch (Revision & 0xF0) {
			case 0x50:
				this->ChipID = std::make_pair(W83667HGB, "Winbond W83667HGB");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0xB4:
			switch (Revision & 0xF0) {
			case 0x70:
				this->ChipID = std::make_pair(NCT6771F, "Nuvoton NCT6771F");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0xC3:
			switch (Revision & 0xF0) {
			case 0x30:
				this->ChipID = std::make_pair(NCT6776F, "Nuvoton NCT6776F");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0xC4:
			switch (Revision & 0xF0) {
			case 0x50:
				this->ChipID = std::make_pair(NCT610X, "Nuvoton NCT610X");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0xC5:
			switch (Revision & 0xF0) {
			case 0x60:
				this->ChipID = std::make_pair(NCT6779D, "Nuvoton NCT6779D");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		case 0xC8:
			switch (Revision) {
			case 0x03:
				this->ChipID = std::make_pair(NCT6791D, "Nuvoton NCT6791D");
				logicalDeviceNumber = WINBOND_NUVOTON_HARDWARE_MONITOR_LDN;
				break;
			} break;
		default:
			boost::format fmt = boost::format("Sio UnknownId %02X Revision %02X") % ChipID % Revision;
			this->ChipID = std::make_pair(ChipID, fmt.str());
			state = false;
			break;
		}
		if(state)
		{
			this->SelectPage(port, data, (UCHAR)logicalDeviceNumber);
			SIOReadWord(port, data,BASE_ADDRESS_REGISTER, LPC_Address);
			Sleep(1);
			DWORD verify = 0;
			SIOReadWord(port, data, BASE_ADDRESS_REGISTER, verify);	

			if (LPC_Address != verify)
				LPC_Address = 0;

			if (LPC_Address == verify && this->ChipID.first == NCT6791D) {
				NuvotonDisableIOSpaceLock(port, data);
			}
			// some Fintek chips have address register offset 0x05 added already
			if ((LPC_Address & 0x07) == 0x05)
				LPC_Address &= 0xFFF8;
			DWORD vendorID = 0;
			switch (this->ChipID.first) 
			{
			case W83627DHG:
			case W83627DHGP:
			case W83627EHF:
			case W83627HF:
			case W83627THF:
			case W83667HG:
			case W83667HGB:
			case W83687THF:
				Winbond = true;
				break;
			case NCT610X:
			case NCT6771F:
			case NCT6776F:
			case NCT6779D:
			case NCT6791D:
				Nuvoton = true;
				break;
			case F71858:
			case F71862:
			case F71869:
			case F71869A:
			case F71882:
			case F71889AD:
			case F71889ED:
			case F71889F:
			case F71808E:
				SV_ASSIST::Ring0::RdIOPort(FINTEK_VENDOR_ID_REGISTER, 2, vendorID);
				vendorID = ((vendorID & 0xFF) << 8) | ((vendorID & 0xFF00) >> 8);
				if (vendorID == FINTEK_VENDOR_ID) {
					Fintek = true;
				}
				break;
			default:
				break;
			}
		}
		this->ExitWinbondFintek(port);
	}
	return state;
}

bool SuperIoEntry::ReadWinbondFintekPage(USHORT port, USHORT data, USHORT logicalDeviceNumber, Super_IO_Data & SIO_Data)
{
	bool state = true;
	if (port != 0x2E)
		return false;
	this->EntryWinbondFintek(port);

	if(Nuvoton||Winbond||Fintek)
	{
		this->SelectPage(port, data, (UCHAR)logicalDeviceNumber);
		for (USHORT j = 0; j < 0x100; ++j)
		{
			DWORD SuperIO_Data = 0;
			SIOReadByte(port, data, j, SuperIO_Data);
			SIO_Data.SuperioData[j] = (UCHAR)SuperIO_Data;
		}
		this->ExitWinbondFintek(port);
	}
	return state;
}
void SuperIoEntry::NuvotonDisableIOSpaceLock(USHORT port, USHORT data)
{
	DWORD options = 0;
	SIOReadByte(port, data, NUVOTON_HARDWARE_MONITOR_IO_SPACE_LOCK, options);

	if ((options & 0x10) > 0) {
		// disable the i/o space lock
		SV_ASSIST::Ring0::WrIOPort(port, 1, NUVOTON_HARDWARE_MONITOR_IO_SPACE_LOCK);
		SV_ASSIST::Ring0::WrIOPort(data, 1, (BYTE)(options & ~0x10));
	}
}

void SuperIoEntry::EntryWinbondFintek(USHORT port)
{
	SV_ASSIST::Ring0::WrIOPort(port, 1, 0x87);
	SV_ASSIST::Ring0::WrIOPort(port, 1, 0x87);
}

void SuperIoEntry::ExitWinbondFintek(USHORT port)
{
	SV_ASSIST::Ring0::WrIOPort(port, 1, 0xAA);
}
#pragma endregion

#pragma region SMSC
bool SuperIoEntry::DetectSMSC()
{
	bool state = false;
	for (auto SIO : SIO_Port)
	{
		if (this->ReadSMSC(SIO.first, SIO.second))
		{
			state = true;
			break;
		}
	}
	return state;
}

bool SuperIoEntry::ReadSMSC(USHORT port, USHORT data)
{
	bool state = true;
	this->EntrySMSC(port);
	DWORD ChipID = 0;
	SIOReadWord(port, data, CHIP_ID_REGISTER, ChipID);
	if (ChipID == 0 || ChipID == 0xFFFF)
	{
		OutputDebugPrintf(_T("Probably SMSC doesnt exist\n"));
		state = false;
	}
	else
	{
		//未知的信息
		switch (ChipID)
		{
		default:
			boost::format fmt = boost::format("SMSC Unknown Id %04X") % ChipID;
			this->ChipID = std::make_pair(ChipID, fmt.str());
			state = false;
			break;
		}
// 		this->SelectPage(port, data, IT87_ENVIRONMENT_CONTROLLER);
// 		SV_ASSIST::Ring0::RdIOPort(BASE_ADDRESS_REGISTER, 1, LPC_Address);
		this->ExitSMSC(port);
	}
	return state;
}

bool SuperIoEntry::ReadSMSCPage(USHORT port, USHORT data, USHORT logicalDeviceNumber, Super_IO_Data & SIO_Data)
{
	bool state = true;
	if (port != 0x2E)
		return false;
	this->EntrySMSC(port);
	if (SMSC)
	{
		this->SelectPage(port, data, (UCHAR)logicalDeviceNumber);
		for (USHORT j = 0; j < 0x100; ++j)
		{
			DWORD SuperIO_Data = 0;
			SIOReadByte(port, data, j, SuperIO_Data);
			SIO_Data.SuperioData[j] = (UCHAR)SuperIO_Data;
		}
		this->ExitSMSC(port);
	}
	return state;
}

void SuperIoEntry::EntrySMSC(USHORT port)
{
	SV_ASSIST::Ring0::WrIOPort(port, 1, 0x55);
}

void SuperIoEntry::ExitSMSC(USHORT port)
{
	SV_ASSIST::Ring0::WrIOPort(port, 1, 0xAA);
}
#pragma endregion
