#include "stdafx.h"
#include "ITE.h"

Hardware::SIO::ITE::ITE() : SIOBase(), IT87_ENVIRONMENT_CONTROLLER{ 0x04u }, IT87_CHIP_VERSION_REGISTER{ 0x22u }
{
}

bool Hardware::SIO::ITE::InitializeSIO(const USHORT& port)
{
	bool ret1 = this->IoPtr.WriteByte(port, 0x87);
	bool ret2 = this->IoPtr.WriteByte(port, 0x01);
	bool ret3 = this->IoPtr.WriteByte(port, 0x55);
	bool ret4 = this->IoPtr.WriteByte(port, 0x55);
	return ret1 && ret2 && ret3 && ret4;
}

bool Hardware::SIO::ITE::DeInitializeSIO(const USHORT& port)
{
	bool ret1 = this->IoPtr.WriteByte(port, 0x02);
	bool ret2 = this->IoPtr.WriteByte(port + 1, 0x02);
	return ret1 && ret2;
}