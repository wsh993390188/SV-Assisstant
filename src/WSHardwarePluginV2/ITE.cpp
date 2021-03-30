#include "stdafx.h"
#include "ITE.h"

Hardware::SIO::ITE::ITE() : SIOBase(), IT87_ENVIRONMENT_CONTROLLER{ 0x04u }, IT87_CHIP_VERSION_REGISTER{ 0x22u }
{
}

bool Hardware::SIO::ITE::InitializeSIO(const USHORT& port)
{
	this->IoPtr.WriteByte(port, 0x87);
	this->IoPtr.WriteByte(port, 0x01);
	this->IoPtr.WriteByte(port, 0x55);
	this->IoPtr.WriteByte(port, 0x55);
	return true;
}

bool Hardware::SIO::ITE::DeInitializeSIO(const USHORT& port)
{
	this->IoPtr.WriteByte(port, 0x02);
	this->IoPtr.WriteByte(port + 1, 0x02);
	return true;
}