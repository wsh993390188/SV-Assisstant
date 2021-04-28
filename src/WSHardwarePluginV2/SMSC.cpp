#include "stdafx.h"
#include "SMSC.h"

bool Hardware::SIO::SMSC::InitializeSIO(const USHORT& port)
{
	return IoPtr.WriteByte(port, 0x55);
}

bool Hardware::SIO::SMSC::DeInitializeSIO(const USHORT& port)
{
	return IoPtr.WriteByte(port, 0xAA);
}