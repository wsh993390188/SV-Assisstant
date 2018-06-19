#pragma once
#include <windows.h>
#include "ACPI Defination.h"


namespace SV_ASSIST 
{
	namespace ACPI
	{
		BOOL Excute();

		const ACPI_MCFG_STRUCTURE& GetMCFG();

		const ACPI_RSDT_STRUCTURE& GetRSDT();
	}
}
