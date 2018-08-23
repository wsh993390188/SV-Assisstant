#include "stdafx.h"
#include "ACPI Source.h"
#include "..\driver\DriverOrigin.h"

CACPI::CACPI() : IsUEFI{ false }, ACPIAddress{}, isSupportACPI{ true }, acpi_rsdp{}, acpi_rsdt{}, acpi_mcfg{}, acpi_facp{}, acpi_fpdt{},
acpi_facs{}, acpi_hpet{}, acpi_sbst{}, acpi_bgrt{}
{
	this->GetACPIAddress();
	GetFirmwareEnvironmentVariable(L"", L"{00000000-0000-0000-0000-000000000000}", NULL, 0);
	if (GetLastError() == ERROR_INVALID_FUNCTION) {
		IsUEFI = false;
	}
	else {
		IsUEFI = true;
	}
}

CACPI::~CACPI()
{
}

void CACPI::Excute()
{
	this->GetXSDT_RSDT();
	this->GetOtherTabels();
}

void CACPI::GetACPIAddress()
{
	char signat[9] = { 0x52,0x53,0x44,0x20,0x50,0x54,0x52,0x20,0 };
	bool state = false;
	//find RSDP(root System Description Pointer)
	for (size_t addr = 0xE0000; addr <= (0xFFFFF); addr += 0x10)//16 bytes boundaries
	{
		int count = 0;
		ULONG hi = 0;
		ZhaoxinDriver::Instance()->RdMemory(addr, 1, hi);
		for (size_t i = 0; hi == signat[i]; ++i)
		{
			++count;
			if ((signat[i + 1] == '\0') && (count == 8))
			{
				ACPIAddress = addr;
				state = true;
				break;
			}
			ZhaoxinDriver::Instance()->RdMemory(addr + i + 1, 1, hi);
		}
		if (state)
			break;
	}

	if (!ACPIAddress)
	{
		bool state = false;
		//find RSDP(root System Description Pointer)
		for (size_t addr = 0x40E; addr < 0x40E+1024; addr += 0x10)//16 bytes boundaries
		{
			int count = 0;
			ULONG hi = 0;
			ZhaoxinDriver::Instance()->RdMemory(addr, 1, hi);
			for (size_t i = 0; hi == signat[i]; ++i)
			{
				++count;
				if ((signat[i + 1] == '\0') && (count == 8))
				{
					ACPIAddress = addr;
					state = true;
					break;
				}
				ZhaoxinDriver::Instance()->RdMemory(addr + i + 1, 1, hi);
			}
			if (state)
				break;
		}
	}
	
	if (!ACPIAddress)
		isSupportACPI = false;
}

void CACPI::GetXSDT_RSDT()
{
	if (isSupportACPI)
	{
		for (size_t i = 0; i < sizeof(ACPI_RSDP_STRUCTURE); i++)
		{
			ULONG temp = 0;
			ZhaoxinDriver::Instance()->RdMemory(ACPIAddress + i, 1, temp);
			memcpy((PUCHAR)(&acpi_rsdp) + i, &temp, 1);
		}

		if(acpi_rsdp.RsdtAddress)
		{
			for (size_t i = 0; i < 36; i++)
			{
				ULONG temp = 0;
				ZhaoxinDriver::Instance()->RdMemory(acpi_rsdp.RsdtAddress + i, 1, temp);
				memcpy((PUCHAR)(&acpi_rsdt) + i, &temp, 1);
			}

			for (size_t i = 36; i < acpi_rsdt.Length; i+=4)
			{
				ULONG temp = 0;
				ZhaoxinDriver::Instance()->RdMemory(acpi_rsdp.RsdtAddress + i, 4, temp);
				acpi_rsdt.Entry.emplace_back(temp);
			}
		}

		if(acpi_rsdp.XsdtAddress)
		{
			for (size_t i = 0; i < 36; i++)
			{
				DWORD temp = 0;
				ZhaoxinDriver::Instance()->RdMemory(acpi_rsdp.XsdtAddress + i, 1, temp);
				memcpy((PUCHAR)(&acpi_xsdt) + i, &temp, 1);
			}


			for (size_t i = 36; i < acpi_xsdt.Length; i += 8)
			{
				DWORD temp1 = 0, temp2 = 0;
				ZhaoxinDriver::Instance()->RdMemory(acpi_rsdp.XsdtAddress + i, 4, temp1);
				ZhaoxinDriver::Instance()->RdMemory(acpi_rsdp.XsdtAddress + i + 4, 4, temp2);
				acpi_xsdt.Entry.emplace_back((temp2 << 31) | temp1);
			}
		}
	}
}

void CACPI::GetOtherTabels()
{
	if (IsUEFI)
	{
		for (const auto& addr : this->acpi_xsdt.Entry)
		{
			ULONG signature = 0;
			ZhaoxinDriver::Instance()->RdMemory(addr, 4, signature);
			ULONG length = 0;
			ZhaoxinDriver::Instance()->RdMemory(addr + 4, 1, length);
			if (signature == 'GFCM')
			{
				for (size_t i = 0; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(&acpi_mcfg) + i, &temp, 1);
				}
			}
			else if (signature == 'PCAF')
			{
				for (size_t i = 0; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(&acpi_facp) + i, &temp, 1);
				}
			}
			else if (signature == 'SCAF')
			{
				acpi_facs = std::make_shared<ACPI_FACS_STRUCTURE>();
				for (size_t i = 0; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)acpi_facs.get() + i, &temp, 1);
				}
			}
			else if (signature == 'TDPF')
			{
				acpi_fpdt = std::make_shared<ACPI_FPDT_STRUCTURE>();
				for (size_t i = 0; i < 36; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)acpi_fpdt.get() + i, &temp, 1);
				}

// 				for (size_t i = 36; i < acpi_fpdt->Length; i+= sizeof(Performance_Record_Structure))
// 				{
// 					Performance_Record_Structure adds = {};
// 					for (size_t j = i;( j < j+sizeof(Performance_Record_Structure)) && (j < acpi_fpdt->Length); j++)
// 					{
// 						ULONG temp = 0;
// 						ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
// 						memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
// 					}
// 					acpi_fpdt->Performance_Records.emplace_back(adds);
// 				}
			}
			else if (signature == 'TEPH')
			{
				for (size_t i = 0; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(&acpi_hpet) + i, &temp, 1);
				}
			}
			else if (signature == 'TSBS')
			{
				acpi_sbst = std::make_shared<ACPI_SBST_STRUCTURE>();
				for (size_t i = 0; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(acpi_sbst.get()) + i, &temp, 1);
				}
			}
			else if (signature == 'TRGB')
			{
				acpi_bgrt = std::make_shared<ACPI_BGRT_STRUCTURE>();
				for (size_t i = 0; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(acpi_bgrt.get()) + i, &temp, 1);
				}
			}
		}
	}
	else
	{
		for (const auto& addr : this->acpi_rsdt.Entry)
		{
			ULONG signature = 0;
			ZhaoxinDriver::Instance()->RdMemory(addr, 4, signature);
			ULONG length = 0;
			ZhaoxinDriver::Instance()->RdMemory(addr + 4, 1, length);
			if (signature == 'MCFG')
			{
				for (size_t i = 0; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(&acpi_mcfg) + i, &temp, 1);
				}
			}
			else if (signature == 'PCAF')
			{
				for (size_t i = 0; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(&acpi_facp) + i, &temp, 1);
				}
			}
			else if (signature == 'SCAF')
			{
				acpi_facs = std::make_shared<ACPI_FACS_STRUCTURE>();
				for (size_t i = 0; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)acpi_facs.get() + i, &temp, 1);
				}
			}
			else if (signature == 'TEPH')
			{
				for (size_t i = 0; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(&acpi_hpet) + i, &temp, 1);
				}
			}
		}
	}
}





