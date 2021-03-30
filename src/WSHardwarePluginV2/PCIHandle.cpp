#include "stdafx.h"
#include "PCIHandle.h"
#ifdef NewRing0
#include "Sys\DriverLoad\WinRing0.h"
#else
#include "HardwareRing0.h"
#endif

bool Hardware::Utils::Ring0::PCIHandle::ReadByte(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, DWORD& value)
{
	return WinRing0::Instance().ReadPci(bus, dev, func, offset, 1, value) == TRUE;
}

bool Hardware::Utils::Ring0::PCIHandle::ReadWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, DWORD& value)
{
	return WinRing0::Instance().ReadPci(bus, dev, func, offset, 2, value) == TRUE;
}

bool Hardware::Utils::Ring0::PCIHandle::ReadDWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, DWORD& value)
{
	return WinRing0::Instance().ReadPci(bus, dev, func, offset, 4, value) == TRUE;
}

bool Hardware::Utils::Ring0::PCIHandle::WriteByte(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, const DWORD value)
{
	return WinRing0::Instance().WritePci(bus, dev, func, offset, 1, value) == TRUE;
}

bool Hardware::Utils::Ring0::PCIHandle::WriteWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, const DWORD value)
{
	return WinRing0::Instance().WritePci(bus, dev, func, offset, 2, value) == TRUE;
}

bool Hardware::Utils::Ring0::PCIHandle::WriteDWORD(const USHORT bus, const USHORT dev, const USHORT func, const USHORT offset, const DWORD value)
{
	return WinRing0::Instance().WritePci(bus, dev, func, offset, 4, value) == TRUE;
}