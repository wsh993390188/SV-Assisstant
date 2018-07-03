// CPU.cpp :
//
#include "stdafx.h"
#include "CPU.h"
#include "Intel.h"
#include "amd.h"
#include "zhaoxin.h"

class CPUDLL
{
	class CPUDLLDATA;
public:
	static CPUDLL* Instance() {
		if (!ttt.get())
			ttt = std::make_shared<CPUDLL>();
		return ttt.get();
	}
	const string& GetManufacturer() { return data->cpu->Manufacturer; }
	const short GetFamily() { return data->cpu->Family; }
	const short GetModel() { return data->cpu->Model; }
	const short GetStepping() { return data->cpu->Stepping; }
	const short GetExtFamily() { return data->cpu->ExtFamily; }
	const short GetExtModel() { return data->cpu->ExtModel; }
	const CPUFeature& GetFeature() { return data->cpu->Feature; }
	const Cache_info* GetCache() { return data->cpu->Cache; }

	const string& GetCPUName() { return data->cpu->Brand; }
	const string& GetProcessorID() { return data->cpu->ProcessorId; }
	const string& GetSocketDesignation() { return data->cpu->SocketDesignation; }
	const UINT GetCurrentClockSpeed() { return data->cpu->CurrentClockSpeed; }
	const UINT GetExtClock() { return data->cpu->ExtClock; }
	const UINT GetCore() { return data->cpu->Core; }
	const UINT GetThread() { return data->cpu->Thread; }
	const UINT GetRevision() { return data->cpu->Revision; }
	const UINT GetMaxClockSpeed() { return data->cpu->MaxClockSpeed; }
	const UINT GetUpgradeMethod() { return data->cpu->UpgradeMethod; }
	const std::string& GetCodename() { return data->cpu->microarchitecture; }
	const UINT GetTechonology() { return data->cpu->Technology; }
	CPUDLL() : data(std::make_shared<CPUDLLDATA>())
	{
	}
private:
	static std::shared_ptr<CPUDLL> ttt;
	std::shared_ptr<CPUDLLDATA> data;
	class CPUDLLDATA
	{
	public:
		CPUDLLDATA()
		{
			switch (CPUBASE::GetCPUBrands())
			{
			case INTEL_:
				cpu = make_shared<Intel>();
				break;
			case ZHAOXIN_:
				cpu = make_shared<Zhaoxin>();
				break;
			case AMD_:
				cpu = make_shared<AMD>();
				break;
			default:
				break;
			}
			this->Update();
		}
		void Update()
		{
			cpu->Init();
			cpu->UpDateData();
		}
		std::shared_ptr<CPUBASE> cpu;
	};
};

std::shared_ptr<CPUDLL> CPUDLL::ttt = nullptr;

const std::string& SV_ASSIST::CPU::GetCPUName()
{
	return CPUDLL::Instance()->GetCPUName();
}

const std::string& SV_ASSIST::CPU::GetProcessorID()
{
	return CPUDLL::Instance()->GetProcessorID();
}
const std::string& SV_ASSIST::CPU::GetSocketDesignation()
{
	return CPUDLL::Instance()->GetSocketDesignation();
}

const UINT SV_ASSIST::CPU::GetCurrentClockSpeed()
{
	return CPUDLL::Instance()->GetCurrentClockSpeed();
}

const UINT SV_ASSIST::CPU::GetExtClock()
{
	return CPUDLL::Instance()->GetExtClock();
}

const UINT SV_ASSIST::CPU::GetCore()
{
	return CPUDLL::Instance()->GetCore();
}

const UINT SV_ASSIST::CPU::GetThread()
{
	return CPUDLL::Instance()->GetThread();
}

const UINT SV_ASSIST::CPU::GetRevision()
{
	return CPUDLL::Instance()->GetRevision();
}

const UINT SV_ASSIST::CPU::GetMaxClockSpeed()
{
	return CPUDLL::Instance()->GetMaxClockSpeed();
}

const UINT SV_ASSIST::CPU::GetUpgradeMethod()
{
	return CPUDLL::Instance()->GetUpgradeMethod();
}

const std::string& SV_ASSIST::CPU::GetManufacturer()
{
	 return CPUDLL::Instance()->GetManufacturer();
}

const short SV_ASSIST::CPU::GetFamily()
{
	return CPUDLL::Instance()->GetFamily();
}

const short SV_ASSIST::CPU::GetModel()
{
	return CPUDLL::Instance()->GetModel();
}

const short SV_ASSIST::CPU::GetStepping()
{
	return CPUDLL::Instance()->GetStepping();
}

const short SV_ASSIST::CPU::GetExtFamily()
{
	return CPUDLL::Instance()->GetExtFamily();
}

const short SV_ASSIST::CPU::GetExtModel()
{
	return CPUDLL::Instance()->GetExtModel();
}

const CPUFeature& SV_ASSIST::CPU::GetFeature()
{
	return CPUDLL::Instance()->GetFeature();
}

const Cache_info * SV_ASSIST::CPU::GetCache()
{
	return CPUDLL::Instance()->GetCache();
}

const std::string& SV_ASSIST::CPU::GetCodeName()
{
	return CPUDLL::Instance()->GetCodename();
}

const unsigned int SV_ASSIST::CPU::GetTechnology()
{
	return CPUDLL::Instance()->GetTechonology();
}

