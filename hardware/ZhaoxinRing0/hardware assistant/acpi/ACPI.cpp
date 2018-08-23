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
			static CACPI_info* Instance() 
			{ 
				if (!temp.get())
					temp= std::make_shared<CACPI_info>();
				return temp.get();
			}
			CACPI_info(void);
			~CACPI_info();
			BOOL Excute();
			const ACPI_MCFG_STRUCTURE& GetMCFG()
			{
				return acpi->acpi_mcfg;
			}

			const ACPI_RSDT_STRUCTURE& GetRSDT()
			{
				return acpi->acpi_rsdt;
			}
		private:
			static std::shared_ptr<CACPI_info> temp;
			std::shared_ptr<CACPI> acpi;
		};

		std::shared_ptr<CACPI_info> CACPI_info::temp = nullptr;

		CACPI_info::CACPI_info() : acpi(new CACPI())
		{
		}

		CACPI_info::~CACPI_info()
		{	}

		BOOL CACPI_info::Excute()
		{
			if (acpi->isSupportACPI)
			{
				acpi->Excute();
			}
			return acpi->isSupportACPI;
		}

		BOOL Excute()
		{
			return CACPI_info::Instance()->Excute();
		}

		const ACPI_MCFG_STRUCTURE& GetMCFG()
		{
			return CACPI_info::Instance()->GetMCFG();
		}

		const ACPI_RSDT_STRUCTURE& GetRSDT()
		{
			return CACPI_info::Instance()->GetRSDT();
		}

		const DWORD64 GetPCIEBaseAddress()
		{
			return CACPI_info::Instance()->GetMCFG().Configuration_space_base_address.Base_Address;
		}
	}
}