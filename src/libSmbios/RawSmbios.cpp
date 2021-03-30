#include "stdafx.h"
#include <windows.h>
#include <sysinfoapi.h>

#include "RawSmbios.h"

using Smbios::RawSmbios;

RawSmbios* Smbios::RawSmbios::Instance()
{
	static RawSmbios inst;
	return &inst;
}

void* Smbios::RawSmbios::GetPhyAdderss() const
{
	return SmbiosBaseAddress;
}

RawSmbios::RawSmbios() : SmbiosBaseAddress(nullptr)
{
	auto buffersize = GetSystemFirmwareTable('RSMB', 0, 0, 0);
	if (buffersize)
	{
		auto buf = new BYTE[buffersize]{};
		auto ret = GetSystemFirmwareTable('RSMB', 0, buf, buffersize);
		if (ret)
		{
			SmbiosBaseAddress = buf;
		}
		else
		{
			delete[] buf;
		}
	}
}

RawSmbios::~RawSmbios()
{
	if (SmbiosBaseAddress)
	{
		delete[] SmbiosBaseAddress;
		SmbiosBaseAddress = nullptr;
	}
}