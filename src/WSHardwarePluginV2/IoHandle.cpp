#include "stdafx.h"
#include "IoHandle.h"
#ifdef NewRing0
#include "Sys\DriverLoad\WinRing0.h"
#else
#include "HardwareRing0.h"
#endif

bool Hardware::Utils::Ring0::IoHandle::ReadByte(const USHORT IoAddress, DWORD& value)
{
	return WinRing0::Instance().RdIOPort(IoAddress, 1, value) == TRUE;
}

bool Hardware::Utils::Ring0::IoHandle::ReadWORD(const USHORT IoAddress, DWORD& value)
{
	return WinRing0::Instance().RdIOPort(IoAddress, 2, value) == TRUE;
}

bool Hardware::Utils::Ring0::IoHandle::ReadDWORD(const USHORT IoAddress, DWORD& value)
{
	return WinRing0::Instance().RdIOPort(IoAddress, 4, value) == TRUE;
}

bool Hardware::Utils::Ring0::IoHandle::WriteByte(const USHORT IoAddress, const DWORD value)
{
	return WinRing0::Instance().WrIOPort(IoAddress, 1, value) == TRUE;
}

bool Hardware::Utils::Ring0::IoHandle::WriteWORD(const USHORT IoAddress, const DWORD value)
{
	return WinRing0::Instance().WrIOPort(IoAddress, 2, value) == TRUE;
}

bool Hardware::Utils::Ring0::IoHandle::WriteDWORD(const USHORT IoAddress, const DWORD value)
{
	return WinRing0::Instance().WrIOPort(IoAddress, 4, value) == TRUE;
}