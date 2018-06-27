// CPU.cpp :
//
#include "stdafx.h"
#include "CPU.h"

class CPUDLL
{
	class CPUDLLDATA;
public:
	static const string& GetManufacturer() { return data.cpu->Manufacturer; }
	static const short GetFamily() { return data.cpu->Family; }
	static const short GetModel() { return data.cpu->Model; }
	static const short GetStepping() { return data.cpu->Stepping; }
	static const short GetExtFamily() { return data.cpu->ExtFamily; }
	static const short GetExtModel() { return data.cpu->ExtModel; }
	static const CPUFeature& GetFeature() { return data.cpu->Feature; }
	static const Cache_info* GetCache() { return data.cpu->Cache; }

	static const string& GetCPUName() { return data.cpu->wmi.Name; }
	static const string& GetProcessorID() { return data.cpu->wmi.ProcessorId; }
	static const string& GetSocketDesignation() { return data.cpu->wmi.SocketDesignation; }
	static const UINT GetCurrentClockSpeed() { return data.cpu->wmi.CurrentClockSpeed; }
	static const UINT GetExtClock() { return data.cpu->wmi.ExtClock; }
	static const UINT GetCore() { return data.cpu->wmi.Core; }
	static const UINT GetThread() { return data.cpu->wmi.Thread; }
	static const UINT GetRevision() { return data.cpu->wmi.Revision; }
	static const UINT GetMaxClockSpeed() { return data.cpu->wmi.MaxClockSpeed; }
	static const UINT GetUpgradeMethod() { return data.cpu->wmi.UpgradeMethod; }
protected:
private:
	static const CPUDLLDATA data;
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

const CPUDLL::CPUDLLDATA CPUDLL::data;

const std::string& SV_ASSIST::CPU::GetCPUName()
{
	return CPUDLL::GetCPUName();
}

const std::string& SV_ASSIST::CPU::GetProcessorID()
{
	return ::CPUDLL::GetProcessorID();
}
const std::string& SV_ASSIST::CPU::GetSocketDesignation()
{
	return CPUDLL::GetSocketDesignation();
}

const UINT SV_ASSIST::CPU::GetCurrentClockSpeed()
{
	return CPUDLL::GetCurrentClockSpeed();
}

const UINT SV_ASSIST::CPU::GetExtClock()
{
	return CPUDLL::GetExtClock();
}

const UINT SV_ASSIST::CPU::GetCore()
{
	return CPUDLL::GetCore();
}

const UINT SV_ASSIST::CPU::GetThread()
{
	return CPUDLL::GetThread();
}

const UINT SV_ASSIST::CPU::GetRevision()
{
	return CPUDLL::GetRevision();
}

const UINT SV_ASSIST::CPU::GetMaxClockSpeed()
{
	return CPUDLL::GetMaxClockSpeed();
}

const UINT SV_ASSIST::CPU::GetUpgradeMethod()
{
	return CPUDLL::GetUpgradeMethod();
}

const std::string& SV_ASSIST::CPU::GetManufacturer()
{
	 return CPUDLL::GetManufacturer();
}

const short SV_ASSIST::CPU::GetFamily()
{
	return CPUDLL::GetFamily();
}

const short SV_ASSIST::CPU::GetModel()
{
	return CPUDLL::GetModel();
}

const short SV_ASSIST::CPU::GetStepping()
{
	return CPUDLL::GetStepping();
}

const short SV_ASSIST::CPU::GetExtFamily()
{
	return CPUDLL::GetExtFamily();
}

const short SV_ASSIST::CPU::GetExtModel()
{
	return CPUDLL::GetExtModel();
}

const CPUFeature& SV_ASSIST::CPU::GetFeature()
{
	return CPUDLL::GetFeature();
}

const Cache_info * SV_ASSIST::CPU::GetCache()
{
	return CPUDLL::GetCache();
}

