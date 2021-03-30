#include "stdafx.h"
#include "Nuvoton.h"

namespace
{
	constexpr auto NUVOTON_HARDWARE_MONITOR_IO_SPACE_LOCK = 0x28u;
}

Hardware::SIO::Nuvoton::Nuvoton()
{
}

bool Hardware::SIO::Nuvoton::InitializeSIO(const USHORT& port)
{
	IoPtr.WriteByte(port, 0x87);
	IoPtr.WriteByte(port, 0x87);
	return true;
}

bool Hardware::SIO::Nuvoton::DeInitializeSIO(const USHORT& port)
{
	IoPtr.WriteByte(port, 0xAA);
	return true;
}

void Hardware::SIO::Nuvoton::NuvotonDisableIOSpaceLock(const USHORT& port, const USHORT& data)
{
	DWORD options = 0;
	SIOReadByte(port, data, NUVOTON_HARDWARE_MONITOR_IO_SPACE_LOCK, options);

	if ((options & 0x10) > 0) {
		// disable the i/o space lock
		IoPtr.WriteByte(port, NUVOTON_HARDWARE_MONITOR_IO_SPACE_LOCK);
		IoPtr.WriteByte(data, (BYTE)(options & ~0x10));
	}
}