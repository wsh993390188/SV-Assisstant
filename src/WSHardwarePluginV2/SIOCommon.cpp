#include "stdafx.h"
#include "SIOCommon.h"

bool Hardware::SIO::SIOCommon::InitializeSIO(const USHORT& port)
{
	return IoPtr.WriteByte(port, 0x87) && IoPtr.WriteByte(port, 0x87);
}

bool Hardware::SIO::SIOCommon::DeInitializeSIO(const USHORT& port)
{
	return IoPtr.WriteByte(port, 0xAA);
}