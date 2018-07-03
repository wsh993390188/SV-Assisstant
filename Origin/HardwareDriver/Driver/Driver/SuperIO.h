#pragma once

#ifndef _SuperIO
#define _SuPerIO
#define ITE_DEVCIE_SELECT_REGISTER		0x07
typedef struct _SuPer_IO
{
	UCHAR SuperioData[256];
}Super_IO;
#endif // !_SuperIO

enum ITE_Chip_Func
{
	FDC = 0x00,
	SerialPort1 = 0x01,
	SerialPort2 = 0x02,
	ParallelPort = 0x03,
	EC = 0x04,
	KBC_Keybroad = 0x05,
	KBC_Mouse = 0x06,
	GPIO = 0x07,
	SerialPort3 = 0x08,
	SerialPort4 = 0x09,
	SerialPort5 = 0x0A,
	SerialPort6 = 0x0B,
	ConsumerIR = 0x0C
};

const UCHAR ITE_Entry[4] = {0x87,0x01,0x55,0x55};
const UCHAR ITE_Exit[2] = { 0x02,0x02 };

const UCHAR WinbondNuvotonFintek_Entry[] = { 0x87, 0x87 };
const UCHAR WinbondNuvotonFintek_Exit[] = { 0xAA };

const UCHAR SMSC_Enter[] = { 0x55 };
const UCHAR SMSC_xit[] = { 0xAA };