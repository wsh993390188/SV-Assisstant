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
			const ACPI_MCFG_STRUCTURE& GetMCFG() const
			{
				return acpi->acpi_mcfg;
			}
			const ACPI_RSDP_STRUCTURE & GetRSDP() const
			{
				return acpi->acpi_rsdp;
			}
			const ACPI_RSDT_STRUCTURE& GetRSDT() const
			{
				return acpi->acpi_rsdt;
			}
			const ACPI_XSDT_STRUCTURE & GetXSDT() const 
			{
				return acpi->acpi_xsdt;
			}
			const ACPI_FADT_STRUCTURE& GetFACP() const 
			{
				return acpi->acpi_facp;
			}
			const ACPI_HPET_STRUCTURE& GetHPET() const
			{
				return acpi->acpi_hpet;
			}

			const ACPI_BGRT_STRUCTURE& GetBGRT() const
			{
				if(acpi->acpi_bgrt.get())
					return *(acpi->acpi_bgrt);
				return ACPI_BGRT_STRUCTURE{};
			}

			const ACPI_SBST_STRUCTURE& GetSBST() const
			{
				if (acpi->acpi_sbst.get())
					return *(acpi->acpi_sbst);
				return ACPI_SBST_STRUCTURE{};
			}
			const ACPI_CPEP_STRUCTURE & GetCPEP() const
			{
				if (acpi->acpi_cpep.get())
					return *(acpi->acpi_cpep);
				return ACPI_CPEP_STRUCTURE{};
			}
			const ACPI_FACS_STRUCTURE & GetFACS() const 
			{
				if (acpi->acpi_facs.get())
					return *(acpi->acpi_facs);
				return ACPI_FACS_STRUCTURE{};
			}
			const ACPI_MADT_STRUCTURE& GetAPIC() const
			{
				return acpi->acpi_apic;
			}
		private:
			explicit CACPI_info(const CACPI_info& that);
			CACPI_info& operator=(const CACPI_info& that) {}
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
		const ACPI_RSDP_STRUCTURE & GetRSDP()
		{
			return CACPI_info::Instance()->GetRSDP();
		}
		const ACPI_XSDT_STRUCTURE & GetXSDT()
		{
			return CACPI_info::Instance()->GetXSDT();
		}
		const ACPI_FADT_STRUCTURE& GetFACP()
		{
			return CACPI_info::Instance()->GetFACP();
		}
		const ACPI_HPET_STRUCTURE& GetHPET() 
		{
			return CACPI_info::Instance()->GetHPET();
		}
		const ACPI_BGRT_STRUCTURE& GetBGRT()
		{
			return CACPI_info::Instance()->GetBGRT();
		}
		const ACPI_MADT_STRUCTURE& GetAPIC()
		{
			return CACPI_info::Instance()->GetAPIC();
		}
		const ACPI_SBST_STRUCTURE& GetSBST()
		{
			return CACPI_info::Instance()->GetSBST();
		}
		const ACPI_CPEP_STRUCTURE & GetCPEP()
		{
			return CACPI_info::Instance()->GetCPEP();
		}
		const ACPI_FACS_STRUCTURE & GetFACS()
		{
			return CACPI_info::Instance()->GetFACS();
		}
		const DWORD64 GetPCIEBaseAddress()
		{
			return CACPI_info::Instance()->GetMCFG().Configuration_space_base_address.Base_Address;
		}
	}
}