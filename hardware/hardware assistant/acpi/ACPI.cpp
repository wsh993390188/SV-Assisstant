// ACPI.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ACPI Source.h"
#include "ACPI.h"
#include <memory>


namespace SV_ASSIST
{
	namespace ACPI
	{
		class CACPI_info {
		public:
			static CACPI_info* Instance();
			CACPI_info(void);
			~CACPI_info();	
			BOOL Excute();
		private:
			std::shared_ptr<CACPI> acpi;
		};

		CACPI_info::CACPI_info() : acpi(new CACPI())
		{
		}

		CACPI_info::~CACPI_info()
		{
		}

		BOOL CACPI_info::Excute()
		{
			if (acpi->isSupportACPI)
			{
				acpi->Get();
			}
			return acpi->isSupportACPI;
		}

		const ACPI_MCFG_STRUCTURE& GetMCFG()
		{
			return acpi->mcfg;
		}

		const ACPI_MCFG_STRUCTURE& GetRSDT()
		{
			return acpi->rsdt;
		}
	}
}
