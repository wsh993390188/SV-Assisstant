#include "stdafx.h"
#include "MemoryHandle.h"
#ifdef NewRing0
#include "Sys\DriverLoad\WinRing0.h"
#else
#include "HardwareRing0.h"
#endif

bool Hardware::Utils::Ring0::MemoryHandle::ReadByte(const DWORD MemoryAddress, DWORD& value)
{
	return WinRing0::Instance().RdMemory(MemoryAddress, 1, value) == TRUE;
}

bool Hardware::Utils::Ring0::MemoryHandle::ReadWORD(const DWORD MemoryAddress, DWORD& value)
{
	return WinRing0::Instance().RdMemory(MemoryAddress, 2, value) == TRUE;
}

bool Hardware::Utils::Ring0::MemoryHandle::ReadDWORD(const DWORD MemoryAddress, DWORD& value)
{
	return WinRing0::Instance().RdMemory(MemoryAddress, 4, value) == TRUE;
}

bool Hardware::Utils::Ring0::MemoryHandle::WriteByte(const DWORD MemoryAddress, const DWORD value)
{
	return WinRing0::Instance().WrMemory(MemoryAddress, 1, value) == TRUE;
}

bool Hardware::Utils::Ring0::MemoryHandle::WriteWORD(const DWORD MemoryAddress, const DWORD value)
{
	return WinRing0::Instance().WrMemory(MemoryAddress, 2, value) == TRUE;
}

bool Hardware::Utils::Ring0::MemoryHandle::WriteDWORD(const DWORD MemoryAddress, const DWORD value)
{
	return WinRing0::Instance().WrMemory(MemoryAddress, 4, value) == TRUE;
}