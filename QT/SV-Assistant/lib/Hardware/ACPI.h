#pragma once
#include "ACPI Defination.h"


namespace SV_ASSIST 
{
	namespace ACPI
	{
		BOOL Excute();

		const ACPI_MCFG_STRUCTURE& GetMCFG();

		const ACPI_RSDT_STRUCTURE& GetRSDT();
		
		/**************************************************************
		*@Function				GetPCIEBaseAddress
		*@brief					获取PCIE在Memory映射中的地址
		*@author				王硕
		*@param
			*@PCIE_BaseAddress	OUT 输出PCIE Configuration Space 在内存中的基地址
		*@return
			*@BOOL				获取PCIE基地址是否成功
		****************************************************************/
		const DWORD64 GetPCIEBaseAddress();
	}
}
