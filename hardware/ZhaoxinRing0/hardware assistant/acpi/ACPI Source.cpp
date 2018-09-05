#include "stdafx.h"
#include "ACPI Source.h"
#include "..\driver\DriverOrigin.h"

CACPI::CACPI() : IsACPI1_0{ false }, ACPIAddress{}, isSupportACPI{ true }, acpi_rsdp{}, acpi_rsdt{}, acpi_mcfg{}, acpi_facp{}, acpi_fpdt{},
acpi_facs{}, acpi_hpet{}, acpi_sbst{}, acpi_bgrt{}, acpi_apic{}, acpi_cpep{}, acpi_ecdt{}
{
	this->GetACPIAddress();
	this->GetXSDT_RSDT();
	if (acpi_rsdp.Revision == 1)
		IsACPI1_0 = true;
}

CACPI::~CACPI()
{
}

void CACPI::Excute()
{
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
	if (!IsACPI1_0)
	{
		for (const auto& addr : this->acpi_xsdt.Entry)
		{
			ULONG signature = 0;
			ZhaoxinDriver::Instance()->RdMemory(addr, 4, signature);
			ULONG length = 0;
			ZhaoxinDriver::Instance()->RdMemory(addr + 4, 4, length);
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
			else if (signature == 'PEPC')
			{
				acpi_cpep = std::make_shared<ACPI_CPEP_STRUCTURE>();
				for (size_t i = 0; i < 44; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)acpi_cpep.get() + i, &temp, 1);
				}
				for (size_t i = 44; i < acpi_cpep->Length; i+= sizeof(Corrected_Platform_Error_Polling_Processor_Structure))
				{
					Corrected_Platform_Error_Polling_Processor_Structure adds = {};
					for (size_t j = i; j < acpi_cpep->Length && j < i + sizeof(Corrected_Platform_Error_Polling_Processor_Structure); j++)
					{
						ULONG temp = 0;
						ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
						memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
					}
					acpi_cpep->CPEP_Processor_Structure.emplace_back(adds);
				}
			}
			else if (signature == 'CIPA')
			{
				for (size_t i = 0; i < 44; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(&acpi_apic) + i, &temp, 1);
				}
				bool Iscompleted = false;
				for (size_t i = 44; i < acpi_apic.Length;)
				{
					MADT_Interrupt_Controller adds = {};
					Local_SAPIC_Structure Local_IO_SAPIC = {};
					char a[0x100] = {};
					ULONG apicType = {}, apicLength = {};
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, apicType);
					ZhaoxinDriver::Instance()->RdMemory(addr + i + 1, 1, apicLength);
					switch (apicType)
					{
					case 0:
						if (apicLength == 8)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Processor_Local_APIC.emplace_back(adds.Processor_Local_APIC);
						}
						break;
					case 1:
						if (apicLength == 12)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.IO_APIC.emplace_back(adds.IO_APIC);
						}
						break;
					case 2:
						if (apicLength == 10)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Interrupt_Source_Override.emplace_back(adds.Interrupt_Source_Override);
						}
						break;
					case 3:
						if (apicLength == 8)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.NMI_Source.emplace_back(adds.NMI_Source);
						}
						break;
					case 4:
						if (apicLength == 6)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Local_APIC_NMI.emplace_back(adds.Local_APIC_NMI);
						}
						break;
					case 5:
						if (apicLength == 12)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Local_APIC_Address_Override.emplace_back(adds.Local_APIC_Address_Override);
						}
						break;
					case 6:
						if (apicLength == 16)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.IO_SAPIC.emplace_back(adds.IO_SAPIC);
						}
						break;
					case 7:

						for (size_t j = i; j < acpi_apic.Length && j < 16; j++)
						{
							ULONG temp = 0;
							ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
							memcpy((PUCHAR)(&Local_IO_SAPIC) + j - i, &temp, 1);
						}
						for (size_t j = i + 16; j < acpi_apic.Length && j < i + apicLength; j++)
						{
							ULONG temp = 0;
							ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
							memcpy(&a[0] + j - i - 16, &temp, 1);
						}
						Local_IO_SAPIC.ACPI_Processor_UID_String = a;
						acpi_apic.Local_IO_SAPIC.emplace_back(Local_IO_SAPIC);
						break;
					case 8:
						if (apicLength == 16)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Platform_Interrupt_Sources.emplace_back(adds.Platform_Interrupt_Sources);
						}
						break;
					case 9:
						if (apicLength == 12)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Local_x2APIC_NMI.emplace_back(adds.Local_x2APIC_NMI);
						}
						break;
					case 0xA:
						if (apicLength == 16)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Platform_Interrupt_Sources.emplace_back(adds.Platform_Interrupt_Sources);
						}
						break;
					case 0xB:
						if (apicLength == 80)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.GICC.emplace_back(adds.GICC);
						}
						break;
					case 0xC:
						if (apicLength == 24)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.GICD.emplace_back(adds.GICD);
						}
						break;
					case 0xD:
						if (apicLength == 24)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.GIC_MSI_Frame.emplace_back(adds.GIC_MSI_Frame);
						}
						break;
					case 0xE:
						if (apicLength == 16)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.GICR.emplace_back(adds.GICR);
						}
						break;
					case 0xF:
						if (apicLength == 20)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.ITS.emplace_back(adds.ITS);
						}
						break;
					default:
						if(apicLength == 0)
							Iscompleted = true;
						break;
					}
					i += apicLength;
					if (Iscompleted)
						break;
				}
				if (!acpi_apic.Processor_Local_APIC.empty())
				std::sort(acpi_apic.Processor_Local_APIC.begin(), acpi_apic.Processor_Local_APIC.end(), [&](const APIC_Structure& a1, const APIC_Structure& a2) -> bool {return a1.APIC_ID < a2.APIC_ID; });
				if (!acpi_apic.Local_APIC_NMI.empty())
				std::sort(acpi_apic.Local_APIC_NMI.begin(), acpi_apic.Local_APIC_NMI.end(), [&](const Local_APIC_NMI_Structure& a1, const Local_APIC_NMI_Structure& a2) -> bool {return a1.ACPI_Processor_UID < a2.ACPI_Processor_UID; });
				if(!acpi_apic.Processor_Local_X2APIC.empty())
				std::sort(acpi_apic.Processor_Local_X2APIC.begin(), acpi_apic.Processor_Local_X2APIC.end(), [&](const Processor_Local_X2APIC_Structure& a1, const Processor_Local_X2APIC_Structure& a2) -> bool {return a1.ACPI_Processor_UID < a2.ACPI_Processor_UID; });
				if (!acpi_apic.Local_x2APIC_NMI.empty())
				std::sort(acpi_apic.Local_x2APIC_NMI.begin(), acpi_apic.Local_x2APIC_NMI.end(), [&](const Local_X2APIC_NMI_Structure& a1, const Local_X2APIC_NMI_Structure& a2) -> bool {return a1.ACPI_Processor_UID < a2.ACPI_Processor_UID; });
			}
			else if (signature == 'TDCE')
			{
				acpi_ecdt = std::make_shared<ACPI_ECDT_STRUCTURE>();
				for (size_t i = 0; i < 65; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(acpi_ecdt.get()) + i, &temp, 1);
				}
				char a[0xFF] = {};
				for (size_t i = 65; i < length; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((&a[0]) + i - 65, &temp, 1);
				}
				acpi_ecdt->EC_ID = a;
			}
			else if (signature == 'TDSS')
			{
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
			else if (signature == 'PEPC')
			{
				acpi_cpep = std::make_shared<ACPI_CPEP_STRUCTURE>();
				for (size_t i = 0; i < 44; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)acpi_cpep.get() + i, &temp, 1);
				}
				for (size_t i = 44; i < acpi_cpep->Length; i += sizeof(Corrected_Platform_Error_Polling_Processor_Structure))
				{
					Corrected_Platform_Error_Polling_Processor_Structure adds = {};
					for (size_t j = i; j < acpi_cpep->Length && j < sizeof(Corrected_Platform_Error_Polling_Processor_Structure); j++)
					{
						ULONG temp = 0;
						ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
						memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
					}
					acpi_cpep->CPEP_Processor_Structure.emplace_back(adds);
				}
			}
			else if (signature == 'CIPA')
			{
				for (size_t i = 0; i < 44; i++)
				{
					ULONG temp = 0;
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, temp);
					memcpy((PUCHAR)(&acpi_apic) + i, &temp, 1);
				}
				bool Iscompleted = false;
				for (size_t i = 44; i < acpi_apic.Length;)
				{
					MADT_Interrupt_Controller adds = {};
					Local_SAPIC_Structure Local_IO_SAPIC = {};
					char a[0x100] = {};
					ULONG apicType = {}, apicLength = {};
					ZhaoxinDriver::Instance()->RdMemory(addr + i, 1, apicType);
					ZhaoxinDriver::Instance()->RdMemory(addr + i + 1, 1, apicLength);
					switch (apicType)
					{
					case 0:
						if (apicLength == 8)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Processor_Local_APIC.emplace_back(adds.Processor_Local_APIC);
						}
						break;
					case 1:
						if (apicLength == 12)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.IO_APIC.emplace_back(adds.IO_APIC);
						}
						break;
					case 2:
						if (apicLength == 10)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Interrupt_Source_Override.emplace_back(adds.Interrupt_Source_Override);
						}
						break;
					case 3:
						if (apicLength == 8)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.NMI_Source.emplace_back(adds.NMI_Source);
						}
						break;
					case 4:
						if (apicLength == 6)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Local_APIC_NMI.emplace_back(adds.Local_APIC_NMI);
						}
						break;
					case 5:
						if (apicLength == 12)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Local_APIC_Address_Override.emplace_back(adds.Local_APIC_Address_Override);
						}
						break;
					case 6:
						if (apicLength == 16)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.IO_SAPIC.emplace_back(adds.IO_SAPIC);
						}
						break;
					case 7:

						for (size_t j = i; j < acpi_apic.Length && j < 16; j++)
						{
							ULONG temp = 0;
							ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
							memcpy((PUCHAR)(&Local_IO_SAPIC) + j - i, &temp, 1);
						}
						for (size_t j = i + 16; j < acpi_apic.Length && j < i + apicLength; j++)
						{
							ULONG temp = 0;
							ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
							memcpy(&a[0] + j - i - 16, &temp, 1);
						}
						Local_IO_SAPIC.ACPI_Processor_UID_String = a;
						acpi_apic.Local_IO_SAPIC.emplace_back(Local_IO_SAPIC);
						break;
					case 8:
						if (apicLength == 16)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Platform_Interrupt_Sources.emplace_back(adds.Platform_Interrupt_Sources);
						}
						break;
					case 9:
						if (apicLength == 12)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Local_x2APIC_NMI.emplace_back(adds.Local_x2APIC_NMI);
						}
						break;
					case 0xA:
						if (apicLength == 16)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.Platform_Interrupt_Sources.emplace_back(adds.Platform_Interrupt_Sources);
						}
						break;
					case 0xB:
						if (apicLength == 80)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.GICC.emplace_back(adds.GICC);
						}
						break;
					case 0xC:
						if (apicLength == 24)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.GICD.emplace_back(adds.GICD);
						}
						break;
					case 0xD:
						if (apicLength == 24)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.GIC_MSI_Frame.emplace_back(adds.GIC_MSI_Frame);
						}
						break;
					case 0xE:
						if (apicLength == 16)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.GICR.emplace_back(adds.GICR);
						}
						break;
					case 0xF:
						if (apicLength == 20)
						{
							for (size_t j = i; j < acpi_apic.Length && j < i + apicLength; j++)
							{
								ULONG temp = 0;
								ZhaoxinDriver::Instance()->RdMemory(addr + j, 1, temp);
								memcpy((PUCHAR)(&adds) + j - i, &temp, 1);
							}
							acpi_apic.ITS.emplace_back(adds.ITS);
						}
						break;
					default:
						if (apicLength == 0)
							Iscompleted = true;
						break;
					}
					i += apicLength;
					if (Iscompleted)
						break;
				}
				if (!acpi_apic.Processor_Local_APIC.empty())
					std::sort(acpi_apic.Processor_Local_APIC.begin(), acpi_apic.Processor_Local_APIC.end(), [&](const APIC_Structure& a1, const APIC_Structure& a2) -> bool {return a1.APIC_ID < a2.APIC_ID; });
				if (!acpi_apic.Local_APIC_NMI.empty())
					std::sort(acpi_apic.Local_APIC_NMI.begin(), acpi_apic.Local_APIC_NMI.end(), [&](const Local_APIC_NMI_Structure& a1, const Local_APIC_NMI_Structure& a2) -> bool {return a1.ACPI_Processor_UID < a2.ACPI_Processor_UID; });
				if (!acpi_apic.Processor_Local_X2APIC.empty())
					std::sort(acpi_apic.Processor_Local_X2APIC.begin(), acpi_apic.Processor_Local_X2APIC.end(), [&](const Processor_Local_X2APIC_Structure& a1, const Processor_Local_X2APIC_Structure& a2) -> bool {return a1.ACPI_Processor_UID < a2.ACPI_Processor_UID; });
				if (!acpi_apic.Local_x2APIC_NMI.empty())
					std::sort(acpi_apic.Local_x2APIC_NMI.begin(), acpi_apic.Local_x2APIC_NMI.end(), [&](const Local_X2APIC_NMI_Structure& a1, const Local_X2APIC_NMI_Structure& a2) -> bool {return a1.ACPI_Processor_UID < a2.ACPI_Processor_UID; });
			}
		}
	}

	if (acpi_facp.FIRMWARE_CTRL)
	{
		ULONG signature = 0;
		ZhaoxinDriver::Instance()->RdMemory(acpi_facp.FIRMWARE_CTRL, 4, signature);
		ULONG length = 0;
		ZhaoxinDriver::Instance()->RdMemory(acpi_facp.FIRMWARE_CTRL + 4, 4, length);
		if (signature == 'SCAF')
		{
			acpi_facs = std::make_shared<ACPI_FACS_STRUCTURE>();
			for (size_t i = 0; i < length; i++)
			{
				ULONG temp = 0;
				ZhaoxinDriver::Instance()->RdMemory(acpi_facp.FIRMWARE_CTRL + i, 1, temp);
				memcpy((PUCHAR)acpi_facs.get() + i, &temp, 1);
			}
		}
	}

	if (acpi_facp.DSDT)
	{

	}
}





