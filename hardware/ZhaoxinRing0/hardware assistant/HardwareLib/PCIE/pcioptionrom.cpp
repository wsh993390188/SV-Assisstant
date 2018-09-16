#include "stdafx.h"
#include "pcioptionrom.h"
#include "..\driver\DriverOrigin.h"

Option_Rom::Option_Rom() : OPDB{}
{ 
	this->Scan_Option_Rom();
	this->Get_Option_Rom();
}

const std::vector<std::pair<PCI_OPTION_ROM_HEADER, PCI_OPTION_ROM_Data_Structure>>& Option_Rom::GetPCIOptionRom() const
{
	return this->OPDB;
}

void Option_Rom::UpdateData()
{
	OPDB.clear();
	this->Scan_Option_Rom();
	this->Get_Option_Rom();
}

Option_Rom::~Option_Rom()
{
}

void Option_Rom::Scan_Option_Rom()
{
	for (size_t addr = 0xC0000; addr <= 0xE0000; addr += 0x10)
	{
		ULONG hi = 0, lo = 0;
		ZhaoxinDriver::Instance()->RdMemory(addr, 1, hi);
		ZhaoxinDriver::Instance()->RdMemory(addr + 1, 1, lo);
		if (hi == 0x55 && lo == 0xAA)
			this->OptionRomHeader.emplace_back(addr);
	}
}

void Option_Rom::Get_Option_Rom()
{
	for (const auto& i : this->OptionRomHeader)
	{
		PCI_OPTION_ROM_HEADER header = {};
		PCI_OPTION_ROM_Data_Structure data = {};

		size_t length = sizeof(PCI_OPTION_ROM_HEADER);
		ULONG hi = 0;
		ZhaoxinDriver::Instance()->RdMemory(i + 2, 1, hi);
		if (hi < sizeof(PCI_OPTION_ROM_HEADER))
			length = hi;
		for (size_t offset = 0; offset < length; offset++)
		{
			ULONG temp = 0;
			ZhaoxinDriver::Instance()->RdMemory(i + offset, 1, temp);
			memcpy((PUCHAR)(&header) + offset,&temp, 1);
		}

		auto addr = header.PCI_Data_Offset + i;
		if ((addr - i) > length)
		{
			for (size_t offset = 0; offset < sizeof(PCI_OPTION_ROM_Data_Structure); offset++)
			{
				ULONG temp = 0;
				ZhaoxinDriver::Instance()->RdMemory(addr + offset, 1, temp);
				memcpy((PUCHAR)(&data) + offset, &temp, 1);
			}
		}
		if (data.Signature[0] == 'P' &&data.Signature[1] == 'C' && data.Signature[2] == 'I'&&data.Signature[3] == 'R')
		{
			auto optionrom = std::make_pair(header, data);
			OPDB.emplace_back(optionrom);
		}
	}
}

